/* demo_clone.c

   Демонстрация использования системного вызова Linux clone().

   Программа создаёт дочерний процесс с использованием clone(). 
   Флаги, передаваемые в вызов clone(), задаются в первом аргументе командной строки.
   Программа поддерживает следующие флаги:
     - d: общий доступ к файловым дескрипторам (CLONE_FILES)
     - f: общий доступ к информации о файловой системе (CLONE_FS)
     - s: общий доступ к обработчикам сигналов (CLONE_SIGHAND)
     - v: общий доступ к виртуальной памяти (CLONE_VM)

   Для корректного использования программы читайте документацию clone(2).
   Эта программа специфична для Linux.
*/

#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/mman.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

// Определяем сигнал, используемый для уведомления родителя о завершении дочернего процесса
#ifndef CHILD_SIG
#define CHILD_SIG SIGUSR1
#endif

// Структура для передачи параметров в дочерний процесс
typedef struct {
    int    fd;         // Файловый дескриптор, который закроет дочерний процесс
    mode_t umask;      // umask, который установит дочерний процесс
    int    exitStatus; // Код завершения дочернего процесса
    int    signal;     // Сигнал, который изменит дочерний процесс
} ChildParams;

// Функция, с которой начинается выполнение дочернего процесса
static int childFunc(void *arg) {
    printf("Child: PID=%ld PPID=%ld\n", (long) getpid(), (long) getppid());

    ChildParams *cp = arg;

    // Устанавливаем umask
    umask(cp->umask);

    // Закрываем файловый дескриптор
    if (close(cp->fd) == -1)
        errExit("child:close");

    // Изменяем обработчик сигнала
    if (signal(cp->signal, SIG_DFL) == SIG_ERR)
        errExit("child:signal");

    return cp->exitStatus; // Возвращаем код завершения
}

// Обработчик сигнала завершения дочернего процесса
static void grimReaper(int sig) {
    int savedErrno = errno; // Сохраняем значение errno
    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
    errno = savedErrno; // Восстанавливаем errno
}

// Вывод информации о правильном использовании программы
static void usageError(char *progName) {
    fprintf(stderr, "Usage: %s [arg]\n", progName);
    fprintf(stderr, "    'arg' может содержать следующие буквы:\n");
    fprintf(stderr, "        d - общий доступ к файловым дескрипторам (CLONE_FILES)\n");
    fprintf(stderr, "        f - общий доступ к файловой системе (CLONE_FS)\n");
    fprintf(stderr, "        s - общий доступ к обработчикам сигналов (CLONE_SIGHAND)\n");
    fprintf(stderr, "        v - общий доступ к виртуальной памяти (CLONE_VM)\n");
    exit(EXIT_FAILURE);
}

// Главная функция программы
int main(int argc, char *argv[]) {
    printf("Parent: PID=%ld PPID=%ld\n", (long) getpid(), (long) getppid());

    // Инициализация параметров для дочернего процесса
    ChildParams cp;
    cp.exitStatus = 22; // Код завершения дочернего процесса

    const mode_t START_UMASK = S_IWOTH; // Исходный umask
    umask(START_UMASK);
    cp.umask = S_IWGRP; // Новый umask для дочернего процесса

    cp.fd = open("/dev/null", O_RDWR); // Файловый дескриптор для закрытия дочерним процессом
    if (cp.fd == -1)
        errExit("open");

    cp.signal = SIGTERM; // Сигнал для изменения
    if (signal(cp.signal, SIG_IGN) == SIG_ERR)
        errExit("signal");

    // Определение флагов для вызова clone()
    int flags = 0;
    if (argc > 1) {
        for (char *p = argv[1]; *p != '\0'; p++) {
            if      (*p == 'd') flags |= CLONE_FILES;
            else if (*p == 'f') flags |= CLONE_FS;
            else if (*p == 's') flags |= CLONE_SIGHAND;
            else if (*p == 'v') flags |= CLONE_VM;
            else usageError(argv[0]);
        }
    }

    // Выделение стека для дочернего процесса
    const int STACK_SIZE = 65536;
    char *stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (stack == MAP_FAILED)
        errExit("mmap");
    char *stackTop = stack + STACK_SIZE;

    // Установка обработчика сигнала для родителя
    struct sigaction sa;
    if (CHILD_SIG != 0) {
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = grimReaper;
        if (sigaction(CHILD_SIG, &sa, NULL) == -1)
            errExit("sigaction");
    }

    // Создание дочернего процесса
    if (clone(childFunc, stackTop, flags | CHILD_SIG, &cp) == -1)
        errExit("clone");

    // Освобождение памяти, выделенной под стек
    munmap(stack, STACK_SIZE);

    // Ожидание завершения дочернего процесса
    int status;
    pid_t pid = waitpid(-1, &status, (CHILD_SIG != SIGCHLD) ? __WCLONE : 0);
    if (pid == -1)
        errExit("waitpid");

    printf("    Child PID=%ld\n", (long) pid);
    printWaitStatus("    Status: ", status);

    // Проверка, повлияли ли изменения дочернего процесса на родителя
    printf("Parent - checking process attributes:\n");
    if (umask(0) != START_UMASK)
        printf("    umask has changed\n");
    else
        printf("    umask has not changed\n");

    ssize_t s = write(cp.fd, "Hello world\n", 12);
    if (s == -1 && errno == EBADF)
        printf("    file descriptor %d has been closed\n", cp.fd);
    else if (s == -1)
        printf("    write() on file descriptor %d failed (%s)\n",
                cp.fd, strerror(errno));
    else
        printf("    write() on file descriptor %d succeeded\n", cp.fd);

    if (sigaction(cp.signal, NULL, &sa) == -1)
        errExit("sigaction");
    if (sa.sa_handler != SIG_IGN)
        printf("    signal disposition has changed\n");
    else
        printf("    signal disposition has not changed\n");

    exit(EXIT_SUCCESS);
}

/* Резюме:
   Программа демонстрирует работу с системным вызовом clone(), создающим 
   дочерний процесс с разделением ресурсов. Используемые флаги задаются 
   аргументами командной строки. Включены примеры передачи параметров 
   дочернему процессу и проверки, как изменения дочернего процесса 
   могут влиять на родительский. */

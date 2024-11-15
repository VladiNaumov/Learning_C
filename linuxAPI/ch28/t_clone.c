/* t_clone.c

   Демонстрация использования системного вызова clone(), специфичного для Linux.
*/

#define _GNU_SOURCE
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sched.h>
#include "tlpi_hdr.h"

#ifndef CHILD_SIG
#define CHILD_SIG SIGUSR1       /* Сигнал, который будет сгенерирован при завершении клонированного дочернего процесса */
#endif

/* Функция, с которой начнет выполнение клонированный дочерний процесс */
static int childFunc(void *arg) {
    // Закрыть переданный файловый дескриптор
    if (close(*((int *) arg)) == -1)
        errExit("close");

    return 0; // Завершение работы дочернего процесса
}

int main(int argc, char *argv[]) {
    const int STACK_SIZE = 65536;       // Размер стека для клонированного процесса
    char *stack;                        // Начало буфера для стека
    char *stackTop;                     // Конец буфера для стека
    int s, fd, flags;

    // Открыть файл "/dev/null", который дочерний процесс будет закрывать
    fd = open("/dev/null", O_RDWR);
    if (fd == -1)
        errExit("open");

    // Если передан аргумент, дочерний процесс будет разделять таблицу файловых дескрипторов с родительским
    flags = (argc > 1) ? CLONE_FILES : 0;

    // Выделить память для стека дочернего процесса
    stack = malloc(STACK_SIZE);
    if (stack == NULL)
        errExit("malloc");
    stackTop = stack + STACK_SIZE; // Предполагаем, что стек растет вниз

    // Игнорируем CHILD_SIG, если это не SIGCHLD, чтобы избежать завершения процесса по умолчанию
    if (CHILD_SIG != 0 && CHILD_SIG != SIGCHLD)
        if (signal(CHILD_SIG, SIG_IGN) == SIG_ERR)
            errExit("signal");

    // Создаем дочерний процесс, который начнет выполнение с функции childFunc()
    if (clone(childFunc, stackTop, flags | CHILD_SIG, (void *) &fd) == -1)
        errExit("clone");

    // Родительский процесс ждет завершения дочернего процесса; если используется сигнал, отличный от SIGCHLD, требуется __WCLONE
    if (waitpid(-1, NULL, (CHILD_SIG != SIGCHLD) ? __WCLONE : 0) == -1)
        errExit("waitpid");
    printf("child has terminated\n");

    // Проверка, повлияло ли закрытие файлового дескриптора в дочернем процессе на родительский
    s = write(fd, "x", 1);
    if (s == -1 && errno == EBADF)
        printf("file descriptor %d has been closed\n", fd);
    else if (s == -1)
        printf("write() on file descriptor %d failed unexpectedly (%s)\n", fd, strerror(errno));
    else
        printf("write() on file descriptor %d succeeded\n", fd);

    exit(EXIT_SUCCESS);
}

/*
Резюме:
- Код демонстрирует использование системного вызова `clone()`.
- Основной процесс создает дочерний с помощью `clone()`, передавая ему файловый дескриптор.
- Дочерний процесс закрывает дескриптор, и проверяется, повлияло ли это на родительский процесс.
- `clone()` позволяет управлять тем, какие ресурсы (например, файловые дескрипторы) разделяются между процессами.
*/

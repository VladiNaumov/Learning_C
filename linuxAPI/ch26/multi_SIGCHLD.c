/* multi_SIGCHLD.c

   Демонстрация использования обработчика сигнала SIGCHLD и того, что
   несколько сигналов SIGCHLD не ставятся в очередь, пока сигнал заблокирован
   во время выполнения обработчика.
*/

#include <signal.h>
#include <sys/wait.h>
#include "print_wait_status.h"
#include "curr_time.h"
#include "tlpi_hdr.h"

static volatile int numLiveChildren = 0;
                /* Количество дочерних процессов, которые были запущены, но еще не завершены */

static void
sigchldHandler(int sig)
{
    int status, savedErrno;
    pid_t childPid;

    /* ОПАСНОСТЬ: Этот обработчик использует небезопасные функции для асинхронных сигналов
       (printf(), printWaitStatus(), currTime(); см. раздел 21.1.2) */

    savedErrno = errno;         /* Сохраняем errno, чтобы не повлиять на её изменение в обработчике */

    printf("%s handler: Пойман сигнал SIGCHLD\n", currTime("%T"));

    /* Не блокирующие ожидания, пока не будут найдены все завершенные дочерние процессы */

    while ((childPid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("%s handler: Собран дочерний процесс %ld - ", currTime("%T"),
                (long) childPid);
        printWaitStatus(NULL, status);
        numLiveChildren--;  /* Уменьшаем количество живых дочерних процессов */
    }

    if (childPid == -1 && errno != ECHILD)
        errMsg("waitpid");

    sleep(5);           /* Искусственно увеличиваем время работы обработчика */
    printf("%s handler: возвращаем управление\n", currTime("%T"));

    errno = savedErrno;  /* Восстанавливаем исходное значение errno */
}

int
main(int argc, char *argv[])
{
    int j, sigCnt;
    sigset_t blockMask, emptyMask;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s child-sleep-time...\n", argv[0]);

    setbuf(stdout, NULL);       /* Отключаем буферизацию stdout */

    sigCnt = 0;
    numLiveChildren = argc - 1;  /* Устанавливаем количество живых дочерних процессов */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchldHandler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    /* Блокируем SIGCHLD, чтобы предотвратить его доставку, если дочерний процесс завершится
       до того, как родитель начнёт цикл sigsuspend() ниже */

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1)
        errExit("sigprocmask");

    /* Создаём дочерний процесс для каждого аргумента командной строки */

    for (j = 1; j < argc; j++) {
        switch (fork()) {
        case -1:
            errExit("fork");

        case 0:         /* Дочерний процесс - засыпает и затем завершает работу */
            sleep(getInt(argv[j], GN_NONNEG, "child-sleep-time"));
            printf("%s Child %d (PID=%ld) exiting\n", currTime("%T"),
                    j, (long) getpid());
            _exit(EXIT_SUCCESS);

        default:        /* Родительский процесс - продолжает цикл создания следующего дочернего */
            break;
        }
    }

    /* Родительский процесс здесь: ожидает SIGCHLD, пока все дочерние процессы не завершатся */

    sigemptyset(&emptyMask);
    while (numLiveChildren > 0) {
        if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
            errExit("sigsuspend");
        sigCnt++;  /* Увеличиваем количество пойманных сигналов SIGCHLD */
    }

    printf("%s Все %d дочерних процессов завершены; SIGCHLD был пойман "
            "%d раз\n", currTime("%T"), argc - 1, sigCnt);

    exit(EXIT_SUCCESS);
}

/*

### Резюме кода

Программа демонстрирует обработку сигнала `SIGCHLD` с помощью обработчика, который используется для обработки завершения дочерних процессов. 
Родительский процесс создает несколько дочерних процессов, каждый из которых засыпает на указанное количество секунд, а затем завершается. 
Родительский процесс блокирует сигнал `SIGCHLD`, чтобы предотвратить его доставку, если дочерний процесс завершится до того, как родитель начнёт ожидать сигнал. 
Когда все дочерние процессы завершены, родитель использует `sigsuspend` для ожидания сигналов. 
Обработчик `SIGCHLD` выполняет неблокирующие ожидания завершения дочерних процессов и выводит их статус.

Программа демонстрирует, что если сигнал блокирован во время выполнения обработчика, то последующие сигналы не ставятся в очередь, и обрабатываются только после разблокировки сигнала.
*/
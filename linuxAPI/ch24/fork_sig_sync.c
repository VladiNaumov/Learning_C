Вот обновленный код с переводом и дополнительными комментариями для ясности, а также краткое резюме.

```c
/* fork_sig_sync.c

   Демонстрация того, как сигналы могут быть использованы для синхронизации
   действий родительского и дочернего процессов.
*/

#include <signal.h>
#include "curr_time.h"                  /* Объявление функции currTime() */
#include "tlpi_hdr.h"

#define SYNC_SIG SIGUSR1                /* Сигнал синхронизации */

static void             /* Обработчик сигнала - ничего не делает, просто возвращает управление */
handler(int sig)
{
}

int
main(int argc, char *argv[])
{
    pid_t childPid;
    sigset_t blockMask, origMask, emptyMask;
    struct sigaction sa;

    setbuf(stdout, NULL);               /* Отключение буферизации stdout */

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SYNC_SIG);    /* Блокировка сигнала синхронизации */
    if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
        errExit("sigprocmask");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SYNC_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    switch (childPid = fork()) {
    case -1:
        errExit("fork");

    case 0: /* Дочерний процесс */

        /* Дочерний процесс выполняет необходимую работу... */

        printf("[%s %ld] Child started - doing some work\n",
                currTime("%T"), (long) getpid());
        sleep(2);               /* Симуляция времени, затраченного на выполнение работы */

        /* И затем сигнализирует родителю, что работа завершена */

        printf("[%s %ld] Child about to signal parent\n",
                currTime("%T"), (long) getpid());
        if (kill(getppid(), SYNC_SIG) == -1)
            errExit("kill");

        /* Теперь дочерний процесс может продолжить выполнение... */

        _exit(EXIT_SUCCESS);

    default: /* Родительский процесс */

        /* Родитель может выполнить некоторую работу, а затем ждать сигнал от дочернего процесса */

        printf("[%s %ld] Parent about to wait for signal\n",
                currTime("%T"), (long) getpid());
        sigemptyset(&emptyMask);
        if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
            errExit("sigsuspend");
        printf("[%s %ld] Parent got signal\n", currTime("%T"), (long) getpid());

        /* Если необходимо, восстанавливаем исходную маску сигналов */

        if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
            errExit("sigprocmask");

        /* Родитель продолжает выполнение других задач... */

        exit(EXIT_SUCCESS);
    }
}
/*
### Резюме кода
Этот код демонстрирует, как родительский и дочерний процессы могут синхронизировать свои действия с помощью сигналов. 
После того как дочерний процесс завершит некоторую работу (симулируемую задержкой в 2 секунды), он посылает родителю сигнал синхронизации. 
Родитель, в свою очередь, блокирует сигнал до тех пор, пока не получит его, после чего продолжает выполнение. 
Использование `sigprocmask` и `sigsuspend` позволяет контролировать маски сигналов и приостановить выполнение родителя до получения сигнала от дочери.
*/
/* sig_receiver.c

   Использование: sig_receiver [время блокировки]

   Улавливает и отображает статистику по сигналам, отправленным программой sig_sender.c.

   Обратите внимание, что, хотя в этой программе для установки обработчика сигнала
   используется функция signal(), предпочтительнее всегда использовать sigaction() для этой задачи.
*/
#define _GNU_SOURCE
#include <signal.h>
#include "signal_functions.h"           /* Объявление функции printSigset() */
#include "tlpi_hdr.h"

static int sigCnt[NSIG];                /* Подсчёт каждого полученного сигнала */
static volatile sig_atomic_t gotSigint = 0;
                                        /* Устанавливается в ненулевое значение, если получен SIGINT */

static void
handler(int sig)
{
    if (sig == SIGINT)
        gotSigint = 1;
    else
        sigCnt[sig]++;
}

int
main(int argc, char *argv[])
{
    int n, numSecs;
    sigset_t pendingMask, blockingMask, emptyMask;

    printf("%s: PID процесса — %ld\n", argv[0], (long) getpid());

    /* Здесь мы используем более простой API signal() для установки обработчика сигнала,
       но по причинам, описанным в Разделе 22.7 TLPI, sigaction()
       является (сильно) предпочтительным API для этой задачи. */

    for (n = 1; n < NSIG; n++)          /* Один и тот же обработчик для всех сигналов */
        (void) signal(n, handler);      /* Игнорируем ошибки */

    /* Если указано время задержки, временно блокируем все сигналы,
       приостанавливаем работу (пока другой процесс отправляет сигналы),
       затем отображаем маску ожидающих сигналов и разблокируем все сигналы */

    if (argc > 1) {
        numSecs = getInt(argv[1], GN_GT_0, NULL);

        sigfillset(&blockingMask);
        if (sigprocmask(SIG_SETMASK, &blockingMask, NULL) == -1)
            errExit("sigprocmask");

        printf("%s: приостановка на %d секунд\n", argv[0], numSecs);
        sleep(numSecs);

        if (sigpending(&pendingMask) == -1)
            errExit("sigpending");

        printf("%s: ожидающие сигналы:\n", argv[0]);
        printSigset(stdout, "\t\t", &pendingMask);

        sigemptyset(&emptyMask);        /* Разблокировка всех сигналов */
        if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1)
            errExit("sigprocmask");
    }

    while (!gotSigint)                  /* Цикл до тех пор, пока не пойман SIGINT */
        continue;

    for (n = 1; n < NSIG; n++)          /* Отображение количества полученных сигналов */
        if (sigCnt[n] != 0)
            printf("%s: сигнал %d получен %d раз%s\n", argv[0], n,
                    sigCnt[n], (sigCnt[n] == 1) ? "" : "а");

    exit(EXIT_SUCCESS);
}
/*

### Резюме кода

Эта программа обрабатывает и учитывает сигналы, отправленные программой `sig_sender.c`. 
Она устанавливает обработчик для всех сигналов и подсчитывает их количество. 
Если задано время задержки, программа блокирует все сигналы на это время, а затем отображает список ожидающих сигналов. 
Получив сигнал SIGINT (обычно генерируемый Ctrl+C), программа завершает цикл и выводит статистику по каждому пойманному сигналу.
*/
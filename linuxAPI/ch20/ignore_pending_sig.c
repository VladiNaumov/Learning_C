/* ignore_pending_sig.c

   Эта программа демонстрирует, что произойдет, если пометить ожидающий сигнал
   (т.е. сигнал, который был отправлен, но в данный момент заблокирован) как игнорируемый.

   Использование: ignore_pending_sig

   Нажмите Control-C (^C), чтобы сгенерировать сигнал SIGINT после того,
   как программа выведет сообщение "sleeping" (см. ниже).
*/
#define _GNU_SOURCE     /* Получить объявление strsignal() из <string.h> */
#include <string.h>
#include <signal.h>
#include "signal_functions.h"   /* Объявление printSigset() */
#include "tlpi_hdr.h"

static void
handler(int sig)
{
    /* НЕБЕЗОПАСНО: Этот обработчик использует функции, небезопасные
       для асинхронных сигналов (printf(), strsignal(), fflush(); см. Раздел 21.1.2) */

    printf("Пойман сигнал %d (%s)\n", sig, strsignal(sig));
    fflush(NULL);
}

int
main(int argc, char *argv[])
{
    const int numSecs = 5;

    /* Установить обработчик для SIGINT */

    printf("Установка обработчика для SIGINT\n");

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    /* Заблокировать SIGINT на некоторое время */

    sigset_t blocked;
    sigemptyset(&blocked);
    sigaddset(&blocked, SIGINT);
    if (sigprocmask(SIG_SETMASK, &blocked, NULL) == -1)
        errExit("sigprocmask");

    printf("БЛОКИРОВКА SIGINT на %d секунд\n", numSecs);
    sleep(numSecs);

    /* Отобразить маску ожидающих сигналов */

    sigset_t pending;
    if (sigpending(&pending) == -1)
        errExit("sigpending");
    printf("ОЖИДАЮЩИЕ сигналы:\n");
    printSigset(stdout, "\t\t", &pending);

    /* Теперь игнорируем SIGINT */

    sleep(2);
    printf("Игнорирование SIGINT\n");
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)     errExit("signal");

    /* Повторно отобразить маску ожидающих сигналов */

    if (sigpending(&pending) == -1)
        errExit("sigpending");
    if (sigismember(&pending, SIGINT)) {
        printf("SIGINT теперь ожидает обработки\n");
    } else {
        printf("ОЖИДАЮЩИЕ сигналы:\n");
        printSigset(stdout, "\t\t", &pending);
    }
    sleep(2);

    /* Восстановить обработчик для SIGINT */

    printf("Восстановление обработчика для SIGINT\n");
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    sleep(2);

    /* И разблокировать SIGINT */

    printf("РАЗБЛОКИРОВКА SIGINT\n");
    sigemptyset(&blocked);
    if (sigprocmask(SIG_SETMASK, &blocked, NULL) == -1)
        errExit("sigprocmask");

    exit(EXIT_SUCCESS);
}
/*

### Резюме кода

Программа демонстрирует работу с сигналом SIGINT (генерируемым, например, с помощью Ctrl+C). 
Она устанавливает обработчик для SIGINT и временно блокирует его. 
Пока сигнал заблокирован, программа проверяет, есть ли он в маске ожидающих сигналов, а затем игнорирует SIGINT, что удаляет его из очереди. 
Затем программа восстанавливает обработчик и разблокирует SIGINT. 
Этот процесс показывает, что происходит с заблокированным и ожидающим сигналом, если его пометить как игнорируемый.
*/
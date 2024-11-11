Вот переведённый код с комментариями:

```c
/* t_sigwaitinfo.c

   Демонстрация использования sigwaitinfo() для синхронного ожидания сигнала.
*/
#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include <time.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int sig;
    siginfo_t si;
    sigset_t allSigs;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [delay-secs]\n", argv[0]);

    printf("%s: PID is %ld\n", argv[0], (long) getpid());

    /* Блокируем все сигналы (кроме SIGKILL и SIGSTOP) */

    sigfillset(&allSigs);
    if (sigprocmask(SIG_SETMASK, &allSigs, NULL) == -1)
        errExit("sigprocmask");
    printf("%s: сигналы заблокированы\n", argv[0]);

    if (argc > 1) {             /* Задержка, чтобы сигналы могли быть отправлены */
        printf("%s: готовимся к задержке на %s секунд\n", argv[0], argv[1]);
        sleep(getInt(argv[1], GN_GT_0, "delay-secs"));
        printf("%s: задержка завершена\n", argv[0]);
    }

    for (;;) {                  /* Получаем сигналы до SIGINT (^C) или SIGTERM */
        sig = sigwaitinfo(&allSigs, &si);
        if (sig == -1)
            errExit("sigwaitinfo");

        if (sig == SIGINT || sig == SIGTERM)
            exit(EXIT_SUCCESS);  // Выход, если сигнал SIGINT или SIGTERM

        printf("Получен сигнал: %d (%s)\n", sig, strsignal(sig));
        printf("    si_signo=%d, si_code=%d (%s), si_value=%d\n",
                si.si_signo, si.si_code,
                (si.si_code == SI_USER) ? "SI_USER" :
                    (si.si_code == SI_QUEUE) ? "SI_QUEUE" : "другой",
                si.si_value.sival_int);
        printf("    si_pid=%ld, si_uid=%ld\n",
                (long) si.si_pid, (long) si.si_uid);
    }
}


/*
   - Объяснение блокировки сигналов и действия программы в случае задержки или получения сигналов.
   - Программа блокирует все сигналы с помощью `sigprocmask` (кроме тех, что нельзя заблокировать, таких как `SIGKILL` и `SIGSTOP`), затем с помощью `sigwaitinfo` она синхронно ожидает сигналы.
   - Когда сигнал получен, программа выводит информацию о сигнале, такой как его номер, код, данные и идентификаторы процесса и пользователя, который отправил сигнал.

 **Процесс работы**:
   - Программа блокирует все сигналы, чтобы они не могли прервать выполнение программы до тех пор, пока она не будет готова обработать их через `sigwaitinfo`.
   - Программа будет ожидать сигналы в бесконечном цикле, выводя информацию о каждом полученном сигнале, пока не получит либо `SIGINT`, либо `SIGTERM`, после чего завершится.

 **Использование `sigwaitinfo()`**:
   - `sigwaitinfo()` используется для синхронного ожидания сигнала из набора заблокированных сигналов. Когда сигнал приходит, программа извлекает информацию о сигнале из структуры `siginfo_t`, такой как процесс, который его отправил, код сигнала и другие данные.
   
*/
/* catch_rtsigs.c

   Использование: catch_rtsigs [block-time [handler-sleep-time]]
                              по умолчанию=0   по умолчанию=1

        block-time указывает количество времени, которое программа
                должна приостановить выполнение после блокировки всех сигналов.
                Это позволяет отправить несколько сигналов процессу до того,
                как он разблокирует все сигналы.

        handler-sleep-time указывает, сколько времени обработчик сигнала
                должен спать перед возвращением. Использование ненулевого значения
                позволяет замедлить процесс, чтобы мы могли увидеть, что происходит,
                когда отправляется несколько сигналов.

   После (опциональной) блокировки всех (возможных) сигналов и сна на 'block-time' секунд
   программа заходит в цикл и непрерывно использует pause(), чтобы ожидать входящих сигналов.

   Программа может быть завершена, если в неё отправить сигнал SIGINT (control-C)
   или SIGTERM.
*/

#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include "tlpi_hdr.h"

static volatile int handlerSleepTime;
static volatile int sigCnt = 0;         /* Количество полученных сигналов */
static volatile sig_atomic_t allDone = 0;

static void             /* Обработчик для сигналов, установленных с SA_SIGINFO */
siginfoHandler(int sig, siginfo_t *si, void *ucontext)
{
    /* НЕБЕЗОПАСНО: Этот обработчик использует небезопасные для асинхронных сигналов функции
       (printf()); см. Раздел 21.1.2) */

    /* SIGINT или SIGTERM могут использоваться для завершения программы */

    if (sig == SIGINT || sig == SIGTERM) {
        allDone = 1;
        return;
    }

    sigCnt++;
    printf("Пойман сигнал %d\n", sig);

    printf("    si_signo=%d, si_code=%d (%s), ", si->si_signo, si->si_code,
            (si->si_code == SI_USER) ? "SI_USER" :
            (si->si_code == SI_QUEUE) ? "SI_QUEUE" : "другой");
    printf("si_value=%d\n", si->si_value.sival_int);
    printf("    si_pid=%ld, si_uid=%ld\n",
            (long) si->si_pid, (long) si->si_uid);

    sleep(handlerSleepTime);
}

int
main(int argc, char *argv[])
{
    struct sigaction sa;
    int sig;
    sigset_t prevMask, blockMask;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [block-time [handler-sleep-time]]\n", argv[0]);

    printf("%s: PID is %ld\n", argv[0], (long) getpid());

    handlerSleepTime = (argc > 2) ? 
                getInt(argv[2], GN_NONNEG, "handler-sleep-time") : 1;

    /* Устанавливаем обработчик для большинства сигналов. Во время выполнения обработчика
       блокируем все остальные сигналы, чтобы предотвратить рекурсивное прерывание обработчиков
       друг друга (что сделает вывод трудно читаемым). */

    sa.sa_sigaction = siginfoHandler;
    sa.sa_flags = SA_SIGINFO;
    sigfillset(&sa.sa_mask);

    for (sig = 1; sig < NSIG; sig++)
        if (sig != SIGTSTP && sig != SIGQUIT)
            sigaction(sig, &sa, NULL);

    /* Опционально блокируем сигналы и спим, чтобы сигналы могли быть отправлены нам
       до того, как они будут разблокированы и обработаны */

    if (argc > 1) {
        sigfillset(&blockMask);
        sigdelset(&blockMask, SIGINT);
        sigdelset(&blockMask, SIGTERM);

        if (sigprocmask(SIG_SETMASK, &blockMask, &prevMask) == -1)
            errExit("sigprocmask");

        printf("%s: сигналы заблокированы - спим %s секунд\n", argv[0], argv[1]);
        sleep(getInt(argv[1], GN_GT_0, "block-time"));
        printf("%s: сон завершен\n", argv[0]);

        if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
            errExit("sigprocmask");
    }

    while (!allDone)                    /* Ожидаем входящих сигналов */
        pause();

    printf("Поймано %d сигналов\n", sigCnt);
    exit(EXIT_SUCCESS);
}
/*
 **Описание работы программы**:
   - Программа демонстрирует обработку сигналов с помощью `sigaction`, где сигналы обрабатываются через функцию `siginfoHandler` с использованием флага `SA_SIGINFO` для получения дополнительной информации о сигнале.
   - Программа может блокировать сигналы, за исключением SIGINT и SIGTERM, чтобы предотвратить их обработку до тех пор, пока не пройдет заданное время блокировки (если указано в аргументах).
   - После блокировки программа засыпает на указанное время и затем разблокирует сигналы для их обработки.
   - Программа ожидает сигналов с помощью `pause()` и обрабатывает их, выводя информацию о сигнале.

**Использование обработчиков сигналов**:
   - Для обработки сигналов используется `sigaction`, а не простые сигнал-обработчики. Этот метод позволяет более гибко работать с сигналами, включая возможность получения дополнительной информации о сигнале через `siginfo_t`.
   
**Завершение работы программы**:
   - Если сигнал SIGINT или SIGTERM получен, программа завершает выполнение, а перед этим выводит количество пойманных сигналов.
   
*/
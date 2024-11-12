/* sig_speed_sigsuspend.c

   Эта программа измеряет, как быстро отправляются и принимаются сигналы.

   Программа порождает родительский и дочерний процессы, которые поочередно
   отправляют друг другу сигналы (первым начинает дочерний процесс). Каждый процесс 
   обрабатывает сигнал с помощью обработчика и ожидает его с помощью sigsuspend().

   Использование: $ time ./sig_speed_sigsuspend num-sigs

   Аргумент 'num-sigs' указывает, сколько раз родитель и дочерний процесс
   должны обменяться сигналами.

   Дочерний процесс                            Родительский процесс

   for (s = 0; s < numSigs; s++) {             for (s = 0; s < numSigs; s++) {
       отправка сигнала родителю                   ожидание сигнала от дочери
       ожидание сигнала от родителя               отправка сигнала дочери
   }                                           }
*/
#include <signal.h>
#include "tlpi_hdr.h"

static void
handler(int sig)
{
}

#define TESTSIG SIGUSR1

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-sigs\n", argv[0]);

    int numSigs = getInt(argv[1], GN_GT_0, "num-sigs");

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(TESTSIG, &sa, NULL) == -1)
        errExit("sigaction");

    /* Блокируем сигнал до fork(), чтобы дочерний процесс не успел отправить
       сигнал родителю до того, как родитель будет готов его перехватить */

    sigset_t blockedMask, emptyMask;
    sigemptyset(&blockedMask);
    sigaddset(&blockedMask, TESTSIG);
    if (sigprocmask(SIG_SETMASK, &blockedMask, NULL) == -1)
        errExit("sigprocmask");

    sigemptyset(&emptyMask);

    pid_t childPid = fork();
    switch (childPid) {
    case -1: errExit("fork");

    case 0:     /* дочерний процесс */
        for (int scnt = 0; scnt < numSigs; scnt++) {
            if (kill(getppid(), TESTSIG) == -1)
                errExit("kill");
            if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
                    errExit("sigsuspend");
        }
        exit(EXIT_SUCCESS);

    default: /* родительский процесс */
        for (int scnt = 0; scnt < numSigs; scnt++) {
            if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
                    errExit("sigsuspend");
            if (kill(childPid, TESTSIG) == -1)
                errExit("kill");
        }
        exit(EXIT_SUCCESS);
    }
}
/*

### Описание работы программы

Программа `sig_speed_sigsuspend.c` измеряет скорость отправки и приема сигналов между родительским и дочерним процессами. Она выполняет следующие действия:

1. **Инициализация сигналов:** Используется сигнал `SIGUSR1` для обмена между процессами.
2. **Блокировка сигналов:** Перед `fork()` блокируется сигнал, чтобы дочерний процесс не отправил сигнал родителю до того, как родитель будет готов его ловить.
3. **Создание процессов:** Программа создает два процесса: родительский и дочерний. Оба процесса поочередно отправляют друг другу сигнал с использованием `kill()` и ожидают сигнала с помощью `sigsuspend()`.
4. **Измерение времени:** Количество передаваемых сигналов между процессами задается аргументом `num-sigs`.
*/
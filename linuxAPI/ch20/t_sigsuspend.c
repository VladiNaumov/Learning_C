/* t_sigsuspend.c

   Короткая программа для демонстрации того, почему sigsuspend(&mask) предпочтительнее
   по сравнению с отдельным вызовом sigprocmask(SIG_SETMASK, &mask, NULL) + pause().
   (По умолчанию эта программа использует sigsuspend(). Чтобы использовать pause(),
   компилируйте с флагом "cc -DUSE_PAUSE".)

   Usage: t_sigsuspend [sleep-time]

   Отправьте сигнал SIGINT в эту программу, нажав control-C (^C).
   (Завершите программу с помощью SIGQUIT, т.е. нажав control-\ (^\).)

   Эта программа содержит дополнительный код, который не представлен в версии, показанной
   в книге. При определении USE_PAUSE при компиляции мы можем заменить использование
   sigsuspend() на атомарную sigprocmask() + pause(). Это позволяет нам показать, что
   при использовании второго способа некоторые сигналы могут быть потеряны.
*/
#define _GNU_SOURCE     /* Получить объявление strsignal() из <string.h> */
#include <string.h>
#include <signal.h>
#include <time.h>
#include "signal_functions.h"           /* Объявления printSigMask()
                                           и printPendingSigs() */
#include "tlpi_hdr.h"

/* Глобальная переменная, увеличиваемая каждый раз, когда обрабатывается SIGINT */
static volatile int sigintCnt = 0;
static volatile sig_atomic_t gotSigquit = 0;

/* Обработчик сигналов */
static void
handler(int sig)
{
    printf("Пойман сигнал %d (%s)\n", sig, strsignal(sig));  // UNSAFE (см. раздел 21.1.2)
    if (sig == SIGQUIT)
        gotSigquit = 1;  // Устанавливаем флаг, если это был сигнал SIGQUIT
    sigintCnt++;  // Увеличиваем счётчик сигналов SIGINT
}

int
main(int argc, char *argv[])
{
    int loopNum;
#ifdef USE_PAUSE
    int sleepTime;
#endif
    time_t startTime;
    sigset_t origMask, blockMask;
    struct sigaction sa;

    printSigMask(stdout, "Начальная маска сигналов:\n");

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGINT);
    sigaddset(&blockMask, SIGQUIT);

#ifdef USE_PAUSE
    sleepTime = (argc > 1) ? getInt(argv[1], GN_NONNEG, NULL) : 0;  // Время сна, если указано
#endif

    /* Блокируем SIGINT и SIGQUIT - на данном этапе предполагаем, что эти сигналы
       не заблокированы (что очевидно для этой простой программы), так что
       'origMask' не будет содержать эти сигналы после вызова. */
    if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
        errExit("sigprocmask - SIG_BLOCK");

    /* Устанавливаем обработчики для SIGINT и SIGQUIT */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
        errExit("sigaction");

    /* Цикл до получения SIGQUIT */
    for (loopNum = 1; !gotSigquit; loopNum++) {
        printf("=== ЦИКЛ %d\n", loopNum);

        /* Эмуляция критической секции, задержка на несколько секунд */
        printSigMask(stdout, "Начало критической секции, маска сигналов:\n");
        for (startTime = time(NULL); time(NULL) < startTime + 4; )
            continue;  // Имитируем работу в течение нескольких секунд

#ifndef USE_PAUSE
        /* Правильный способ: используем sigsuspend() для атомарного разблокирования
           сигналов и ожидания сигнала */
        printPendingSigs(stdout,
                "Перед sigsuspend() - ожидающие сигналы:\n");
        if (sigsuspend(&origMask) == -1 && errno != EINTR)
            errExit("sigsuspend");
#else

        /* Неправильный способ: сначала разблокируем сигнал через sigprocmask(),
           затем вызываем pause() */
        if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
            errExit("sigprocmask - SIG_SETMASK");

        /* В этот момент, если поступит сигнал SIGINT, он будет пойман и обработан
           до того, как мы вызовем pause(), и pause() заблокирует выполнение. 
           (Только следующий сигнал SIGINT после pause() фактически прервет выполнение.) */

        if (sleepTime > 0) {
            printf("Разблокировали SIGINT, теперь ждем %d секунд\n", sleepTime);
            for (startTime = time(NULL);
                    time(NULL) < startTime + sleepTime; )
                continue;
            printf("Завершили ожидание - теперь переходим к pause()\n");
        }

        /* Теперь ожидаем сигнал */
        pause();

        printf("Количество сигналов = %d\n", sigintCnt);
        sigintCnt = 0;
#endif
    }

    /* Восстанавливаем маску сигналов, чтобы сигналы снова могли быть обработаны */
    if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
        errExit("sigprocmask - SIG_SETMASK");

    printSigMask(stdout, "=== Выход из цикла\nВосстановленная маска сигналов:\n");

    /* Выполняем другие действия... */

    exit(EXIT_SUCCESS);
}

/*
 **Резюме**:
   - Программа демонстрирует два способа ожидания сигналов: с использованием `sigsuspend()` и через комбинацию `sigprocmask()` и `pause()`. 
   Это позволяет показать, что первый способ является безопасным и атомарным, тогда как второй способ может привести к потерям сигналов, 
   если они поступают в промежуток между разблокировкой сигнала и вызовом `pause()`.
 */
/* ptmr_null_evp.c

   Программа для демонстрации использования POSIX таймеров с NULL в качестве 
   аргумента 'evp' в функции timer_create(). 

   Поддержка Linux таймеров была введена начиная с ядра 2.6. На старых системах 
   существовала неполная пользовательская реализация POSIX таймеров в glibc.
*/

#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <time.h>
#include "curr_time.h"  /* Декларация функции currTime() */
#include "tlpi_hdr.h"

static void handler(int sig, siginfo_t *si, void *uc) {
    printf("[%s] Получен сигнал %d\n", currTime("%T"), sig);
    printf("    sival_int          = %d\n", si->si_value.sival_int);
#ifdef __linux__
    printf("    si_overrun         = %d\n", si->si_overrun);
#endif
    printf("    timer_getoverrun() = %d\n",
            timer_getoverrun((timer_t) si->si_value.sival_ptr));
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        usageErr("%s secs [nsecs [int-secs [int-nsecs]]]\n", argv[0]);

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) == -1)
        errExit("sigaction");

    timer_t tid;
    if (timer_create(CLOCK_REALTIME, NULL, &tid) == -1)
        errExit("timer_create");
    printf("ID таймера = %ld\n", (long) tid);

    struct itimerspec ts;
    ts.it_value.tv_sec = atoi(argv[1]);
    ts.it_value.tv_nsec = (argc > 2) ? atoi(argv[2]) : 0;
    ts.it_interval.tv_sec = (argc > 3) ? atoi(argv[3]) : 0;
    ts.it_interval.tv_nsec = (argc > 4) ? atoi(argv[4]) : 0;
    if (timer_settime(tid, 0, &ts, NULL) == -1)
        errExit("timer_settime");

    for (int j = 0; ; j++)
        pause();
}

/*
### Объяснение:
- **Обработчик сигналов (`handler`)**: При получении сигнала SIGALRM выводится информация о сигнале, а также данные о таймере, включая его количество пропущенных срабатываний и текущие значения с помощью `timer_getoverrun()`.
- **Создание таймера**: В программе создается POSIX таймер с помощью `timer_create()` с NULL в качестве аргумента `evp`, что означает, что не используется структура с дополнительными параметрами.
- **Настройка таймера**: С помощью `timer_settime()` устанавливаются начальное время и интервал для таймера, который будет срабатывать через указанный промежуток времени.
- **Основной цикл**: Программа использует бесконечный цикл, в котором вызывается `pause()` для приостановки программы, пока не поступит сигнал.
*/
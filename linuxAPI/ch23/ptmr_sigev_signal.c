/* ptmr_sigev_signal.c

   Эта программа демонстрирует использование сигналов как механизма уведомления
   об истечении POSIX таймера. Каждый аргумент командной строки программы
   задает начальное значение и интервал для POSIX таймера. Формат этих аргументов
   определен функцией itimerspecFromStr().

   Программа устанавливает обработчик для сигнала уведомления таймера, создает
   и активирует по одному таймеру для каждого аргумента командной строки, а затем
   переходит в ожидание. Каждое истечение таймера вызывает генерацию сигнала, и
   когда обработчик вызывается, он отображает информацию об истечении таймера.

   Поддержка таймеров в ядре Linux добавлена с версии Linux 2.6. На более старых
   системах была предоставлена неполная реализация POSIX таймеров в glibc.
*/

#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <time.h>
#include "curr_time.h"                  /* Объявляет currTime() */
#include "itimerspec_from_str.h"        /* Объявляет itimerspecFromStr() */
#include "tlpi_hdr.h"

#define TIMER_SIG SIGRTMAX              /* Сигнал уведомления для таймеров */

static void handler(int sig, siginfo_t *si, void *uc) {
    timer_t *tidptr;

    tidptr = si->si_value.sival_ptr;

    /* НЕБЕЗОПАСНО: Этот обработчик использует функции, не предназначенные для
       вызова из обработчиков сигналов (printf(); см. раздел 21.1.2) */

    printf("[%s] Получен сигнал %d\n", currTime("%T"), sig);
    printf("    *sival_ptr         = %ld\n", (long) *tidptr);
    printf("    timer_getoverrun() = %d\n", timer_getoverrun(*tidptr));
}

int main(int argc, char *argv[]) {
    struct itimerspec ts;
    struct sigaction sa;
    struct sigevent sev;
    timer_t *tidlist;
    int j;

    if (argc < 2)
        usageErr("%s secs[/nsecs][:int-secs[/int-nsecs]]...\n", argv[0]);

    tidlist = calloc(argc - 1, sizeof(timer_t));
    if (tidlist == NULL)
        errExit("malloc");

    /* Устанавливаем обработчик для сигнала уведомления */

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(TIMER_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    /* Создаем и запускаем таймер для каждого аргумента командной строки */

    sev.sigev_notify = SIGEV_SIGNAL;    /* Уведомление через сигнал */
    sev.sigev_signo = TIMER_SIG;        /* Используем этот сигнал */

    for (j = 0; j < argc - 1; j++) {
        itimerspecFromStr(argv[j + 1], &ts);

        sev.sigev_value.sival_ptr = &tidlist[j];
                /* Позволяет обработчику получить ID этого таймера */

        if (timer_create(CLOCK_REALTIME, &sev, &tidlist[j]) == -1)
            errExit("timer_create");
        printf("ID таймера: %ld (%s)\n", (long) tidlist[j], argv[j + 1]);

        if (timer_settime(tidlist[j], 0, &ts, NULL) == -1)
            errExit("timer_settime");
    }

    for (;;)                            /* Ожидание поступающих сигналов таймера */
        pause();
}

/*
### Объяснение:
- **Обработчик сигнала** `handler`: Функция вызывается при истечении таймера и выводит информацию о таймере, используя указатель `sival_ptr` для получения его идентификатора. `timer_getoverrun()` используется для получения количества пропущенных срабатываний таймера.
  
- **Основной код**:
  - **Создание таймеров**: Каждый таймер создается и настраивается с помощью `timer_create` и `timer_settime` соответственно. 
  - **Ожидание сигналов**: Программа входит в бесконечный цикл с `pause()`, чтобы ждать сигналы от таймеров.
  
- **Параметры командной строки**: Аргументы командной строки задают начальное время и интервал для каждого таймера, 
что позволяет настроить множественные таймеры одновременно.
*/
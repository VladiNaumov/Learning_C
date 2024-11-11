/* t_clock_nanosleep.c

   Демонстрация использования clock_nanosleep() для выполнения паузы
   на заданный временной интервал в наносекундах.

   Поддержка clock_nanosleep() появилась в Linux начиная с ядра 2.6.
*/

#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 600
#define _XOPEN_SOURCE 600
#endif
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void sigintHandler(int sig) {
    return;  /* Просто прерывает clock_nanosleep() */
}

int main(int argc, char *argv[]) {
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s secs nanosecs [a]\n", argv[0]);

    /* Установка обработчика для SIGINT для прерывания clock_nanosleep() */

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigintHandler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    /* Если передано более трех аргументов командной строки, используется флаг TIMER_ABSTIME */

    int flags = (argc > 3) ? TIMER_ABSTIME : 0;

    struct timespec request;

    if (flags == TIMER_ABSTIME) {
        if (clock_gettime(CLOCK_REALTIME, &request) == -1)
            errExit("clock_gettime");
        printf("Начальное значение CLOCK_REALTIME: %ld.%09ld\n",
                (long) request.tv_sec, request.tv_nsec);

        request.tv_sec  += getLong(argv[1], 0, "secs");
        request.tv_nsec += getLong(argv[2], 0, "nanosecs");
        if (request.tv_nsec >= 1000000000) {
            request.tv_sec += request.tv_nsec / 1000000000;
            request.tv_nsec %= 1000000000;
        }

    } else {                    /* Относительное ожидание */
        request.tv_sec  = getLong(argv[1], 0, "secs");
        request.tv_nsec = getLong(argv[2], 0, "nanosecs");
    }

    struct timeval start, finish;
    if (gettimeofday(&start, NULL) == -1)
        errExit("gettimeofday");

    for (;;) {
        struct timespec remain;
        int s = clock_nanosleep(CLOCK_REALTIME, flags, &request, &remain);
        if (s != 0 && s != EINTR)
            errExitEN(s, "clock_nanosleep");

        if (s == EINTR)
            printf("Прервано... ");

        if (gettimeofday(&finish, NULL) == -1)
            errExit("gettimeofday");
        printf("Время сна: %.6f секунд", finish.tv_sec - start.tv_sec +
                        (finish.tv_usec - start.tv_usec) / 1000000.0);

        if (s == 0)
            break;  /* Сон завершен */

        if (flags != TIMER_ABSTIME) {
            printf("... Осталось: %ld.%09ld",
                    (long) remain.tv_sec, remain.tv_nsec);

            request = remain;
        }

        printf("... Перезапуск\n");
    }

    printf("\nСон завершен\n");
    exit(EXIT_SUCCESS);
}

/*
### Объяснение:
- `sigintHandler` — обработчик, который прерывает `clock_nanosleep()` по сигналу `SIGINT`.
- `clock_nanosleep()` выполняет паузу на заданный временной интервал. Если программа прерывается, оставшееся время сохраняется в `remain`, и цикл возобновляется с этим значением.
- Если передан флаг `TIMER_ABSTIME`, используется абсолютное время ожидания.
*/
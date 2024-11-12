/* t_nanosleep.c

   Демонстрация использования nanosleep() для сна на интервал, указанный в наносекундах.

   См. также t_clock_nanosleep.c.
*/

#define _POSIX_C_SOURCE 199309
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void sigintHandler(int sig) {
    return;  /* Просто прерывает nanosleep() */
}

int main(int argc, char *argv[]) {
    struct timeval start, finish;
    struct timespec request, remain;
    struct sigaction sa;
    int s;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s secs nanosecs\n", argv[0]);

    request.tv_sec = getLong(argv[1], 0, "secs");
    request.tv_nsec = getLong(argv[2], 0, "nanosecs");

    /* Разрешаем обработку сигнала SIGINT для прерывания nanosleep() */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigintHandler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    if (gettimeofday(&start, NULL) == -1)
        errExit("gettimeofday");

    for (;;) {
        s = nanosleep(&request, &remain);
        if (s == -1 && errno != EINTR)
            errExit("nanosleep");

        if (gettimeofday(&finish, NULL) == -1)
            errExit("gettimeofday");

        printf("Заснул на: %9.6f секунд\n", finish.tv_sec - start.tv_sec +
                        (finish.tv_usec - start.tv_usec) / 1000000.0);

        if (s == 0)
            break;  /* nanosleep() завершен */

        printf("Оставшееся время: %2ld.%09ld\n", (long) remain.tv_sec, remain.tv_nsec);
        request = remain;  /* Следующий сон будет с оставшимся временем */
    }

    printf("Сон завершен\n");
    exit(EXIT_SUCCESS);
}

/*
### Объяснение:
- **Обработчик сигнала**: Функция `sigintHandler` прерывает выполнение функции `nanosleep()`, если поступает сигнал SIGINT (например, при нажатии Ctrl+C).
- **Основная логика**:
  - Программа использует `nanosleep()` для приостановки выполнения на заданное количество секунд и наносекунд.
  - Если сигнал SIGINT поступает, то `nanosleep()` прерывается и оставшееся время сна сохраняется в переменную `remain`, чтобы продолжить сна с оставшимся временем.
  - Время сна отслеживается с помощью `gettimeofday()`, и выводится фактическое время, которое программа провела в состоянии сна.
  - После завершения сна программа выводит сообщение о завершении.

- **Поддержка прерываний**: Программа прерывает `nanosleep()` с помощью сигнала SIGINT и продолжает сон, если необходимо, с оставшимся временем.
*/
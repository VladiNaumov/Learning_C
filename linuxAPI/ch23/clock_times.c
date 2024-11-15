/* clock_times.c

   Программа для отображения значений CLOCK_REALTIME, CLOCK_MONOTONIC и
   CLOCK_BOOTTIME. Полезна для работы с пространствами времени.
*/

#define _XOPEN_SOURCE 600
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define SECS_IN_DAY (24 * 60 * 60) // Количество секунд в одном дне

/* 
 * Функция для отображения времени и разрешения для указанного clock ID.
 * Параметры:
 *   - clock: идентификатор используемых часов
 *   - name: имя часов (для вывода)
 *   - showRes: если true, отображается разрешение часов
 */
static void displayClock(clockid_t clock, char *name, bool showRes) {
    struct timespec ts;

    // Получение текущего времени для заданного clock ID
    if (clock_gettime(clock, &ts) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    // Вывод текущего времени в секундах и наносекундах
    printf("%-15s: %10ld.%03ld (", name,
            (long) ts.tv_sec, ts.tv_nsec / 1000000);

    // Вывод времени в формате дней, часов, минут и секунд
    long days = ts.tv_sec / SECS_IN_DAY;
    if (days > 0)
        printf("%ld days + ", days);

    printf("%2ldh %2ldm %2lds", (ts.tv_sec % SECS_IN_DAY) / 3600,
            (ts.tv_sec % 3600) / 60, ts.tv_sec % 60);
    printf(")\n");

    // Получение разрешения для заданного clock ID
    if (clock_getres(clock, &ts) == -1) {
        perror("clock_getres");
        exit(EXIT_FAILURE);
    }

    // Вывод разрешения часов, если showRes = true
    if (showRes)
        printf("     resolution: %10ld.%09ld\n",
                (long) ts.tv_sec, ts.tv_nsec);
}

int main(int argc, char *argv[]) {
    bool showRes = argc > 1; // Проверка, был ли передан аргумент для вывода разрешения

    // Отображение времени для разных типов часов
    displayClock(CLOCK_REALTIME, "CLOCK_REALTIME", showRes);
#ifdef CLOCK_TAI
    displayClock(CLOCK_TAI, "CLOCK_TAI", showRes);
#endif
    displayClock(CLOCK_MONOTONIC, "CLOCK_MONOTONIC", showRes);
#ifdef CLOCK_BOOTTIME
    displayClock(CLOCK_BOOTTIME, "CLOCK_BOOTTIME", showRes);
#endif

    exit(EXIT_SUCCESS);
}

/*
### Комментарии:
- `displayClock` получает и выводит текущее время и, если задано, разрешение выбранного таймера (в наносекундах).
- В `main` с помощью аргумента определяется, будет ли программа выводить разрешение для каждого таймера.
*/
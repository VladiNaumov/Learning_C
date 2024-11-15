/* demo_timerfd.c

   Пример использования API timerfd, который создает таймеры, 
   чьи срабатывания можно считывать через файловый дескриптор.

   Программа специфична для Linux. API timerfd поддерживается с ядра версии 2.6.25.
   Библиотечная поддержка предоставляется с glibc версии 2.8.
*/
#include <sys/timerfd.h>
#include <time.h>
#include <stdint.h>                     /* Определение uint64_t */
#include "itimerspec_from_str.h"        /* Объявление itimerspecFromStr() */
#include "tlpi_hdr.h"

int main(int argc, char *argv[]) {
    struct itimerspec ts;
    struct timespec start, now;
    int maxExp, fd, secs, nanosecs;
    uint64_t numExp, totalExp;
    ssize_t s;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s secs[/nsecs][:int-secs[/int-nsecs]] [max-exp]\n", argv[0]);

    itimerspecFromStr(argv[1], &ts);
    maxExp = (argc > 2) ? getInt(argv[2], GN_GT_0, "max-exp") : 1;

    fd = timerfd_create(CLOCK_REALTIME, 0); /* Создаем таймер */
    if (fd == -1)
        errExit("timerfd_create");

    if (timerfd_settime(fd, 0, &ts, NULL) == -1) /* Устанавливаем таймер */
        errExit("timerfd_settime");

    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
        errExit("clock_gettime");

    for (totalExp = 0; totalExp < maxExp;) {
        /* Чтение количества срабатываний таймера, затем вывод 
           времени с момента запуска, количества срабатываний и 
           общего количества срабатываний. */

        s = read(fd, &numExp, sizeof(uint64_t));
        if (s != sizeof(uint64_t))
            errExit("read");

        totalExp += numExp;

        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
            errExit("clock_gettime");

        secs = now.tv_sec - start.tv_sec;
        nanosecs = now.tv_nsec - start.tv_nsec;
        if (nanosecs < 0) {
            secs--;
            nanosecs += 1000000000;
        }

        printf("%d.%03d: expirations read: %llu; total=%llu\n",
                secs, (nanosecs + 500000) / 1000000,
                (unsigned long long) numExp, (unsigned long long) totalExp);
    }

    exit(EXIT_SUCCESS);
}
/*

### Объяснение
1. **Функция timerfd_create()**: Создает файловый дескриптор для таймера, связанного с `CLOCK_REALTIME`.
2. **Функция timerfd_settime()**: Устанавливает параметры для созданного таймера.
3. **Чтение срабатываний**: В цикле программа считывает срабатывания таймера, используя системный вызов `read()`, и отображает информацию о каждом срабатывании.
4. **Отслеживание времени**: Используя `clock_gettime()`, программа вычисляет, сколько времени прошло с момента начала работы таймера.
*/

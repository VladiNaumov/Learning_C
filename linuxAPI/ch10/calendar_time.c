/* Этот код демонстрирует использование функций для работы с календарным временем, включая получение текущего времени и его отображение в различных форматах. */


/* calendar_time.c

   Демонстрация использования функций для работы с календарным временем.

   Программа получает текущее время и отображает его в различных форматах.
*/

#include <locale.h>
#include <time.h>
#include <sys/time.h>
#include "tlpi_hdr.h"

#define SECONDS_IN_TROPICAL_YEAR (365.24219 * 24 * 60 * 60)  // Секунд в тропическом году

int main(int argc, char *argv[])
{
    time_t t;                  // Переменная для хранения времени в секундах с начала эпохи
    struct tm *gmp, *locp;     // Указатели на структуру tm для хранения разбора времени
    struct tm gm, loc;         // Локальные копии структур tm
    struct timeval tv;         // Структура для хранения времени с микросекундами

    // Получаем текущее время
    t = time(NULL);
    printf("Секунд с начала эпохи (1 января 1970 года): %ld", (long) t);
    printf(" (около %6.3f лет)\n", t / SECONDS_IN_TROPICAL_YEAR);

    // Получаем время с микросекундами
    if (gettimeofday(&tv, NULL) == -1)
        errExit("gettimeofday");
    printf("  gettimeofday() вернул %ld сек, %ld микросекунд\n",
            (long) tv.tv_sec, (long) tv.tv_usec);

    // Преобразуем время в UTC
    gmp = gmtime(&t);
    if (gmp == NULL)
        errExit("gmtime");

    gm = *gmp;  // Сохраняем локальную копию структуры, так как gmp может быть изменён
    printf("Разбиение времени gmtime():\n");
    printf("  год=%d месяц=%d день=%d час=%d мин=%d сек=%d ", gm.tm_year,
            gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec);
    printf("день недели=%d день года=%d летнее время=%d\n", gm.tm_wday, gm.tm_yday, gm.tm_isdst);

    /* Переменная окружения TZ влияет на функцию localtime().
       Например:
            TZ=Pacific/Auckland calendar_time
    */

    // Преобразуем время в локальное
    locp = localtime(&t);
    if (locp == NULL)
        errExit("localtime");

    loc = *locp;  // Сохраняем локальную копию структуры

    printf("Разбиение времени localtime():\n");
    printf("  год=%d месяц=%d день=%d час=%d мин=%d сек=%d ",
            loc.tm_year, loc.tm_mon, loc.tm_mday,
            loc.tm_hour, loc.tm_min, loc.tm_sec);
    printf("день недели=%d день года=%d летнее время=%d\n\n",
            loc.tm_wday, loc.tm_yday, loc.tm_isdst);

    // Форматирование времени с помощью asctime() и ctime()
    printf("asctime() формирует gmtime(): %s", asctime(&gm));
    printf("ctime() формирует time():     %s", ctime(&t));

    // Преобразуем время из структур tm обратно в количество секунд с начала эпохи
    printf("mktime() для gmtime():    %ld сек\n", (long) mktime(&gm));
    printf("mktime() для localtime(): %ld сек\n", (long) mktime(&loc));

    exit(EXIT_SUCCESS);
}

/*

### Пояснение
1. **time()** — получает текущее время в секундах с 1 января 1970 года.
2. **gettimeofday()** — получает текущее время в секундах и микросекундах.
3. **gmtime()** — преобразует время `time_t` в структуру `tm` в формате UTC (время по Гринвичу).
4. **localtime()** — преобразует `time_t` в структуру `tm` для локального часового пояса (влияет переменная окружения `TZ`).
5. **asctime()** — форматирует структуру `tm` в строку.
6. **ctime()** — форматирует время `time_t` в строку.
7. **mktime()** — преобразует `tm` обратно в `time_t`, определяя, если нужно, переход на летнее время.

Этот код подходит для работы с временем в различных форматах, как локально, так и в UTC.
*/

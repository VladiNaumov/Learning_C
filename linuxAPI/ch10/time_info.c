/* ch10 Time */

/*
- Временем процесса называется объем времени центрального процессора, использованный 
процессом с момента его создания. В целях учета ядро делит время центрального про-
цессора на следующие два компонента.
- Пользовательское время центрального процессора, представляющее собой время, 
потраченное на выполнение кода в пользовательском режиме. Иногда его называют 
фактическим временем, и оно является временем, когда программе кажется, что она 
имеет доступ к ЦП.
- Системное время центрального процессора, представляющее собой время, потра-
ченное на выполнение кода в режиме ядра. Это время, которое ядро затрачивает на 
выполнение системных вызовов или других задач в интересах программы (например, 
на обслуживание ошибок отсутствия страниц).
Иногда время процесса называют общим временем центрального процессора, потребленным процессом.

$ time ./myprog
real    0m4.84s
user    0m1.030s
sys     0m3.43s

*/

#include <sys/time.h>
#include <time.h>
#include <locale.h>
#include <stdio.h>

int gettimeofday(struct timeval *tv, struct timezone *tz);
/* 
   Получает текущее время с микросекундной точностью.
   `tv` - структура для хранения времени в секундах и микросекундах с 1 января 1970 года.
   `tz` - структура часового пояса (обычно передают NULL).
   Использование:
   struct timeval tv;
   gettimeofday(&tv, NULL);
*/

time_t time(time_t *timep);
/* 
   Возвращает текущее время в секундах с 1 января 1970 года.
   `timep` - указатель для сохранения результата (или NULL, чтобы просто получить значение).
   Использование:
   time_t t = time(NULL);
*/

char *ctime(const time_t *timep);
/*
   Преобразует время (time_t) в строку формата "Thu Nov  4 10:26:24 2024\n".
   Использование:
   time_t t = time(NULL);
   printf("%s", ctime(&t));
*/

struct tm *gmtime(const time_t *timep);
/*
   Преобразует время в структуру `tm`, представляющую время в формате UTC.
   Использование:
   time_t t = time(NULL);
   struct tm *utc = gmtime(&t);
*/

struct tm *localtime(const time_t *timep);
/*
   Преобразует время в структуру `tm`, представляющую локальное время.
   Использование:
   time_t t = time(NULL);
   struct tm *local = localtime(&t);
*/

time_t mktime(struct tm *timeptr);
/*
   Преобразует локальное время (структура `tm`) обратно в `time_t`.
   Использование:
   struct tm local;
   local.tm_year = 123; // 2023 - 1900
   local.tm_mon = 9;    // Октябрь (месяцы с 0)
   local.tm_mday = 4;   // День
   time_t t = mktime(&local);
*/

char *asctime(const struct tm *timeptr);
/*
   Преобразует структуру `tm` в строку формата "Thu Nov  4 10:26:24 2024\n".
   Использование:
   struct tm *local = localtime(&t);
   printf("%s", asctime(local));
*/

size_t strftime(char *outstr, size_t maxsize, const char *format, const struct tm *timeptr);
/*
   Форматирует структуру `tm` в строку согласно указанному формату.
   `format` может включать такие символы, как %Y для года, %m для месяца и т.д.
   Использование:
   struct tm *local = localtime(&t);
   char buf[64];
   strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
*/

char *currTime(const char *format);
/*
   Пользовательская функция, возвращающая текущее время в формате `format`.
   Использование зависит от реализации функции currTime.
*/

char *strptime(const char *str, const char *format, struct tm *timeptr);
/*
   Анализирует строку `str` согласно `format` и заполняет структуру `tm`.
   Использование:
   struct tm tm;
   strptime("2024-11-04", "%Y-%m-%d", &tm);
*/

char *setlocale(int category, const char *locale);
/*
   Устанавливает локаль для программы.
   Использование:
   setlocale(LC_TIME, "en_US.UTF-8");
*/

int settimeofday(const struct timeval *tv, const struct timezone *tz);
/*
   Устанавливает текущее время. Требует прав суперпользователя.
   Использование:
   struct timeval tv;
   tv.tv_sec = 1670000000;
   tv.tv_usec = 0;
   settimeofday(&tv, NULL);
*/

int adjtime(struct timeval *delta, struct timeval *olddelta);
/*
   Медленно изменяет время на величину `delta`, полезно для корректировки времени.
   Использование:
   struct timeval delta;
   delta.tv_sec = 1; // прибавит 1 секунду
   adjtime(&delta, NULL);
*/

clock_t times(struct tms *buf);
/*
   Возвращает процессорное время, затраченное на процесс и его потомков.
   Использование:
   struct tms t;
   clock_t time_spent = times(&t);
*/

clock_t clock(void);
/*
   Возвращает количество тиков, затраченных на выполнение программы.
   Использование:
   clock_t start = clock();
   // ... код ...
   clock_t end = clock();
   double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
*/

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <locale.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>

int main() {
    // Пример для gettimeofday
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("Current time (seconds): %ld, microseconds: %ld\n", tv.tv_sec, tv.tv_usec);

    // Пример для time
    time_t current_time = time(NULL);
    printf("Current time (seconds since Epoch): %ld\n", current_time);

    // Пример для ctime
    printf("Current time (formatted): %s", ctime(&current_time));

    // Пример для gmtime
    struct tm *utc_time = gmtime(&current_time);
    printf("UTC time: %d-%02d-%02d %02d:%02d:%02d\n",
           utc_time->tm_year + 1900, utc_time->tm_mon + 1, utc_time->tm_mday,
           utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec);

    // Пример для localtime
    struct tm *local_time = localtime(&current_time);
    printf("Local time: %d-%02d-%02d %02d:%02d:%02d\n",
           local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
           local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    // Пример для mktime
    local_time->tm_hour += 1;  // Добавляем 1 час
    time_t modified_time = mktime(local_time);
    printf("Modified time (1 hour later): %s", ctime(&modified_time));

    // Пример для asctime
    printf("Local time (asctime): %s", asctime(local_time));

    // Пример для strftime
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
    printf("Formatted local time: %s\n", buffer);

    // Пример для strptime
    struct tm parsed_time;
    memset(&parsed_time, 0, sizeof(parsed_time));
    strptime("2024-11-04 10:26:24", "%Y-%m-%d %H:%M:%S", &parsed_time);
    printf("Parsed time: %d-%02d-%02d %02d:%02d:%02d\n",
           parsed_time.tm_year + 1900, parsed_time.tm_mon + 1, parsed_time.tm_mday,
           parsed_time.tm_hour, parsed_time.tm_min, parsed_time.tm_sec);

    // Пример для setlocale
    setlocale(LC_TIME, "en_US.UTF-8");
    strftime(buffer, sizeof(buffer), "%A, %d %B %Y", local_time);
    printf("Localized date: %s\n", buffer);

    // Пример для settimeofday
    // struct timeval new_tv = { .tv_sec = 1670000000, .tv_usec = 0 };
    // settimeofday(&new_tv, NULL);  // Требуются права суперпользователя

    // Пример для adjtime
    // struct timeval delta = { .tv_sec = 1, .tv_usec = 0 };  // на 1 сек вперед
    // adjtime(&delta, NULL);  // Требуются права суперпользователя

    // Пример для times
    struct tms t;
    clock_t start_time = times(&t);
    sleep(1); // Симулируем работу
    clock_t end_time = times(&t);
    printf("User time: %ld, System time: %ld\n", t.tms_utime, t.tms_stime);
    printf("Elapsed ticks: %ld\n", end_time - start_time);

    // Пример для clock
    clock_t start = clock();
    sleep(1); // Симулируем работу
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU time used: %.2f seconds\n", cpu_time_used);

    return 0;
}

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
#include <sys/times.h>

/* ============ Получение времени ============ */

/*
 * gettimeofday - получает текущее время.
 * tv - структура для хранения времени.
 * tz - структура для хранения информации о временной зоне.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
int gettimeofday(struct timeval *tv, struct timezone *tz);

/*
 * time - возвращает текущее время в формате времени с 1 января 1970.
 * timep - указатель на переменную для записи времени.
 * Возвращает текущее время в секундах с 1 января 1970.
 */
time_t time(time_t *timep);

/*
 * ctime - преобразует время в формате time_t в строку в формате "Tue Jan 1 01:00:00 1970\n".
 * timep - указатель на структуру time_t.
 * Возвращает строку с датой и временем.
 */
char *ctime(const time_t *timep);

/*
 * gmtime - преобразует время в формате time_t в UTC.
 * timep - указатель на структуру time_t.
 * Возвращает указатель на структуру tm с временем в UTC.
 */
struct tm *gmtime(const time_t *timep);

/*
 * localtime - преобразует время в формате time_t в локальное время.
 * timep - указатель на структуру time_t.
 * Возвращает указатель на структуру tm с локальным временем.
 */
struct tm *localtime(const time_t *timep);

/*
 * mktime - преобразует структуру tm в time_t.
 * timeptr - указатель на структуру tm.
 * Возвращает время в формате time_t.
 */
time_t mktime(struct tm *timeptr);

/*
 * asctime - преобразует структуру tm в строку формата "Tue Jan 1 01:00:00 1970\n".
 * timeptr - указатель на структуру tm.
 * Возвращает строку с датой и временем.
 */
char *asctime(const struct tm *timeptr);

/*
 * strftime - форматирует дату и время по указанному формату.
 * outstr - строка для вывода отформатированной даты и времени.
 * maxsize - максимальный размер выходной строки.
 * format - строка формата.
 * timeptr - указатель на структуру tm.
 * Возвращает количество символов, записанных в outstr.
 */
size_t strftime(char *outstr, size_t maxsize, const char *format, const struct tm *timeptr);

/* ============ Установка времени ============ */

/*
 * settimeofday - устанавливает системное время.
 * tv - структура для хранения времени.
 * tz - структура для хранения информации о временной зоне.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
int settimeofday(const struct timeval *tv, const struct timezone *tz);

/*
 * adjtime - корректирует системное время.
 * delta - структура для хранения изменений времени.
 * olddelta - структура для хранения предыдущих изменений.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
int adjtime(struct timeval *delta, struct timeval *olddelta);

/* ============ Время выполнения ============ */

/*
 * times - получает время выполнения процесса.
 * buf - структура для хранения времени.
 * Возвращает количество тиков процессора, использованных процессом.
 */
clock_t times(struct tms *buf);

/*
 * clock - возвращает количество тиков процессора, использованных процессом с момента его запуска.
 * Возвращает количество тиков процессора.
 */
clock_t clock(void);

/* ============ Работа с текущим временем ============ */

/*
 * currTime - возвращает текущее время в строковом формате.
 * format - строка формата.
 * Возвращает строку с текущим временем, отформатированным по указанному формату.
 */
char *currTime(const char *format);

/* ============ Парсинг времени ============ */

/*
 * strptime - парсит строку, содержащую дату и время, по указанному формату.
 * str - строка с датой и временем.
 * format - формат для парсинга строки.
 * timeptr - структура для хранения результата.
 * Возвращает указатель на первую нераспознанную часть строки.
 */
char *strptime(const char *str, const char *format, struct tm *timeptr);

/* ============ Локализация ============ */

/*
 * setlocale - устанавливает текущую локаль.
 * category - категория локали.
 * locale - строка, представляющая новую локаль.
 * Возвращает строку с предыдущей локалью.
 */
char *setlocale(int category, const char *locale);

//* 12 SYSTEM AND PROCESS INFORMATION */

/*
 * uname - получает информацию о системе.
 * utsbuf - структура, куда будет записана информация.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <sys/utsname.h>
int uname(struct utsname *utsbuf);

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

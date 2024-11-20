/* daemon_SIGHUP.c

   Демонстрация использования сигнала SIGHUP для того, чтобы сообщить демону
   о необходимости перечитать конфигурационный файл и переоткрыть файл журнала.

   В версии кода, представленной в книге, функции logOpen(), logClose(), 
   logMessage() и readConfigFile() были упущены ради краткости. Этот файл содержит 
   полный вариант программы, который можно скомпилировать и запустить.
*/

#include <sys/stat.h>
#include <signal.h>
#include "become_daemon.h"
#include "tlpi_hdr.h"
#include <time.h>
#include <stdarg.h>

static const char *LOG_FILE = "/tmp/ds.log";      /* Путь к файлу журнала */
static const char *CONFIG_FILE = "/tmp/ds.conf"; /* Путь к конфигурационному файлу */

static FILE *logfp; /* Поток для работы с файлом журнала */

/**
 * Пишет сообщение в журнал.
 *
 * Сообщения предшествуют временной меткой. 
 * Функция поддерживает переменное количество аргументов, как printf().
 *
 * Параметры:
 * - format: Формат строки (как в printf).
 * - ...: Дополнительные параметры для строки.
 */
static void logMessage(const char *format, ...) {
    va_list argList;
    const char *TIMESTAMP_FMT = "%F %X"; /* Формат временной метки YYYY-MM-DD HH:MM:SS */
#define TS_BUF_SIZE sizeof("YYYY-MM-DD HH:MM:SS") /* Размер буфера для временной метки */
    char timestamp[TS_BUF_SIZE];
    time_t t;
    struct tm *loc;

    t = time(NULL);
    loc = localtime(&t);
    if (loc == NULL || strftime(timestamp, TS_BUF_SIZE, TIMESTAMP_FMT, loc) == 0)
        fprintf(logfp, "???Unknown time????: ");
    else
        fprintf(logfp, "%s: ", timestamp);

    va_start(argList, format);
    vfprintf(logfp, format, argList);
    fprintf(logfp, "\n");
    va_end(argList);
}

/**
 * Открывает файл журнала.
 *
 * Параметры:
 * - logFilename: Имя файла журнала.
 */
static void logOpen(const char *logFilename) {
    mode_t m;

    m = umask(077);
    logfp = fopen(logFilename, "a");
    umask(m);

    if (logfp == NULL) /* Если не удалось открыть файл */
        exit(EXIT_FAILURE);

    setbuf(logfp, NULL); /* Отключаем буферизацию stdio */

    logMessage("Opened log file");
}

/**
 * Закрывает файл журнала.
 */
static void logClose(void) {
    logMessage("Closing log file");
    fclose(logfp);
}

/**
 * (Пере)инициализирует настройки из конфигурационного файла.
 *
 * В реальном приложении конфигурационный файл содержал бы параметры демона. 
 * Здесь мы просто читаем одну строку из файла и записываем её в журнал.
 *
 * Параметры:
 * - configFilename: Имя конфигурационного файла.
 */
static void readConfigFile(const char *configFilename) {
    FILE *configfp;
#define SBUF_SIZE 100
    char str[SBUF_SIZE];

    configfp = fopen(configFilename, "r");
    if (configfp != NULL) { /* Игнорируем несуществующий файл */
        if (fgets(str, SBUF_SIZE, configfp) == NULL)
            str[0] = '\0';
        else
            str[strlen(str) - 1] = '\0'; /* Удаляем завершающий '\n' */
        logMessage("Read config file: %s", str);
        fclose(configfp);
    }
}

static volatile sig_atomic_t hupReceived = 0; /* Устанавливается в ненулевое значение при получении SIGHUP */

/**
 * Обработчик сигнала SIGHUP.
 */
static void sighupHandler(int sig) {
    hupReceived = 1;
}

/**
 * Основная функция программы.
 *
 * Демон инициализируется, обрабатывает сигналы SIGHUP для перечитывания конфигурации
 * и записывает периодические сообщения в журнал.
 *
 * Параметры:
 * - argc: Количество аргументов командной строки.
 * - argv: Массив строковых аргументов.
 *
 * Возвращает:
 * - Завершение программы (обычно бесконечный цикл).
 */
int main(int argc, char *argv[]) {
    const int SLEEP_TIME = 15; /* Время сна между сообщениями (в секундах) */
    int count = 0;             /* Количество завершённых интервалов сна */
    int unslept;               /* Оставшееся время сна */
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sighupHandler;
    if (sigaction(SIGHUP, &sa, NULL) == -1)
        errExit("sigaction");

    if (becomeDaemon(0) == -1)
        errExit("becomeDaemon");

    logOpen(LOG_FILE);
    readConfigFile(CONFIG_FILE);

    unslept = SLEEP_TIME;

    for (;;) {
        unslept = sleep(unslept); /* Возвращает > 0, если сон был прерван */

        if (hupReceived) { /* Если получен SIGHUP */
            hupReceived = 0;
            logClose();
            logOpen(LOG_FILE);
            readConfigFile(CONFIG_FILE);
        }

        if (unslept == 0) { /* Интервал завершён */
            count++;
            logMessage("Main: %d", count);
            unslept = SLEEP_TIME; /* Сбросить интервал */
        }
    }
}

/*
Резюме кода:
Этот демон использует сигнал SIGHUP для переинициализации. При получении SIGHUP он 
закрывает текущий журнал, переоткрывает его, перечитывает конфигурационный файл и 
продолжает работу. Демон записывает сообщения в журнал через регулярные интервалы.
*/

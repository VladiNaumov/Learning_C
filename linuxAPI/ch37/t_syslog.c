/* t_syslog.c

   Пример использования syslog(3) для записи произвольных сообщений в системный журнал.
   Использование описано в функции usageError() ниже.
*/

#include <syslog.h>
#include "tlpi_hdr.h"

/**
 * Выводит информацию о правильном использовании программы и завершает её выполнение.
 *
 * Аргументы:
 * - progName: Имя программы, переданное через argv[0].
 */
static void usageError(const char *progName) {
    fprintf(stderr, "Usage: %s [-p] [-e] [-l level] \"message\"\n", progName);
    fprintf(stderr, "    -p   log PID\n");
    fprintf(stderr, "    -e   log to stderr also\n");
    fprintf(stderr, "    -l   level (g=EMERG; a=ALERT; c=CRIT; e=ERR\n");
    fprintf(stderr, "                w=WARNING; n=NOTICE; i=INFO; d=DEBUG)\n");
    exit(EXIT_FAILURE);
}

/**
 * Основная функция программы. Обрабатывает аргументы командной строки и записывает
 * сообщение в системный журнал с указанным уровнем приоритета.
 *
 * Аргументы:
 * - argc: Количество аргументов командной строки.
 * - argv: Массив аргументов командной строки.
 *
 * Возвращает:
 * - 0 при успешном завершении.
 */
int main(int argc, char *argv[]) {
    int options = 0;         /* Флаги для openlog() */
    int level = LOG_INFO;    /* Уровень сообщения по умолчанию */
    int opt;

    /* Обработка опций командной строки */
    while ((opt = getopt(argc, argv, "l:pe")) != -1) {
        switch (opt) {
        case 'l': /* Указан уровень логирования */
            switch (optarg[0]) {
            case 'a': level = LOG_ALERT;        break;
            case 'c': level = LOG_CRIT;         break;
            case 'e': level = LOG_ERR;          break;
            case 'w': level = LOG_WARNING;      break;
            case 'n': level = LOG_NOTICE;       break;
            case 'i': level = LOG_INFO;         break;
            case 'd': level = LOG_DEBUG;        break;
            default:  cmdLineErr("Invalid level: %c\n", optarg[0]);
            }
            break;

        case 'p': /* Логировать PID процесса */
            options |= LOG_PID;
            break;

#if !defined(__hpux) && !defined(__sun)
        /* Не поддерживается на HP-UX 11 и Solaris 8 */
        case 'e': /* Дублировать вывод в stderr */
            options |= LOG_PERROR;
            break;
#endif

        default:
            fprintf(stderr, "Invalid option\n");
            usageError(argv[0]);
        }
    }

    /* Проверка на наличие сообщения */
    if (argc != optind + 1)
        usageError(argv[0]);

    /* Запись в системный журнал */
    openlog(argv[0], options, LOG_USER); /* Открываем журнал с заданными опциями */
    syslog(LOG_USER | level, "%s", argv[optind]); /* Логируем сообщение */
    closelog(); /* Закрываем журнал */

    exit(EXIT_SUCCESS);
}

/*
Резюме:
Программа демонстрирует использование функции `syslog()` для записи сообщений в системный журнал.
1. Поддерживаются опции:
   - `-p`: Логирование PID процесса.
   - `-e`: Логирование в stderr (если поддерживается).
   - `-l`: Задание уровня приоритета сообщения (например, ALERT, INFO, DEBUG).
2. Сообщение для записи передаётся как последний аргумент командной строки.
3. В зависимости от указанных опций программа настраивает запись в журнал через `openlog()`.
4. Уровни сообщений кодируются буквами (например, 'a' для ALERT).
5. Поддерживается стандарт POSIX для обработки аргументов.
*/

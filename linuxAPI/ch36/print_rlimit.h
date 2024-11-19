/* print_rlimit.h

   Заголовочный файл для print_rlimit.c.
*/

#ifndef PRINT_RLIMIT_H      /* Предотвращение повторного включения */
#define PRINT_RLIMIT_H

/*
 * printRlimit:
 * Выводит мягкое (soft) и жесткое (hard) ограничения для указанного ресурса.
 *
 * Параметры:
 * - msg: сообщение, отображаемое перед значениями ограничений.
 * - resource: идентификатор ресурса (например, RLIMIT_CPU, RLIMIT_NOFILE и т.д.).
 *
 * Возвращаемое значение:
 * - 0, если функция успешно завершена;
 * - -1, если произошла ошибка.
 */
int printRlimit(const char *msg, int resource);

#endif


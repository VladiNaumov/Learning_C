/* become_daemon.h

   Заголовочный файл для реализации become_daemon.c.
*/

#ifndef BECOME_DAEMON_H             /* Предотвращение повторного включения */
#define BECOME_DAEMON_H

/* Маски битов для аргумента 'flags' функции becomeDaemon() */

#define BD_NO_CHDIR           01    /* Не выполнять chdir("/") */
#define BD_NO_CLOSE_FILES     02    /* Не закрывать все открытые файлы */
#define BD_NO_REOPEN_STD_FDS  04    /* Не перенаправлять stdin, stdout и stderr 
                                       в /dev/null */
#define BD_NO_UMASK0         010    /* Не устанавливать umask(0) */

#define BD_MAX_CLOSE  8192          /* Максимальное количество файловых дескрипторов,
                                       которые будут закрыты, если sysconf(_SC_OPEN_MAX)
                                       не определён */

/**
 * Переводит процесс в режим демона.
 *
 * Аргументы:
 * - flags: Управляет поведением функции через маски битов, такие как BD_NO_CHDIR,
 *   BD_NO_CLOSE_FILES и другие.
 *
 * Возвращает:
 * - 0 при успехе.
 * - -1 при ошибке.
 */
int becomeDaemon(int flags);

#endif


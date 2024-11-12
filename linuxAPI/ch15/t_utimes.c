/* t_utimes.c

   Демонстрирует использование функции utimes(): устанавливает время последней 
   модификации файла равным времени последнего доступа, но изменяет микросекундные 
   компоненты этих двух отметок времени.

   Использование: t_utimes file

   См. также t_utime.c.
*/

#include <sys/stat.h>
#include <sys/time.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    struct stat sb;
    if (stat(argv[1], &sb) == -1)       /* Получаем текущее время файла */
        errExit("stat");

    struct timeval tv[2];
    tv[0].tv_sec = sb.st_atime;         /* Секунды для atime остаются без изменений */
    tv[0].tv_usec = 223344;             /* Изменяем микросекунды для atime */
    tv[1].tv_sec = sb.st_atime;         /* Секунды mtime = секундам atime */
    tv[1].tv_usec = 667788;             /* Изменяем микросекунды для mtime */

    if (utimes(argv[1], tv) == -1)
        errExit("utimes");

    exit(EXIT_SUCCESS);
}

/*
Резюме:
Эта программа использует функцию utimes для изменения временных меток доступа (atime) 
и модификации (mtime) файла. Она устанавливает одинаковое значение секунд для atime 
и mtime, но задает разные микросекундные значения для каждой из них. Таким образом, 
проверяется возможность настройки временных меток с точностью до микросекунд.
*/

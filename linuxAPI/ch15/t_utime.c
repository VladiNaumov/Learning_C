/* t_utime.c

   Демонстрирует использование функции utime(): устанавливает время последней
   модификации файла таким же, как время последнего доступа.

   Использование: t_utime file

   См. также t_utimes.c.
*/

#include <sys/stat.h>
#include <utime.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    char *pathname = argv[1];

    struct stat sb;
    if (stat(pathname, &sb) == -1)    /* Получаем текущее время файла */
        errExit("stat");

    struct utimbuf utb;
    utb.actime = sb.st_atime;         /* Оставляем время доступа без изменений */
    utb.modtime = sb.st_atime;        /* Делаем время модификации равным времени доступа */
    if (utime(pathname, &utb) == -1)  /* Обновляем временные метки файла */
        errExit("utime");

    exit(EXIT_SUCCESS);
}

/*
Резюме:
Эта программа использует функцию utime для изменения временной метки модификации (mtime) 
файла, устанавливая её равной времени последнего доступа (atime). Это может использоваться 
для синхронизации временных меток доступа и модификации, оставляя значение времени 
последнего доступа без изменений.
*/

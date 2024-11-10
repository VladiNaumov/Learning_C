/* list_files_readdir_r.c

   Демонстрирует использование opendir() и readdir_r() для вывода списка файлов
   в каталоге.

   Использование: list_files_readdir_r [dir...]

   Обходит каждый каталог, указанный в командной строке (текущий каталог,
   если ничего не указано), и отображает список содержащихся в нем файлов.

   См. также list_files_readdir.c.
*/

#if defined(__APPLE__)
        /* Для Darwin требуется этот заголовок перед <dirent.h> */
#include <sys/types.h>
#endif
#include <limits.h>
#include <dirent.h>
#include <stddef.h>
#include "tlpi_hdr.h"

/* Выводит список всех файлов в каталоге 'dirpath' */
static void listFiles(const char *dirpath)
{
    DIR *dirp;
    Boolean isCurrent;          /* True, если 'dirpath' — это "." */
    struct dirent *result, *entryp;
    int nameMax;

    isCurrent = strcmp(dirpath, ".") == 0;

    /* В Linux NAME_MAX определен в <limits.h>. Однако этот предел
       может варьироваться для разных файловых систем, поэтому для
       получения истинного предела следует использовать pathconf(). */

    nameMax = pathconf(dirpath, _PC_NAME_MAX);
    if (nameMax == -1)          /* Неопределённый или ошибка */
        nameMax = 255;          /* Предполагаем значение */

    entryp = malloc(offsetof(struct dirent, d_name) + nameMax + 1);
    if (entryp == NULL)
        errExit("malloc");

    /* Открываем каталог - при ошибке выводим сообщение и выходим */

    dirp = opendir(dirpath);
    if (dirp == NULL) {
        errMsg("opendir failed on '%s'", dirpath);
        free(entryp);
        return;
    }

    /* Читаем каждую запись в каталоге */

    for (;;) {
        errno = readdir_r(dirp, entryp, &result);
        if (errno != 0)
            errExit("readdir_r");

        if (result == NULL)     /* Конец каталога */
            break;

        /* Пропускаем . и .. */

        if (strcmp(entryp->d_name, ".") == 0 ||
                strcmp(entryp->d_name, "..") == 0)
            continue;

        /* Выводим каталог + имя файла */

        if (!isCurrent) printf("%s/", dirpath);
        printf("%s\n", entryp->d_name);
    }

    if (closedir(dirp) == -1)
        errMsg("closedir");
}

int main(int argc, char *argv[])
{
    if (argc == 1)              /* Без аргументов - используем текущий каталог */
        listFiles(".");
    else
        for (argv++; *argv; argv++)
            listFiles(*argv);
    exit(EXIT_SUCCESS);
}

/*

### Описание кода

1. **listFiles()** — Функция выводит список всех файлов в заданном каталоге. Для каждого файла:
   - Открывает каталог с помощью `opendir`.
   - Использует `readdir_r` для безопасного чтения записей.
   - Пропускает `.` и `..`, чтобы не зациклиться.
   - Выводит путь и имя каждого файла.

2. **main()** — Проверяет, был ли указан каталог в командной строке:
   - Если нет, выводит содержимое текущего каталога (`"."`).
   - Если да, обрабатывает каждый каталог, переданный в аргументах.

### Примечание

Функция `readdir_r` может быть устаревшей и заменяется на `readdir`, так как `readdir` является потокобезопасной для стандартных случаев.
*/
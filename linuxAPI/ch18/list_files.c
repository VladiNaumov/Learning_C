/* list_files.c

   Демонстрирует использование opendir() и связанных функций для перечисления
   файлов в директории.

   Обходит каждую директорию, указанную в командной строке (или текущую директорию,
   если аргументы не заданы) и выводит список содержащихся файлов.

   Использование: list_files [dir...]
*/

#if defined(__APPLE__)
    /* Darwin требует этот заголовок перед включением <dirent.h> */
#include <sys/types.h>
#endif

#include <dirent.h>
#include "tlpi_hdr.h"

/* Выводит список всех файлов в директории 'dirpath' */
static void listFiles(const char *dirpath)
{
    DIR *dirp;
    struct dirent *dp;
    Boolean isCurrent;  /* True, если 'dirpath' — это "." */

    isCurrent = strcmp(dirpath, ".") == 0;

    dirp = opendir(dirpath);
    if (dirp == NULL) {
        errMsg("opendir failed on '%s'", dirpath);
        return;
    }

    /* Для каждой записи в этой директории печатаем директорию и имя файла */
    for (;;) {
        errno = 0;  // Чтобы различать ошибки и конец директории
        dp = readdir(dirp);
        if (dp == NULL)
            break;

        // Пропускаем . и ..
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        if (!isCurrent)
            printf("%s/", dirpath);  // Добавляем путь к файлу, если это не текущая директория
        printf("%s\n", dp->d_name);
    }

    if (errno != 0)
        errExit("readdir");

    if (closedir(dirp) == -1)
        errMsg("closedir");
}

int main(int argc, char *argv[])
{
    // Проверяем, если указан флаг помощи
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [dir-path...]\n", argv[0]);

    // Если аргументов нет, используем текущую директорию
    if (argc == 1)
        listFiles(".");
    else
        for (argv++; *argv; argv++)
            listFiles(*argv);

    exit(EXIT_SUCCESS);
}

/* view_symlink.c

   Демонстрирует использование readlink() и realpath() для чтения и отображения
   содержимого символической ссылки.
*/

#include <sys/stat.h>
#include <limits.h>             /* Для определения PATH_MAX */
#include "tlpi_hdr.h"

#define BUF_SIZE PATH_MAX

int main(int argc, char *argv[])
{
    struct stat statbuf;
    char buf[BUF_SIZE];
    ssize_t numBytes;

    // Проверка аргументов командной строки
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    /* Используем lstat() для проверки, является ли указанный путь
       символической ссылкой. Альтернативно, можно было бы проверить,
       завершилась ли readlink() с ошибкой EINVAL. */

    if (lstat(argv[1], &statbuf) == -1)
        errExit("lstat");

    // Проверяем, является ли файл символической ссылкой
    if (!S_ISLNK(statbuf.st_mode))
        fatal("%s is not a symbolic link", argv[1]);

    // Чтение символической ссылки
    numBytes = readlink(argv[1], buf, BUF_SIZE - 1);
    if (numBytes == -1)
        errExit("readlink");

    buf[numBytes] = '\0';  // Добавляем завершающий нулевой байт
    printf("readlink: %s --> %s\n", argv[1], buf);

    // Получение абсолютного пути для символической ссылки
    if (realpath(argv[1], buf) == NULL)
        errExit("realpath");
    printf("realpath: %s --> %s\n", argv[1], buf);

    exit(EXIT_SUCCESS);
}

/*

### Объяснение кода

1. **lstat()** — Проверяет, является ли указанный путь символической ссылкой. Если это не так, программа завершится с сообщением об ошибке.
2. **readlink()** — Считывает содержимое символической ссылки и сохраняет его в буфере.
3. **realpath()** — Преобразует символическую ссылку в абсолютный путь.

Этот код выводит две строки:
- `readlink`: показывает путь, на который указывает символическая ссылка.
- `realpath`: показывает абсолютный путь к файлу, на который ссылается ссылка.

*/
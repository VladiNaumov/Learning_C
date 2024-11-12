
/*
Программа t_statfs.c

Демонстрирует использование функции statfs() для получения информации о
смонтированной файловой системе.

Программа специфична для Linux.

См. также t_statvfs.c.
*/

#include <sys/statfs.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s path\n", argv[0]);  // Проверка аргументов командной строки

    struct statfs sfs;
    if (statfs(argv[1], &sfs) == -1)
        errExit("statfs");  // Вызов statfs() для получения информации о файловой системе

    // Вывод информации о файловой системе
    printf("Тип файловой системы:               %#lx\n",
            (unsigned long) sfs.f_type);
    printf("Оптимальный размер блока ввода-вывода: %lu\n",
            (unsigned long) sfs.f_bsize);
    printf("Общее количество блоков данных:     %lu\n",
            (unsigned long) sfs.f_blocks);
    printf("Свободные блоки данных:             %lu\n",
            (unsigned long) sfs.f_bfree);
    printf("Свободные блоки для непользователей суперпользователя:  %lu\n",
            (unsigned long) sfs.f_bavail);
    printf("Общее количество i-узлов:           %lu\n",
            (unsigned long) sfs.f_files);
    printf("Идентификатор файловой системы:     %#x, %#x\n",
            (unsigned) sfs.f_fsid.__val[0], (unsigned) sfs.f_fsid.__val[1]);
    printf("Свободные i-узлы:                   %lu\n",
            (unsigned long) sfs.f_ffree);
    printf("Максимальная длина имени файла:     %lu\n",
            (unsigned long) sfs.f_namelen);

    exit(EXIT_SUCCESS);
}

/*
### Пояснение
- Функция `statfs()` используется для получения структурированной информации о файловой системе, смонтированной по указанному пути.
- Параметры, такие как тип файловой системы (`f_type`), 
размер блока ввода-вывода (`f_bsize`), 
общее количество блоков данных (`f_blocks`), и другие, печатаются для анализа состояния и особенностей файловой системы.

*/
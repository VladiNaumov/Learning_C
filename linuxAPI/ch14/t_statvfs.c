/*
Программа t_statvfs.c

Демонстрирует использование функции statvfs() для получения информации о
смонтированной файловой системе.

См. также t_statfs.c.
*/

#include <sys/statvfs.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s path\n", argv[0]);  // Проверка аргументов командной строки

    struct statvfs sb;
    if (statvfs(argv[1], &sb) == -1)
        errExit("statvfs");  // Вызов statvfs() для получения информации о файловой системе

    // Вывод информации о файловой системе
    printf("Размер блока                     %lu\n", sb.f_bsize);
    printf("Базовый размер блока             %lu\n", sb.f_frsize);
    printf("Общее количество блоков          %lu\n", (unsigned long) sb.f_blocks);
    printf("Свободные блоки для привилегированного процесса   %lu\n", (unsigned long) sb.f_bfree);
    printf("Свободные блоки для непривилегированного процесса %lu\n", (unsigned long) sb.f_bavail);
    printf("Общее количество i-узлов         %lu\n", (unsigned long) sb.f_files);
    printf("Свободные i-узлы для привилегированного процесса   %lu\n", (unsigned long) sb.f_ffree);
    printf("Свободные i-узлы для непривилегированного процесса %lu\n", (unsigned long) sb.f_favail);
    printf("Идентификатор файловой системы   %#lx\n", sb.f_fsid);
    printf("Флаги                             %#lx\n", sb.f_flag);
    printf("Максимальная длина имени файла   %lu\n", sb.f_namemax);

    exit(EXIT_SUCCESS);
}

/*

### Пояснение
- `statvfs()` получает информацию о файловой системе в структуру `statvfs`, включая такие параметры, как размер блока (`f_bsize`), 
базовый размер блока (`f_frsize`), общее количество блоков (`f_blocks`), количество свободных блоков для разных процессов, а также общее и свободное количество i-узлов.
- `f_flag` содержит флаги, указывающие на особенности файловой системы (например, возможность монтирования только для чтения).
*/
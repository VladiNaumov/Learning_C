/* view_cap_xattr.c

   Отображает содержимое расширенного атрибута "security.capability" файла.
   Этот атрибут используется для хранения привилегий, связанных с файлом.
*/

#include <sys/xattr.h>
#include <sys/capability.h>
#include <linux/capability.h>   /* Определяет 'struct vfs_ns_cap_data' и
                                   VFS_CAP_* константы */
#include "tlpi_hdr.h"

/**
 * Основная функция программы. Проверяет расширенный атрибут "security.capability"
 * файла и отображает его содержимое.
 *
 * Аргументы:
 * - argc: Количество аргументов командной строки.
 * - argv: Массив аргументов командной строки.
 *
 * Возвращает:
 * - Завершает выполнение программы с кодом успеха или ошибки.
 */
int main(int argc, char *argv[]) {
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s <file>\n", argv[0]);

    struct vfs_ns_cap_data cap_data;
    ssize_t valueLen = getxattr(argv[1], "security.capability",
                        (char *) &cap_data, sizeof(cap_data));
    if (valueLen == -1) {
        if (errno == ENODATA)
            fatal("\"%s\" has no \"security.capability\" attribute", argv[1]);
        else
            errExit("getxattr");
    }

    printf("Capability version: %d",
            cap_data.magic_etc >> VFS_CAP_REVISION_SHIFT);

    /* Проверяем, является ли версия атрибута третьей. */
    if ((cap_data.magic_etc & VFS_CAP_REVISION_MASK) == VFS_CAP_REVISION_3)
        printf("   [root ID = %u]", cap_data.rootid);

    printf("\n");

    /* Размер возвращённого значения зависит от версии расширенного атрибута. */
    printf("Length of returned value = %zd\n", valueLen);

    /* Отображение возможностей файла */
    printf("    Effective bit:   %d\n",
            cap_data.magic_etc & VFS_CAP_FLAGS_EFFECTIVE);
    printf("    Permitted set:   %08x %08x\n",
            cap_data.data[1].permitted, cap_data.data[0].permitted);
    printf("    Inheritable set: %08x %08x\n",
            cap_data.data[1].inheritable, cap_data.data[0].inheritable);

    exit(EXIT_SUCCESS);
}

/*
Резюме:
1. Программа читает расширенный атрибут "security.capability" файла.
2. Используется системный вызов `getxattr()` для извлечения данных.
3. Выводится версия привилегий, длина возвращённого значения, а также
   установленные флаги: Effective, Permitted и Inheritable.
4. Код завершает работу с кодом успеха или сообщает об ошибке, если
   атрибут отсутствует или произошёл сбой.
*/


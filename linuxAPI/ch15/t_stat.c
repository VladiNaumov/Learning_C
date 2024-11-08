/* t_stat.c

   Программа, которая отображает информацию, возвращаемую системными вызовами stat()/lstat().
   
   Использование: t_stat [-l] file

   Опция '-l' указывает, что следует использовать lstat() вместо stat().
*/

#include <sys/sysmacros.h>
#if defined(_AIX)
#define _BSD
#endif
#if defined(__sgi) || defined(__sun)            /* Некоторые системы требуют этого */
#include <sys/mkdev.h>                          /* Для получения major() и minor() */
#endif
#if defined(__hpux)                             /* Другие системы требуют этого */
#include <sys/mknod.h>
#endif
#include <sys/stat.h>
#include <time.h>
#include "file_perms.h"
#include "tlpi_hdr.h"

static void
displayStatInfo(const struct stat *sb)
{
    printf("Тип файла:                ");

    switch (sb->st_mode & S_IFMT) {
    case S_IFREG:  printf("обычный файл\n");            break;
    case S_IFDIR:  printf("каталог\n");                 break;
    case S_IFCHR:  printf("символьное устройство\n");    break;
    case S_IFBLK:  printf("блочное устройство\n");      break;
    case S_IFLNK:  printf("символическая (мягкая) ссылка\n"); break;
    case S_IFIFO:  printf("FIFO или канал\n");         break;
    case S_IFSOCK: printf("сокет\n");                   break;
    default:       printf("неизвестный тип файла?\n"); break;
    }

    printf("Устройство, содержащее i-узел: major=%ld   minor=%ld\n",
                (long) major(sb->st_dev), (long) minor(sb->st_dev));

    printf("Номер i-узла:            %ld\n", (long) sb->st_ino);

    printf("Режим:                    %lo (%s)\n",
            (unsigned long) sb->st_mode, filePermStr(sb->st_mode, 0));

    if (sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX))
        printf("    установлены специальные биты: %s%s%s\n",
                (sb->st_mode & S_ISUID) ? "set-UID " : "",
                (sb->st_mode & S_ISGID) ? "set-GID " : "",
                (sb->st_mode & S_ISVTX) ? "sticky " : "");

    printf("Число (жестких) ссылок:   %ld\n", (long) sb->st_nlink);

    printf("Владельцы:                UID=%ld   GID=%ld\n",
            (long) sb->st_uid, (long) sb->st_gid);

    if (S_ISCHR(sb->st_mode) || S_ISBLK(sb->st_mode))
        printf("Номер устройства (st_rdev):  major=%ld; minor=%ld\n",
                (long) major(sb->st_rdev), (long) minor(sb->st_rdev));

    printf("Размер файла:             %lld байт\n", (long long) sb->st_size);
    printf("Оптимальный размер блока ввода-вывода:   %ld байт\n", (long) sb->st_blksize);
    printf("Количество блоков по 512 байт:    %lld\n", (long long) sb->st_blocks);

    printf("Последний доступ к файлу:         %s", ctime(&sb->st_atime));
    printf("Последнее изменение файла:       %s", ctime(&sb->st_mtime));
    printf("Последнее изменение статуса:    %s", ctime(&sb->st_ctime));
}

int
main(int argc, char *argv[])
{
    struct stat sb;
    Boolean statLink;           /* True, если указан "-l" (т.е. используем lstat) */
    int fname;                  /* Позиция аргумента с именем файла в argv[] */

    statLink = (argc > 1) && strcmp(argv[1], "-l") == 0;
                                /* Простое разбор команды для "-l" */
    fname = statLink ? 2 : 1;

    if (fname >= argc || (argc > 1 && strcmp(argv[1], "--help") == 0))
        usageErr("%s [-l] file\n"
                 "        -l = использовать lstat() вместо stat()\n", argv[0]);

    if (statLink) {
        if (lstat(argv[fname], &sb) == -1)
            errExit("lstat");
    } else {
        if (stat(argv[fname], &sb) == -1)
            errExit("stat");
    }

    displayStatInfo(&sb);

    exit(EXIT_SUCCESS);
}

/*
Резюме:
Эта программа выводит информацию о файле или символической ссылке, используя системные вызовы stat() или lstat().
При использовании флага '-l' будет использован вызов lstat(), который работает с символическими ссылками и их метаданными.
Программа выводит информацию о типе файла, его правах доступа, владельцах, размере, а также о времени последнего доступа и модификации.
*/

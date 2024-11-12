/* nftw_dir_tree.c

   Демонстрирует использование функции nftw(3). Проходит по дереву
   каталогов, заданному в командной строке (или по текущему рабочему каталогу,
   если каталог не указан), и отображает иерархию файлов с отступами.
   Для каждого файла выводится:

      * буква, указывающая тип файла (как в "ls -l"), полученная с помощью stat(2);
      * строка, указывающая тип файла, предоставленная nftw();
      * номер i-узла файла.
*/

#if defined(__sun)
#define _XOPEN_SOURCE 500       /* Для Solaris 8 */
#else
#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 600
#define _XOPEN_SOURCE 600       /* Для функций nftw() и S_IFSOCK */
#endif
#endif
#include <ftw.h>
#include "tlpi_hdr.h"

/* Сообщение об ошибке использования программы */
static void usageError(const char *progName, const char *msg)
{
    if (msg != NULL)
        fprintf(stderr, "%s\n", msg);
    fprintf(stderr, "Использование: %s [-d] [-m] [-p] [directory-path]\n", progName);
    fprintf(stderr, "\t-d Использовать флаг FTW_DEPTH\n");
    fprintf(stderr, "\t-m Использовать флаг FTW_MOUNT\n");
    fprintf(stderr, "\t-p Использовать флаг FTW_PHYS\n");
    exit(EXIT_FAILURE);
}

/* Функция, вызываемая nftw() для каждого файла */
static int dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    if (type == FTW_NS) {
        printf("?");  /* Не удалось выполнить stat() для файла */
    } else {
        switch (sbuf->st_mode & S_IFMT) {
        case S_IFREG:  printf("-"); break; /* Обычный файл */
        case S_IFDIR:  printf("d"); break; /* Каталог */
        case S_IFCHR:  printf("c"); break; /* Символьное устройство */
        case S_IFBLK:  printf("b"); break; /* Блочное устройство */
        case S_IFLNK:  printf("l"); break; /* Символьная ссылка */
        case S_IFIFO:  printf("p"); break; /* Канал FIFO */
        case S_IFSOCK: printf("s"); break; /* Сокет */
        default:       printf("?"); break; /* Неожиданный тип файла */
        }
    }

    printf(" %s  ", (type == FTW_D)  ? "D  " : (type == FTW_DNR) ? "DNR" :
            (type == FTW_DP) ? "DP " : (type == FTW_F)   ? "F  " :
            (type == FTW_SL) ? "SL " : (type == FTW_SLN) ? "SLN" :
            (type == FTW_NS) ? "NS " : "  ");

    if (type != FTW_NS)
        printf("%7ld ", (long) sbuf->st_ino);  /* Вывод номера i-узла */
    else
        printf("        ");

    printf(" %*s", 4 * ftwb->level, "");        /* Подходящий отступ */
    printf("%s\n",  &pathname[ftwb->base]);     /* Вывод имени файла */
    return 0;                                   /* Продолжаем обход */
}

int main(int argc, char *argv[])
{
    int flags = 0;
    int opt;

    /* Обработка опций командной строки */
    while ((opt = getopt(argc, argv, "dmp")) != -1) {
        switch (opt) {
        case 'd': flags |= FTW_DEPTH;   break;  /* Проход в глубину */
        case 'm': flags |= FTW_MOUNT;   break;  /* Только одна файловая система */
        case 'p': flags |= FTW_PHYS;    break;  /* Игнорировать символьные ссылки */
        default:  usageError(argv[0], NULL);
        }
    }

    if (argc > optind + 1)
        usageError(argv[0], NULL);

    /* Запуск функции nftw для обхода каталога */
    if (nftw((argc > optind) ? argv[optind] : ".", dirTree, 10, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

/*

### Описание кода

1. **dirTree()** — Функция, вызываемая `nftw` для каждого файла/каталога. Она выводит:
   - Букву, указывающую тип файла (например, `d` для каталога, `-` для обычного файла).
   - Тип файла, предоставленный `nftw` (например, `D` для каталога).
   - Номер i-узла.
2. **nftw()** — Функция рекурсивно обходит дерево каталогов, вызывая `dirTree` для каждого файла или каталога.
3. **Опции**:
   - `-d` для обхода в глубину.
   - `-m` для работы только с одной файловой системой.
   - `-p` для игнорирования символических ссылок.
*/
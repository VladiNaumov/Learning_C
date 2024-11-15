/* acct_view.c

   Вывод содержимого файла учета процессов.
*/

#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/acct.h>
#include <limits.h>
#include "ugid_functions.h"             /* Определение функции userNameFromId() */
#include "tlpi_hdr.h"

#define TIME_BUF_SIZE 100

/* Преобразование значения типа comp_t в long long */
static long long comptToLL(comp_t ct) {
    const int EXP_SIZE = 3;             /* 3-битный показатель степени (основание 8) */
    const int MANTISSA_SIZE = 13;       /* 13-битная мантисса */
    const int MANTISSA_MASK = (1 << MANTISSA_SIZE) - 1;
    long long mantissa, exp;

    mantissa = ct & MANTISSA_MASK;
    exp = (ct >> MANTISSA_SIZE) & ((1 << EXP_SIZE) - 1);
    return mantissa << (exp * 3);       /* Основание 8 = сдвиг влево на 3 бита */
}

int main(int argc, char *argv[]) {
    int acctFile;
    struct acct ac;
    ssize_t numRead;
    char *s;
    char timeBuf[TIME_BUF_SIZE];
    struct tm *loc;
    time_t t;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    /* Открытие файла учета процессов */
    acctFile = open(argv[1], O_RDONLY);
    if (acctFile == -1)
        errExit("open");

    /* Заголовок таблицы */
    printf("command  flags   term.  user     "
           "start time            CPU   elapsed\n");
    printf("                status           "
           "                      time    time\n");

    /* Чтение и обработка записей из файла */
    while ((numRead = read(acctFile, &ac, sizeof(struct acct))) > 0) {
        if (numRead != sizeof(struct acct))
            fatal("partial read"); // Ошибка: неполное чтение

        /* Имя команды */
        printf("%-8.8s  ", ac.ac_comm);

        /* Флаги процесса */
        printf("%c", (ac.ac_flag & AFORK) ? 'F' : '-');
        printf("%c", (ac.ac_flag & ASU)   ? 'S' : '-');

        /* Дополнительные флаги (если поддерживаются) */
#ifdef AXSIG
        printf("%c", (ac.ac_flag & AXSIG) ? 'X' : '-');
#else
        printf(" ");
#endif
#ifdef ACORE
        printf("%c", (ac.ac_flag & ACORE) ? 'C' : '-');
#else
        printf(" ");
#endif

        /* Код завершения */
#ifdef __linux__
        printf(" %#6lx   ", (unsigned long) ac.ac_exitcode);
#else
#if ! defined(__FreeBSD__) && ! defined(__NetBSD__) && ! defined(__APPLE__)
        printf(" %#6lx   ", (unsigned long) ac.ac_stat);
#else
        printf("          ");
#endif
#endif

        /* Имя пользователя */
        s = userNameFromId(ac.ac_uid);
        printf("%-8.8s ", (s == NULL) ? "???" : s);

        /* Время начала процесса */
        t = ac.ac_btime;
        loc = localtime(&t);
        if (loc == NULL) {
            printf("???Unknown time???  ");
        } else {
            strftime(timeBuf, TIME_BUF_SIZE, "%Y-%m-%d %T ", loc);
            printf("%s ", timeBuf);
        }

        /* Время работы процесса */
        printf("%5.2f %7.2f ",
               (double) (comptToLL(ac.ac_utime) + comptToLL(ac.ac_stime)) / sysconf(_SC_CLK_TCK),
               (double) comptToLL(ac.ac_etime) / sysconf(_SC_CLK_TCK));
        printf("\n");
    }

    if (numRead == -1)
        errExit("read");

    exit(EXIT_SUCCESS);
}

/*
Резюме:
- Программа читает файл учета процессов (обычно `/var/account/pacct`).
- Каждая запись содержит информацию о завершившемся процессе:
  - Имя команды, флаги, пользователь, время старта, использование CPU и общее время выполнения.
- Функция `comptToLL()` преобразует значения в формате `comp_t` (используемом для времени в файле учета).
- Программа демонстрирует работу с низкоуровневыми API Linux для анализа процесса.
*/

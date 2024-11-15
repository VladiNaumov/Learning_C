/* acct_v3_view.c

   Отображение содержимого файла учёта процессов версии 3, специфичного для Linux.

   Программа предназначена для анализа и отображения информации учёта процессов
   из файла, созданного системой Linux 2.6 (или более поздней версии) с включенной
   опцией CONFIG_BSD_PROCESS_ACCT_V3.

   Формат учёта версии 3 поддерживается начиная с ядра 2.6.8.

   Зависимости при компиляции:
   - Требуется "ugid_functions.h" для функций userNameFromId() и groupNameFromId().
   - Требуется "tlpi_hdr.h" для обработки ошибок и утилит.

   Автор: модифицировано для учебных целей.
*/

#include <fcntl.h>           // Для работы с файловыми дескрипторами
#include <time.h>            // Для работы с временем
#include <sys/stat.h>        // Для работы с файлами
#include <sys/acct.h>        // Структуры для учёта процессов
#include <limits.h>          // Ограничения, такие как PATH_MAX
#include "ugid_functions.h"  // Преобразование UID и GID в имена
#include "tlpi_hdr.h"        // Общие утилиты и обработка ошибок

#define TIME_BUF_SIZE 100    // Размер буфера для форматированной даты/времени

// Определяем структуру acct_v3, если она недоступна
#ifndef GLIBC_DEFINES_ACCT_V3
struct acct_v3 {
    char        ac_flag;      // Флаги учёта
    char        ac_version;   // Номер версии
    u_int16_t   ac_tty;       // Терминал, связанный с процессом
    u_int32_t   ac_exitcode;  // Код завершения процесса
    u_int32_t   ac_uid;       // Идентификатор пользователя
    u_int32_t   ac_gid;       // Идентификатор группы
    u_int32_t   ac_pid;       // Идентификатор процесса
    u_int32_t   ac_ppid;      // Идентификатор родительского процесса
    u_int32_t   ac_btime;     // Время запуска процесса
    float       ac_etime;     // Общее время выполнения
    comp_t      ac_utime;     // Пользовательское время CPU
    comp_t      ac_stime;     // Системное время CPU
    comp_t      ac_mem;       // Использование памяти
    comp_t      ac_io;        // Ввод-вывод
    comp_t      ac_rw;        // Чтение-запись
    comp_t      ac_minflt;    // Незначительные ошибки страниц
    comp_t      ac_majflt;    // Значительные ошибки страниц
    comp_t      ac_swaps;     // Количество свопов
    char        ac_comm[ACCT_COMM]; // Имя команды
};
#endif

// Функция преобразует значение типа comp_t в long long
static long long comptToLL(comp_t ct) {
    const int EXP_SIZE = 3;             // Размер экспоненты (3 бита, основание 8)
    const int MANTISSA_SIZE = 13;       // Мантисса (13 бит)
    const int MANTISSA_MASK = (1 << MANTISSA_SIZE) - 1;
    long long mantissa, exp;

    mantissa = ct & MANTISSA_MASK;      // Извлекаем мантиссу
    exp = (ct >> MANTISSA_SIZE) & ((1 << EXP_SIZE) - 1); // Извлекаем экспоненту
    return mantissa << (exp * 3);       // Преобразуем в значение
}

// Главная функция программы
int main(int argc, char *argv[]) {
    // Проверка аргументов командной строки
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("Usage: %s file\n", argv[0]);

    // Открываем файл учёта
    int acctFile = open(argv[1], O_RDONLY);
    if (acctFile == -1)
        errExit("Failed to open file");

    // Вывод заголовков таблицы
    printf("ver. command    flags   term.   PID   PPID  user     group"
           "      start date+time     CPU   elapsed\n");
    printf("                       status                             "
           "                          time    time\n");

    // Чтение и обработка записей из файла учёта
    ssize_t numRead;
    struct acct_v3 ac;
    while ((numRead = read(acctFile, &ac, sizeof(struct acct_v3))) > 0) {
        if (numRead != sizeof(struct acct_v3))
            fatal("Partial read");

        // Вывод версии и имени команды
        printf("%1d    ", (int) ac.ac_version);
        printf("%-8.8s   ", ac.ac_comm);

        // Вывод флагов учёта
        printf("%c", (ac.ac_flag & AFORK) ? 'F' : '-');
        printf("%c", (ac.ac_flag & ASU)   ? 'S' : '-');
        printf("%c", (ac.ac_flag & AXSIG) ? 'X' : '-');
        printf("%c", (ac.ac_flag & ACORE) ? 'C' : '-');

        // Вывод кода завершения, PID и PPID
        printf(" %#6lx   ", (unsigned long) ac.ac_exitcode);
        printf(" %5ld %5ld  ", (long) ac.ac_pid, (long) ac.ac_ppid);

        // Преобразование UID и GID в имена
        char *s;
        s = userNameFromId(ac.ac_uid);
        printf("%-8.8s ", (s == NULL) ? "???" : s);

        s = groupNameFromId(ac.ac_gid);
        printf("%-8.8s ", (s == NULL) ? "???" : s);

        // Преобразование времени запуска процесса
        time_t t = ac.ac_btime;
        struct tm *loc = localtime(&t);
        if (loc == NULL) {
            printf("???Unknown time???  ");
        } else {
            char timeBuf[TIME_BUF_SIZE];
            strftime(timeBuf, TIME_BUF_SIZE, "%Y-%m-%d %T ", loc);
            printf("%s ", timeBuf);
        }

        // Вывод времени CPU и общего времени выполнения
        printf("%5.2f %7.2f ", 
               (double) (comptToLL(ac.ac_utime) + comptToLL(ac.ac_stime)) / sysconf(_SC_CLK_TCK),
               ac.ac_etime / sysconf(_SC_CLK_TCK));
        printf("\n");
    }

    // Обработка ошибок чтения
    if (numRead == -1)
        errExit("Error reading file");

    // Завершение программы
    exit(EXIT_SUCCESS);
}

/* Резюме:
   Эта программа читает файл учёта процессов версии 3, созданный системой Linux,
   и отображает информацию о процессах в табличном формате. Она включает данные
   о времени выполнения, идентификаторах пользователей, группах и процессах, а
   также информацию о коде завершения и флагах процесса. */

```c
/*
Программа t_mount.c

Демонстрирует использование функции mount() для создания точки монтирования.

Примеры использования:
        t_mount -t ext3 /dev/sda12 /testfs
        t_mount -t ext2 -f r -o nogrpid /dev/sda9 /mydir ext2
            (Флаг "r" указывает, что файловая система будет смонтирована
            только для чтения.)

Программа специфична для Linux.
*/

#include <sys/mount.h>
#include "tlpi_hdr.h"

#ifndef MS_DIRSYNC      /* Может быть не определено в старых версиях заголовков glibc */
#define MS_DIRSYNC 128
#endif

#ifndef MS_BIND         /* Может быть не определено в старых версиях заголовков glibc */
#define MS_BIND 4096
#endif

#ifndef MS_MOVE         /* Может быть не определено в старых версиях заголовков glibc */
#define MS_MOVE 8192
#endif

#ifndef MS_REC          /* Может быть не определено в старых версиях заголовков glibc */
#define MS_REC 16384
#endif

#ifndef MS_UNBINDABLE   /* Может быть не определено в старых версиях заголовков glibc */
#define MS_UNBINDABLE (1<<17) /* изменить на "unbindable" */
#endif

#ifndef MS_PRIVATE      /* Может быть не определено в старых версиях заголовков glibc */
#define MS_PRIVATE (1<<18) /* изменить на "private" */
#endif

#ifndef MS_SLAVE        /* Может быть не определено в старых версиях заголовков glibc */
#define MS_SLAVE (1<<19) /* изменить на "slave" */
#endif

#ifndef MS_SHARED       /* Может быть не определено в старых версиях заголовков glibc */
#define MS_SHARED (1<<20) /* изменить на "shared" */
#endif

#ifndef MS_LAZYTIME
#define MS_LAZYTIME     (1<<25)
#endif

/*
Функция usageError():
Выводит сообщение об ошибке, если аргументы командной строки переданы неправильно.
Аргументы:
- progName: имя программы,
- msg: сообщение об ошибке, которое нужно вывести перед подсказкой по использованию.
*/

static void
usageError(const char *progName, const char *msg)
{
    if (msg != NULL)
        fprintf(stderr, "%s", msg);

    fprintf(stderr, "Usage: %s [options] source target\n\n", progName);
    fprintf(stderr, "Доступные параметры:\n");

#define fpe(str) fprintf(stderr, "    " str)    /* Укороченная запись для вывода */
    fpe("-t fstype        [например, 'ext2' или 'reiserfs']\n");
    fpe("-o data          [зависимые от файловой системы параметры,\n");
    fpe("                 например, 'bsdgroups' для ext2]\n");
    fpe("-f mountflags    можно указать следующие флаги:\n");

#define fpe2(str) fprintf(stderr, "            " str)
    fpe2("b - MS_BIND         создать привязку к каталогу\n");
    fpe2("d - MS_DIRSYNC      синхронное обновление каталогов\n");
    fpe2("l - MS_MANDLOCK     разрешить обязательную блокировку\n");
    fpe2("L - MS_LAZYATIME    ленивое обновление atime\n");
    fpe2("m - MS_MOVE         атомарное перемещение поддерева\n");
    fpe2("A - MS_NOATIME      не обновлять время последнего доступа (atime)\n");
    fpe2("V - MS_NODEV        запретить доступ к устройствам\n");
    fpe2("D - MS_NODIRATIME   не обновлять atime для каталогов\n");
    fpe2("E - MS_NOEXEC       запретить выполнение файлов\n");
    fpe2("S - MS_NOSUID       запретить программы с set-user/group-ID\n");
    fpe2("p - MS_PRIVATE      пометить как private\n");
    fpe2("r - MS_RDONLY       монтирование только для чтения\n");
    fpe2("c - MS_REC          рекурсивное монтирование\n");
    fpe2("T - MS_RELATIME     относительное обновление atime\n");
    fpe2("R - MS_REMOUNT      повторное монтирование\n");
    fpe2("h - MS_SHARED       пометить как shared\n");
    fpe2("v - MS_SLAVE        пометить как slave\n");
    fpe2("s - MS_SYNCHRONOUS  синхронная запись\n");
    fpe2("u - MS_UNBINDABLE   пометить как unbindable\n");
    exit(EXIT_FAILURE);
}

/*
Функция main():
Основная функция программы, обрабатывает аргументы командной строки, задает флаги
и вызывает mount() для монтирования файловой системы с заданными параметрами.
*/

int
main(int argc, char *argv[])
{
    unsigned long flags;
    char *data, *fstype;
    int j, opt;

    flags = 0;
    data = NULL;
    fstype = NULL;

    while ((opt = getopt(argc, argv, "o:t:f:")) != -1) {  // Обработка опций командной строки
        switch (opt) {
        case 'o':  // Указаны параметры для файловой системы
            data = optarg;
            break;

        case 't':  // Указан тип файловой системы
            fstype = optarg;
            break;

        case 'f':  // Заданы флаги монтирования
            for (j = 0; j < strlen(optarg); j++) {
                // Обработка флагов для mount()
                switch (optarg[j]) {
                case 'b': flags |= MS_BIND;             break;
                case 'd': flags |= MS_DIRSYNC;          break;
                case 'l': flags |= MS_MANDLOCK;         break;
                case 'm': flags |= MS_MOVE;             break;
                case 'A': flags |= MS_NOATIME;          break;
                case 'V': flags |= MS_NODEV;            break;
                case 'D': flags |= MS_NODIRATIME;       break;
                case 'E': flags |= MS_NOEXEC;           break;
                case 'S': flags |= MS_NOSUID;           break;
                case 'p': flags |= MS_PRIVATE;          break;
                case 'r': flags |= MS_RDONLY;           break;
                case 'c': flags |= MS_REC;              break;
                case 'T': flags |= MS_RELATIME;         break;
                case 'R': flags |= MS_REMOUNT;          break;
                case 'h': flags |= MS_SHARED;           break;
                case 'v': flags |= MS_SLAVE;            break;
                case 's': flags |= MS_SYNCHRONOUS;      break;
                case 'u': flags |= MS_UNBINDABLE;       break;
                default:  usageError(argv[0], NULL);
                }
            }
            break;

        default:
            usageError(argv[0], NULL);
        }
    }

    if (argc != optind + 2) // Проверка на корректность количества аргументов
        usageError(argv[0], "Неверное количество аргументов\n");

    // Вызов mount() с переданными параметрами
    if (mount(argv[optind], argv[optind + 1], fstype, flags, data) == -1)
        errExit("mount");  // Завершение с ошибкой, если mount() завершился неудачей

    exit(EXIT_SUCCESS);  // Успешное завершение программы
}
```

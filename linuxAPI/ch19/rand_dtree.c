
/* rand_dtree.c

   Тестовая программа для использования вместе с inotify_dtree.c.

   Эта программа случайным образом создает, удаляет или переименовывает
   подкаталоги в директории, указанной в единственном аргументе командной строки.
*/

#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 700
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif
#include <stdarg.h>
#include <limits.h>
#include <ftw.h>
#include "tlpi_hdr.h"

#define DLIM 60

/* Хак! Мы не можем передавать аргументы в функцию, вызываемую nftw(),
   поэтому мы используем эти глобальные переменные для обмена информацией с функцией */

static int dcnt;
static char **dirList = NULL;
static int dlSize = 0;
static const int D_INCR = 1000;

static int
traverseTree(const char *pathname, const struct stat *sb, int tflag,
             struct FTW *ftwbuf)
{
    if (! S_ISDIR(sb->st_mode))
        return 0;

    //printf("%s\n", pathname);

    if (dcnt >= dlSize) {
        dlSize += D_INCR;
        dirList = realloc(dirList, dlSize * sizeof(char *));
        if (dirList == NULL)
            errExit("realloc");
    }

    dirList[dcnt] = strdup(pathname);

    dcnt++;

    return 0;
}

static int
getDirList(const char *pathname)
{
    dcnt = 0;

    if (nftw(pathname, traverseTree, 20, FTW_PHYS) == -1)
        errMsg("nftw: %s", pathname);

    return dcnt;
}

static FILE *logfp = NULL;

static void
logMessage(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);

    if (logfp != NULL)
        vfprintf(logfp, format, argList);

    va_end(argList);
}

static void
usageError(char *pname)
{
    fprintf(stderr, "Использование: %s [опции] dirpath {c|d|m}\n\n", pname);
    fprintf(stderr, "Выполнение случайных операций в "
            "дереве каталогов 'dirpath'\n");
    fprintf(stderr, "    c == создание каталогов\n");
    fprintf(stderr, "    d == удаление каталогов\n");
    fprintf(stderr, "    m == переименование каталогов\n\n");
    fprintf(stderr, "Опции:\n");
    fprintf(stderr, "    -l logfile     Записать действия в лог файл\n");
    fprintf(stderr, "    -m maxops      Выполнить не более 'maxops' операций "
            "(по умолчанию без ограничения)\n");
    fprintf(stderr, "    -s usecs       Спать 'usecs' микросекунд "
            "между каждой операцией\n");
    fprintf(stderr, "    -z stopfile    Немедленно остановиться, когда будет создан файл "
            "'stopfile'\n");
    exit(EXIT_FAILURE);
}

#define MARKER_STRING "--"

int
main(int argc, char *argv[])
{
    srandom(0);

    char *stopFile = NULL;
    int maxops = 0;
    int usecs = 1;
    int opt;
    while ((opt = getopt(argc, argv, "l:m:s:z:")) != -1) {
        switch (opt) {
        case 's':
            usecs = atoi(optarg);
            break;

        case 'z':
            stopFile = optarg;
            break;

        case 'm':
            maxops = atoi(optarg);
            break;

        case 'l':
            logfp = fopen(optarg, "w+");
            if (logfp == NULL)
                errExit("fopen");
            setbuf(logfp, NULL);
            break;

        default:
            usageError(argv[0]);
        }
    }

    if (optind + 1 >= argc)
        usageError(argv[0]);

    int opcnt = 0;

    for (;;) {
        getDirList(argv[optind]);

        int nslashes;
        char *to_move;
        char path[PATH_MAX];

        switch (argv[optind + 1][0]) {
        case 'c':
            snprintf(path, sizeof(path), "%s/%ld%s%s_%d",
                    dirList[random() % dcnt],
                    (long) getpid() % 100, MARKER_STRING, "cr", opcnt);
            if (strlen(path) > DLIM)
                continue;
            nslashes = 0;
            for (char *p = path; *p; p++)
                if (*p == '/')
                    nslashes++;

            if (nslashes > 1)
                if (random() % nslashes > 0)
                    continue;

            if (mkdir(path, 0700) == 0)
                logMessage("mkdir: %s\n", path);

            int scnt = 1;
            while ((random() % 3) < 2) {
                char spath[PATH_MAX];

                int s = snprintf(spath, sizeof(path), "%s/%ld%s%s%d_%d", path,
                        (long) getpid() % 100,
                        MARKER_STRING, "scr", scnt, opcnt);
                if (s > DLIM)
                    break;

                if (mkdir(spath, 0700) == 0)
                    logMessage("mkdir: %s\n", spath);

                strncpy(path, spath, PATH_MAX);
                path[PATH_MAX - 1] = '\0';
                scnt++;
            }
            break;

        case 'd':
            if (dcnt == 0)
                continue;

            snprintf(path, sizeof(path), "%s", dirList[random() % dcnt]);
            while (strstr(path, MARKER_STRING) != NULL) {

                if (rmdir(path) == -1)
                    break;
                logMessage("rmdir: %s\n", path);

                char *p = strrchr(path, '/');
                if (p == NULL)
                    break;

                *p = '\0';
            }
            break;

        case 'm':
            if (dcnt < 3)
                continue;

            to_move = dirList[random() % dcnt];

            if (strstr(to_move, MARKER_STRING) != NULL) {
                char tfile[PATH_MAX];

                char *p = strrchr(to_move, '/');
                snprintf(tfile, sizeof(tfile), "%s", p + 1);
                p = strstr(tfile, "__ren");
                if (p != NULL)
                    *p = '\0';

                char target[PATH_MAX];
                int s = snprintf(target, sizeof(target), "%s/%s__ren%04d-%ld",
                                dirList[random() % dcnt],
                                tfile, opcnt, (long) getpid());

                if (s > DLIM)
                    break;

                if (rename(to_move, target) == 0)
                    logMessage("rename: %s ==> %s\n", to_move, target);
            }

            break;
        }

        for (int j = 0; j < dcnt; j++) {
            free(dirList[j]);
            dirList[j] = NULL;
        }

        opcnt++;

        usleep(usecs);

        if (maxops > 0 && opcnt >= maxops)
            break;

        if (access(stopFile, F_OK) == 0)
            break;
    }

    exit(EXIT_SUCCESS);
}

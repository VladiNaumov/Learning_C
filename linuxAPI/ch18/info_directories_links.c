/* 18. Directories and Links */

/*
 * link - создает жесткую ссылку на файл.
 * oldpath - исходный путь.
 * newpath - новый путь для жесткой ссылки.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <unistd.h>
int link(const char *oldpath, const char *newpath);

/*
 * unlink - удаляет файл или символическую ссылку.
 * pathname - путь к файлу или символической ссылке.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <unistd.h>
int unlink(const char *pathname);

/*
 * rename - переименовывает файл или каталог.
 * oldpath - старый путь.
 * newpath - новый путь.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <stdio.h>
int rename(const char *oldpath, const char *newpath);

/*
 * symlink - создает символическую ссылку.
 * filepath - путь к целевому файлу.
 * linkpath - путь для символической ссылки.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <unistd.h>
int symlink(const char *filepath, const char *linkpath);

/*
 * readlink - читает символическую ссылку.
 * pathname - путь к символической ссылке.
 * buffer - буфер для записи целевого пути.
 * bufsiz - размер буфера.
 * Возвращает количество записанных байт или -1 при ошибке.
 */
#include <unistd.h>
ssize_t readlink(const char *pathname, char *buffer, size_t bufsiz);

/*
 * mkdir - создает новый каталог.
 * pathname - путь к новому каталогу.
 * mode - режим доступа (права).
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <sys/stat.h>
int mkdir(const char *pathname, mode_t mode);

/*
 * rmdir - удаляет пустой каталог.
 * pathname - путь к удаляемому каталогу.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <unistd.h>
int rmdir(const char *pathname);

/*
 * remove - удаляет файл или каталог.
 * pathname - путь к файлу или каталогу.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <stdio.h>
int remove(const char *pathname);

/*
 * opendir - открывает каталог для чтения.
 * dirpath - путь к каталогу.
 * Возвращает указатель на структуру DIR или NULL при ошибке.
 */
#include <dirent.h>
DIR *opendir(const char *dirpath);

/*
 * fdopendir - открывает каталог, используя файловый дескриптор.
 * fd - файловый дескриптор.
 * Возвращает указатель на структуру DIR или NULL при ошибке.
 */
#include <dirent.h>
DIR *fdopendir(int fd);

/*
 * readdir - читает следующую запись из каталога.
 * dirp - указатель на структуру DIR.
 * Возвращает указатель на структуру dirent или NULL при ошибке или конце каталога.
 */
#include <dirent.h>
struct dirent *readdir(DIR *dirp);

/*
 * rewinddir - сбрасывает указатель на начало каталога.
 * dirp - указатель на структуру DIR.
 * Возвращает void.
 */
#include <dirent.h>
void rewinddir(DIR *dirp);

/*
 * closedir - закрывает каталог.
 * dirp - указатель на структуру DIR.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <dirent.h>
int closedir(DIR *dirp);

/*
 * dirfd - возвращает файловый дескриптор для открытого каталога.
 * dirp - указатель на структуру DIR.
 * Возвращает файловый дескриптор или -1 при ошибке.
 */
#include <dirent.h>
int dirfd(DIR *dirp);

/*
 * readdir_r - безопасная версия readdir для многозадачности.
 * dirp - указатель на структуру DIR.
 * entry - структура для записи записи каталога.
 * result - указатель на результат.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <dirent.h>
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);

/*
 * nftw - выполняет обход каталога с вызовом функции для каждого элемента.
 * dirpath - путь к каталогу.
 * func - функция обратного вызова.
 * nopenfd - максимальное количество файлов, которые могут быть открыты.
 * flags - флаги.
 * Возвращает количество обработанных файлов или -1 при ошибке.
 */
#define _XOPEN_SOURCE 500
#include <ftw.h>
int nftw(const char *dirpath, int (*func)(const char *pathname, const struct stat *statbuf, int typeflag, struct FTW *ftwbuf), int nopenfd, int flags);

/*
 * getcwd - получает текущую рабочую директорию.
 * cwdbuf - буфер для хранения пути.
 * size - размер буфера.
 * Возвращает указатель на буфер или NULL при ошибке.
 */
#include <unistd.h>
char *getcwd(char *cwdbuf, size_t size);

/*
 * chdir - изменяет текущую рабочую директорию.
 * pathname - путь к новой рабочей директории.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <unistd.h>
int chdir(const char *pathname);

/*
 * fchdir - изменяет текущую рабочую директорию по файловому дескриптору.
 * fd - файловый дескриптор.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#define _XOPEN_SOURCE 500
#include <unistd.h>
int fchdir(int fd);

/*
 * openat - открывает файл относительно заданной директории.
 * dirfd - файловый дескриптор директории.
 * pathname - путь к файлу.
 * flags - флаги открытия.
 * mode - права доступа (опционально).
 * Возвращает файловый дескриптор или -1 при ошибке.
 */
#define _XOPEN_SOURCE 700
#include <fcntl.h>
int openat(int dirfd, const char *pathname, int flags, ... /* mode_t mode */);

/*
 * chroot - изменяет корневую директорию для процесса.
 * pathname - путь к новой корневой директории.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#define _BSD_SOURCE
#include <unistd.h>
int chroot(const char *pathname);

/*
 * realpath - разрешает абсолютный путь.
 * pathname - путь к файлу или каталогу.
 * resolved_path - буфер для записи абсолютного пути.
 * Возвращает указатель на resolved_path или NULL при ошибке.
 */
#include <stdlib.h>
char *realpath(const char *pathname, char *resolved_path);

/*
 * dirname - возвращает имя каталога из пути.
 * pathname - путь к файлу или каталогу.
 * Возвращает указатель на строку с именем каталога.
 */
#include <libgen.h>
char *dirname(char *pathname);

/*
 * basename - возвращает имя файла из пути.
 * pathname - путь к файлу или каталогу.
 * Возвращает указатель на строку с именем файла.
 */
#include <libgen.h>
char *basename(char *pathname);
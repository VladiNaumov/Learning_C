/* 18. Directories and Links */

#include <unistd.h>
/* Создает жесткую ссылку на файл по пути `oldpath` с новым именем `newpath`. */
int link(const char *oldpath, const char *newpath);

#include <unistd.h>
/* Удаляет ссылку на файл по пути `pathname`. Если это последняя ссылка, файл удаляется. */
int unlink(const char *pathname);

#include <stdio.h>
/* Переименовывает файл или директорию с `oldpath` на `newpath`. */
int rename(const char *oldpath, const char *newpath);

#include <unistd.h>
/* Создает символическую ссылку `linkpath` на файл по пути `filepath`. */
int symlink(const char *filepath, const char *linkpath);

#include <unistd.h>
/* Читает содержимое символической ссылки `pathname` в буфер `buffer` размера `bufsiz`. */
ssize_t readlink(const char *pathname, char *buffer, size_t bufsiz);

#include <sys/stat.h>
/* Создает новую директорию по пути `pathname` с правами доступа `mode`. */
int mkdir(const char *pathname, mode_t mode);

#include <unistd.h>
/* Удаляет пустую директорию по пути `pathname`. */
int rmdir(const char *pathname);

#include <stdio.h>
/* Удаляет файл или пустую директорию по пути `pathname`. */
int remove(const char *pathname);

#include <dirent.h>
/* Открывает директорию `dirpath` и возвращает указатель на объект DIR для работы с ней. */
DIR *opendir(const char *dirpath);

#include <dirent.h>
/* Открывает директорию по дескриптору файла `fd`. */
DIR *fdopendir(int fd);

#include <dirent.h>
/* Читает следующий элемент (файл или директорию) в открытой директории `dirp`. */
struct dirent *readdir(DIR *dirp);

#include <dirent.h>
/* Сбрасывает позицию чтения в директории `dirp` на начало. */
void rewinddir(DIR *dirp);

#include <dirent.h>
/* Закрывает открытую директорию `dirp`. */
int closedir(DIR *dirp);

#include <dirent.h>
/* Получает файловый дескриптор, связанный с открытой директорией `dirp`. */
int dirfd(DIR *dirp);

#include <dirent.h>
/* Потокобезопасная версия функции readdir. Читает следующий элемент директории `dirp`. */
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);

#define _XOPEN_SOURCE 500
#include <ftw.h>
/* Рекурсивно обходит директорию `dirpath`, вызывая функцию `func` для каждого элемента. */
int nftw(const char *dirpath,
         int (*func)(const char *pathname, const struct stat *statbuf,
                     int typeflag, struct FTW *ftwbuf),
         int nopenfd, int flags);

#include <unistd.h>
/* Получает текущий рабочий каталог и сохраняет его путь в буфер `cwdbuf` размера `size`. */
char *getcwd(char *cwdbuf, size_t size);

#include <unistd.h>
/* Изменяет текущий рабочий каталог на `pathname`. */
int chdir(const char *pathname);

#define _XOPEN_SOURCE 500     /* Or: #define _BSD_SOURCE */
#include <unistd.h>
/* Изменяет текущий рабочий каталог на каталог, открытый файловым дескриптором `fd`. */
int fchdir(int fd);

#define _XOPEN_SOURCE 700     /* Or define _POSIX_C_SOURCE >= 200809 */
#include <fcntl.h>
/* Открывает файл по пути `pathname` относительно каталога, указанного дескриптором `dirfd`. */
int openat(int dirfd, const char *pathname, int flags, ... /* mode_t mode */);

#define _BSD_SOURCE
#include <unistd.h>
/* Устанавливает корневой каталог для текущего процесса на `pathname`. */
int chroot(const char *pathname);

#include <stdlib.h>
/* Возвращает абсолютный путь для файла, заданного `pathname`, и сохраняет его в `resolved_path`. */
char *realpath(const char *pathname, char *resolved_path);

#include <libgen.h>
/* Возвращает путь к родительскому каталогу `pathname`. */
char *dirname(char *pathname);
/* Возвращает базовое имя файла из `pathname`. */
char *basename(char *pathname);

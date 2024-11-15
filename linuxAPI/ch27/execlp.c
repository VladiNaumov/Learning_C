/* execlp.c

   Реализация функции execlp() с использованием execve().

   Подробности см. в спецификации SUSv3 для функций exec
   (XSH:exec). Более полную альтернативную реализацию можно найти в
   posix/exec*.c в исходных кодах glibc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

extern char **environ;

#define max(x,y) ((x) > (y) ? (x) : (y))
#define SHELL_PATH "/bin/sh"            /* Путь к стандартному shell */

/* Выполнить скрипт с помощью стандартного shell */

static void
execShScript(int argc, char *argv[], char *envp[])
{
    char *shArgv[argc + 1];     /* Массив аргументов для shell */

    shArgv[0] = SHELL_PATH;     /* Первый аргумент — путь к shell */
    for (int j = 0; j <= argc; j++)
        shArgv[j + 1] = argv[j];
    execve(SHELL_PATH, shArgv, envp);

    /* Если execve() не сработал, вернемся к вызывающей функции */
}

int
execlp(const char *filename, const char *arg, ...)
{
    /***** Построение списка аргументов из переменного количества аргументов *****/

    int argvSize = 100;                 /* Начальный размер массива аргументов */
    char **argv = calloc(argvSize, sizeof(void *));
    if (argv == NULL)
        return -1;

    argv[0] = (char *) arg;
    int argc = 1;

    /* Проходим по переменному списку аргументов, пока не найдем NULL,
       постепенно заполняя массив argv */

    va_list argList;                    /* Переменный список аргументов */
    va_start(argList, arg);
    while (argv[argc - 1] != NULL) {
        if (argc + 1 >= argvSize) {     /* Увеличиваем argv при необходимости */
            char **nargv;

            argvSize += 100;
            nargv = realloc(argv, argvSize * sizeof(void *));
            if (nargv == NULL) {
                free(argv);
                return -1;
            } else {
                argv = nargv;
            }
        }

        argv[argc] = va_arg(argList, char *);
        argc++;
    }

    va_end(argList);

    /***** Создание envp на основе окружения вызывающего процесса *****/

    int j;

    for (j = 0; environ[j] != NULL; )   /* Определяем размер environ */
        j++;
    char **envp = calloc(j + 1, sizeof(void *));
    if (envp == NULL) {
        free(argv);
        return -1;
    }

    for (j = 0; environ[j] != NULL; j++)    /* Копируем environ в envp */
        envp[j] = strdup(environ[j]);
    envp[j] = NULL;             /* Массив должен заканчиваться NULL */

    /***** Пытаемся выполнить файл filename *****/

    int fndEACCES = false;              /* Флаг, если execve() возвращает EACCES */
    int savedErrno;

    if (strchr(filename, '/') != NULL) {

         /* Если файл содержит '/', он является путем, и не используем PATH */

        char *pathname = strdup(filename);

        execve(pathname, argv, envp);

        savedErrno = errno;             /* Сохраняем errno для возвращения */
        if (errno == ENOEXEC)
            execShScript(argc, argv, envp);

        free(pathname);                 /* Освобождаем память */

    } else {            /* Используем PATH */

        /* Если PATH не определен, задаем его как "." */

        char *p = getenv("PATH");
        char *PATH = (p == NULL || strlen(p) == 0) ? strdup(".") : strdup(p);

        /* Проходим по каждому префиксу PATH, пытаясь выполнить файл filename.
           Цикл завершится, когда мы выполним файл или закончатся префиксы. */

        char *prStart, *prEnd;          /* Начало и конец текущего префикса */
        prStart = PATH;
        bool morePrefixes = true;       /* Флаг наличия префиксов в PATH */

        while (morePrefixes) {

            /* Находим конец префикса */

            prEnd = strchr(prStart, ':');
            if (prEnd == NULL)          /* Последний префикс */
                prEnd = prStart + strlen(prStart);

            /* Создаем полный путь из префикса и имени файла */

            char *pathname;
            pathname = malloc(max(1, prEnd - prStart) + strlen(filename) + 2);
            pathname[0] = '\0';
            if (prEnd == prStart)       /* Пустой префикс — текущий каталог */
                strcat(pathname, ".");
            else
                strncat(pathname, prStart, prEnd - prStart);
            strcat(pathname, "/");
            strcat(pathname, filename);

            if (*prEnd == '\0')         /* Нет больше префиксов */
                morePrefixes = false;
            else
                prStart = prEnd + 1;    /* Переход к следующему префиксу */

            /* Пытаемся выполнить pathname; execve() вернет значение только при ошибке */

            execve(pathname, argv, envp);
            savedErrno = errno;         /* Сохраняем errno для возвращения */
            if (errno == EACCES)
                fndEACCES = true;
            else if (errno == ENOEXEC)
                execShScript(argc, argv, envp);

            /* Освобождаем память для pathname */

            free(pathname);
        }

        free(PATH);
    }

    /* Если мы дошли сюда, execve() завершился ошибкой;
       очищаем память и возвращаем -1, сохраняя errno */

    free(argv);
    for (int j = 0; envp[j] != NULL; j++)
        free(envp[j]);
    free(envp);

    /* SUSv3 требует, что если любой execve() вернул EACCES,
       тогда exec[lv]p() должен вернуть именно это значение */

    errno = fndEACCES ? EACCES : savedErrno;
    return -1;
}
/*

### Резюме кода

Этот код реализует функцию `execlp()` через вызов `execve()`. `execlp()` 
позволяет выполнить программу, передав ей аргументы и окружение. Если переданный файл не содержит путь, функция ищет его в `PATH`. 
При ошибках выполнения, таких как отсутствие разрешения или неправильный формат файла, программа пытается выполнить его как скрипт с помощью `/bin/sh`.
*/
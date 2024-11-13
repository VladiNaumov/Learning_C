/* fork_file_sharing.c

   Демонстрация того, что файловые дескрипторы дочернего процесса, созданного с помощью fork(),
   ссылаются на те же самые открытые файловые объекты, что и у родительского процесса.
*/

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int fd, flags;
    char template[] = "/tmp/testXXXXXX";  /* Шаблон для создания временного файла */

    setbuf(stdout, NULL);                   /* Отключение буферизации stdout */

    /* Открываем временный файл, устанавливаем его смещение в произвольное значение,
       и меняем один из флагов открытого файла. */

    fd = mkstemp(template);    /* Создаем временный файл */
    if (fd == -1)
        errExit("mkstemp");

    printf("File offset before fork(): %lld\n",
            (long long) lseek(fd, 0, SEEK_CUR));  /* Получаем текущее смещение файла */

    flags = fcntl(fd, F_GETFL);  /* Получаем флаги файла */
    if (flags == -1)
        errExit("fcntl - F_GETFL");
    printf("O_APPEND flag before fork() is: %s\n",
            (flags & O_APPEND) ? "on" : "off");

    switch (fork()) {
    case -1:
        errExit("fork");

    case 0:     /* Дочерний процесс: изменяем смещение файла и флаги */
        if (lseek(fd, 1000, SEEK_SET) == -1)  /* Устанавливаем смещение файла в 1000 */
            errExit("lseek");

        flags = fcntl(fd, F_GETFL);         /* Получаем текущие флаги файла */
        if (flags == -1)
            errExit("fcntl - F_GETFL");
        flags |= O_APPEND;                  /* Включаем флаг O_APPEND */
        if (fcntl(fd, F_SETFL, flags) == -1)  /* Применяем изменения флагов */
            errExit("fcntl - F_SETFL");
        _exit(EXIT_SUCCESS);

    default:    /* Родительский процесс: видит изменения, сделанные дочерним процессом */
        if (wait(NULL) == -1)
            errExit("wait");                /* Ждем завершения дочернего процесса */
        printf("Child has exited\n");

        printf("File offset in parent: %lld\n",
                (long long) lseek(fd, 0, SEEK_CUR));  /* Проверяем смещение файла в родительском процессе */

        flags = fcntl(fd, F_GETFL);  /* Получаем флаги файла в родительском процессе */
        if (flags == -1)
            errExit("fcntl - F_GETFL");
        printf("O_APPEND flag in parent is: %s\n",
                (flags & O_APPEND) ? "on" : "off");  /* Проверяем флаг O_APPEND в родительском процессе */
        exit(EXIT_SUCCESS);
    }
}

/*
### Резюме кода
Этот код демонстрирует, что файловые дескрипторы, унаследованные дочерним процессом после вызова `fork()`, 
ссылаются на те же самые открытые файловые объекты, что и у родительского процесса. 
После создания временного файла родитель и дочерний процесс изменяют его состояние, включая смещение в файле и флаги, такие как `O_APPEND`. 
Дочерний процесс изменяет эти параметры, а родительский процесс, после завершения дочернего, видит изменения, сделанные в дочернем процессе, 
подтверждая, что они используют один и тот же файловый объект.

*/
/* system.c

   Реализация функции system(3).
*/
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

int
system(const char *command)
{
    sigset_t blockMask, origMask;
    struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
    pid_t childPid;
    int status, savedErrno;

    if (command == NULL)                /* Проверяет, доступен ли shell */
        return system(":") == 0;

    /* Родительский процесс (вызывающий system()) блокирует SIGCHLD и
       игнорирует SIGINT и SIGQUIT во время выполнения дочернего процесса.
       Настройки сигналов меняются перед fork() для предотвращения 
       гонки. После fork() их необходимо восстановить в дочернем процессе. */

    sigemptyset(&blockMask);            /* Блокируем SIGCHLD */
    sigaddset(&blockMask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &blockMask, &origMask);

    saIgnore.sa_handler = SIG_IGN;      /* Игнорируем SIGINT и SIGQUIT */
    saIgnore.sa_flags = 0;
    sigemptyset(&saIgnore.sa_mask);
    sigaction(SIGINT, &saIgnore, &saOrigInt);
    sigaction(SIGQUIT, &saIgnore, &saOrigQuit);

    switch (childPid = fork()) {
    case -1: /* Ошибка при fork() */
        status = -1;
        break;          /* Продолжаем для сброса настроек сигналов */

    case 0: /* Дочерний процесс: выполнение команды */

        /* Игнорируем ошибки, так как они не повлияют на вызывающего system(),
           поскольку родительский процесс независим от дочернего. */

        saDefault.sa_handler = SIG_DFL; /* Восстанавливаем стандартные обработчики */
        saDefault.sa_flags = 0;
        sigemptyset(&saDefault.sa_mask);

        if (saOrigInt.sa_handler != SIG_IGN)
            sigaction(SIGINT, &saDefault, NULL);
        if (saOrigQuit.sa_handler != SIG_IGN)
            sigaction(SIGQUIT, &saDefault, NULL);

        sigprocmask(SIG_SETMASK, &origMask, NULL); /* Восстанавливаем маску сигналов */

        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        _exit(127);                     /* Ошибка: не удалось выполнить shell */

    default: /* Родительский процесс: ждем завершения дочернего процесса */

        /* Используем waitpid() для получения статуса дочернего процесса.
           wait() мог бы случайно обработать другие дочерние процессы. */

        while (waitpid(childPid, &status, 0) == -1) {
            if (errno != EINTR) {       /* Ошибка, отличная от EINTR */
                status = -1;
                break;                  /* Выход из цикла */
            }
        }
        break;
    }

    /* Разблокируем SIGCHLD, восстанавливаем обработчики SIGINT и SIGQUIT */

    savedErrno = errno;                 /* Сохраняем errno, так как он может измениться */

    sigprocmask(SIG_SETMASK, &origMask, NULL);
    sigaction(SIGINT, &saOrigInt, NULL);
    sigaction(SIGQUIT, &saOrigQuit, NULL);

    errno = savedErrno;

    return status;
}
/*
### Резюме кода

Этот код реализует функцию `system()`, которая выполняет указанную команду в дочернем процессе, используя shell. 
В родительском процессе временно блокируются сигналы `SIGCHLD`, `SIGINT`, и `SIGQUIT` для предотвращения сбоев, а затем восстанавливаются. 
В дочернем процессе команда запускается через shell с использованием `execl`.
*/
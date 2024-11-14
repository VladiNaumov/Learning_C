/* print_wait_status.c

   Разбор и вывод статуса завершения процесса, возвращаемого функцией wait() 
   и аналогичными вызовами.
*/
#define _GNU_SOURCE     /* Подключает декларацию функции strsignal() из <string.h> */
#include <string.h>
#include <sys/wait.h>
#include "print_wait_status.h"  /* Подключение функции printWaitStatus() */
#include "tlpi_hdr.h"

/* ПРИМЕЧАНИЕ: В следующей функции используется printf(), который не является
   безопасным для вызова из асинхронных сигналов (см. Раздел 21.1.2). Поэтому
   эта функция также не является асинхронно-безопасной (не следует вызывать её из
   обработчика сигнала SIGCHLD). */

void                    /* Анализирует статус wait() с использованием макросов W* */
printWaitStatus(const char *msg, int status)
{
    if (msg != NULL)
        printf("%s", msg); /* Печатает сообщение, если оно задано */

    if (WIFEXITED(status)) {
        /* Если процесс завершился нормально */
        printf("child exited, status=%d\n", WEXITSTATUS(status));

    } else if (WIFSIGNALED(status)) {
        /* Если процесс был убит сигналом */
        printf("child killed by signal %d (%s)",
                WTERMSIG(status), strsignal(WTERMSIG(status))); /* Выводит номер и название сигнала */
#ifdef WCOREDUMP        /* Этот макрос может отсутствовать на некоторых системах */
        if (WCOREDUMP(status))
            printf(" (core dumped)"); /* Указывает, что произошел дамп памяти */
#endif
        printf("\n");

    } else if (WIFSTOPPED(status)) {
        /* Если процесс был остановлен сигналом */
        printf("child stopped by signal %d (%s)\n",
                WSTOPSIG(status), strsignal(WSTOPSIG(status))); /* Выводит сигнал, остановивший процесс */

#ifdef WIFCONTINUED     /* Поддерживается в SUSv3, но может отсутствовать в старых версиях Linux и некоторых UNIX-системах */
    } else if (WIFCONTINUED(status)) {
        /* Если процесс продолжил выполнение после остановки */
        printf("child continued\n");
#endif

    } else {            /* Этот случай не должен происходить */
        printf("what happened to this child? (status=%x)\n",
                (unsigned int) status); /* Выводит непредвиденный статус процесса */
    }
}

/*

### Резюме кода

Эта программа анализирует статус завершения дочернего процесса после вызова функции `wait()` и печатает результат, 
объясняя причину завершения процесса. Она определяет, был ли процесс завершен, убит сигналом, остановлен или продолжен после остановки. 
Функция использует макросы `WIF*` и `W*` для определения состояния процесса и выводит соответствующие сообщения. 
Код также поддерживает вывод информации о дампе памяти, если процесс завершился с его созданием.
*/

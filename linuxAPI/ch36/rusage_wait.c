/* rusage_wait.c

   Демонстрация того, что вызов getrusage() с RUSAGE_CHILDREN 
   возвращает информацию только о тех дочерних процессах, 
   для которых был вызван wait().
*/

#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/resource.h>
#include "tlpi_hdr.h"

#define NSECS 3         /* Количество времени CPU, которое будет потреблено дочерним процессом */

#define SIG SIGUSR1     /* Сигнал, который дочерний процесс отправляет родителю, 
                           сообщая, что он завершает выполнение */

/* Обработчик сигнала, используется для прерывания sigsuspend() */
static void
handler(int sig)
{
    /* Пустая функция, прерывает ожидание в sigsuspend() */
}

/* Вывод информации об использовании ресурсов дочерними процессами */
static void
printChildRusage(const char *msg)
{
    struct rusage ru;

    printf("%s", msg);
    if (getrusage(RUSAGE_CHILDREN, &ru) == -1)
        errExit("getrusage");
    printf("user CPU=%.2f secs; system CPU=%.2f secs\n",
            ru.ru_utime.tv_sec + ru.ru_utime.tv_usec / 1000000.0,
            ru.ru_stime.tv_sec + ru.ru_stime.tv_usec / 1000000.0);
}

int
main(int argc, char *argv[])
{
    setbuf(stdout, NULL);       /* Отключение буферизации stdout для немедленного вывода */

    /* Установка обработчика сигнала */
    struct sigaction sa;
    sa.sa_handler = handler;   /* Указываем обработчик */
    sa.sa_flags = 0;           /* Нет дополнительных флагов */
    sigemptyset(&sa.sa_mask);  /* Очищаем маску сигналов */
    if (sigaction(SIG, &sa, NULL) == -1)
        errExit("sigaction");

    /* Блокировка сигнала до готовности родительского процесса к его обработке */
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIG);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
        errExit("sigprocmask");

    switch (fork()) {
    case -1:
        /* Ошибка при вызове fork() */
        errExit("fork");

    case 0: /* Дочерний процесс */
        /* Потребление CPU в течение заданного времени */
        for (clock_t start = clock(); clock() - start < NSECS * CLOCKS_PER_SEC;)
            continue;

        printf("Child terminating\n");

        /* Отправка сигнала родителю о скором завершении */
        if (kill(getppid(), SIG) == -1)
            errExit("kill");

        exit(EXIT_SUCCESS); /* Завершение дочернего процесса */

    default: /* Родительский процесс */
        /* Ожидание сигнала от дочернего процесса */
        sigemptyset(&mask);
        sigsuspend(&mask);

        sleep(2); /* Даем дочернему процессу время завершиться */

        /* Печать использования ресурсов до вызова wait() */
        printChildRusage("Before wait: ");
        if (wait(NULL) == -1)
            errExit("wait"); /* Ожидание завершения дочернего процесса */

        /* Печать использования ресурсов после вызова wait() */
        printChildRusage("After wait:  ");
        exit(EXIT_SUCCESS);
    }
}

/*
Резюме:
Программа демонстрирует, что getrusage() с параметром RUSAGE_CHILDREN возвращает
информацию о времени CPU, затраченном дочерними процессами, только после вызова wait().
- Родительский процесс создает дочерний процесс, который выполняет интенсивные вычисления.
- Дочерний процесс сигнализирует о завершении с помощью SIGUSR1.
- Родительский процесс вызывает getrusage() до и после вызова wait(),
  показывая разницу в собранной информации.
*/

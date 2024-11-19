/* disc_SIGHUP.c

   Эта программа демонстрирует, что при "отключении терминала" сигнал SIGHUP
   отправляется всем членам группы процессов, находящейся на переднем плане терминала.

   Попробуйте использовать следующую команду для запуска программы в окне X, а затем закройте окно:

        exec disc_SIGHUP > sig.log 2>&1

   (Поскольку команда заменяет оболочку на эту программу, она становится управляющим процессом для терминала.)
*/

#define _GNU_SOURCE     /* Для получения объявления strsignal() из <string.h> */
#include <string.h>
#include <signal.h>
#include "tlpi_hdr.h"

// Обработчик сигнала SIGHUP
static void handler(int sig)
{
    printf("PID %ld: caught signal %2d (%s)\n", (long) getpid(),
            sig, strsignal(sig));  // Вывод информации о сигнале
    /* НЕБЕЗОПАСНО (см. Раздел 21.1.2 в TLPI) */
}

int main(int argc, char *argv[])
{
    pid_t parentPid, childPid;
    int j;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s {d|s}... [ > sig.log 2>&1 ]\n", argv[0]);

    setbuf(stdout, NULL);  // Отключаем буферизацию для немедленного вывода

    parentPid = getpid();  // Получаем PID родительского процесса
    printf("PID of parent process is:       %ld\n", (long) parentPid);
    printf("Foreground process group ID is: %ld\n",
            (long) tcgetpgrp(STDIN_FILENO));  // ID группы процессов переднего плана

    // Создание дочерних процессов
    for (j = 1; j < argc; j++) {
        childPid = fork();
        if (childPid == -1)
            errExit("fork");

        if (childPid == 0) {  // Дочерний процесс
            if (argv[j][0] == 'd')  // 'd' --> перемещение в другую группу процессов
                if (setpgid(0, 0) == -1)
                    errExit("setpgid");

            // Устанавливаем обработчик сигнала SIGHUP
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if (sigaction(SIGHUP, &sa, NULL) == -1)
                errExit("sigaction");

            break;  // Дочерний процесс выходит из цикла
        }
    }

    // Все процессы продолжают выполнение отсюда

    alarm(60);  // Таймер завершения: завершение через 60 секунд

    printf("PID=%ld PGID=%ld\n", (long) getpid(), (long) getpgrp());

    // Бесконечное ожидание сигналов
    for (;;)
        pause();
}

/*
 * Резюме:
 *
 * - Программа демонстрирует влияние сигнала SIGHUP на процессы, когда происходит отключение терминала.
 * - Родительский процесс создает несколько дочерних процессов. Если аргумент команды 'd', процесс перемещается в новую группу процессов.
 * - Все процессы устанавливают обработчик для SIGHUP, который выводит сообщение при получении сигнала.
 * - При отключении терминала процессы из группы переднего плана получают SIGHUP, а перемещенные в другую группу — нет.
 * - Таймер alarm(60) предотвращает "вечное" выполнение программы.
 *
 * Использование: помогает изучить управление группами процессов и поведение сигналов в UNIX-системах.
 */

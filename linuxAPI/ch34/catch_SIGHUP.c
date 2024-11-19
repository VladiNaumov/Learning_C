/* catch_SIGHUP.c

   Перехват сигнала SIGHUP и отображение сообщения.

   Использование: catch_SIGHUP [x] [ > logfile 2>&1 ]
*/

#if !defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 500
#define _XOPEN_SOURCE 500
#endif
#include <unistd.h>
#include <signal.h>
#include "tlpi_hdr.h"

// Обработчик сигнала SIGHUP
static void handler(int sig)
{
    // Пустой обработчик; просто для регистрации сигнала
}

int main(int argc, char *argv[])
{
    pid_t childPid;
    struct sigaction sa;

    setbuf(stdout, NULL);  // Отключаем буферизацию stdout для немедленного вывода сообщений

    // Настраиваем обработчик для сигнала SIGHUP
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGHUP, &sa, NULL) == -1)
        errExit("sigaction");

    // Создаем дочерний процесс
    childPid = fork();
    if (childPid == -1)
        errExit("fork");

    // Дочерний процесс может быть перемещен в новую группу процессов
    if (childPid == 0 && argc > 1)
        if (setpgid(0, 0) == -1)  // Переместить процесс в новую группу
            errExit("setpgid");

    // Вывод информации о текущем процессе
    printf("PID=%ld; PPID=%ld; PGID=%ld; SID=%ld\n",
           (long) getpid(), (long) getppid(),
           (long) getpgrp(), (long) getsid(0));

    // Устанавливаем таймер на 60 секунд. Если программа не завершится
    // по другим причинам, она будет убита сигналом SIGALRM.
    alarm(60);

    // Бесконечный цикл ожидания сигналов
    for (;;) {
        pause();  // Ожидаем сигнал
        printf("%ld: caught SIGHUP\n", (long) getpid());
    }
}

/*
 * Резюме:
 *
 * - Программа демонстрирует обработку сигнала SIGHUP с помощью пользовательского обработчика.
 * - После запуска программа выводит информацию о процессе: его PID, PPID, PGID и SID.
 * - Если указать аргумент `x`, дочерний процесс будет перемещен в новую группу процессов.
 * - Обработчик `handler` срабатывает при получении сигнала SIGHUP, и программа выводит сообщение.
 * - Таймер `alarm(60)` завершает программу через 60 секунд, если она не завершилась ранее.
 *
 * Основное применение — проверка поведения сигналов и управления группами процессов.
 */

/* handling_SIGTSTP.c

   Пример корректного способа обработки сигнала SIGTSTP и его повторного вызова,
   чтобы родительский процесс мог обнаружить, что программа была остановлена SIGTSTP.
*/

#include <signal.h>
#include <errno.h>
#include "tlpi_hdr.h"

// Обработчик для SIGTSTP
static void tstpHandler(int sig)
{
    sigset_t tstpMask, prevMask;
    int savedErrno;
    struct sigaction sa;

    savedErrno = errno;  // Сохранение errno для последующего восстановления

    printf("Caught SIGTSTP\n");  // Печатаем сообщение (НЕБЕЗОПАСНО)

    // Устанавливаем обработчик сигнала SIGTSTP по умолчанию
    if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
        errExit("signal");

    // Генерируем сигнал SIGTSTP для приостановки процесса
    raise(SIGTSTP);

    // Разблокируем SIGTSTP, чтобы процесс был немедленно приостановлен
    sigemptyset(&tstpMask);
    sigaddset(&tstpMask, SIGTSTP);
    if (sigprocmask(SIG_UNBLOCK, &tstpMask, &prevMask) == -1)
        errExit("sigprocmask");

    // Возобновление выполнения здесь после SIGCONT
    if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
        errExit("sigprocmask");

    // Восстанавливаем обработчик для SIGTSTP
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = tstpHandler;
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
        errExit("sigaction");

    printf("Exiting SIGTSTP handler\n");
    errno = savedErrno;  // Восстанавливаем errno
}

int main(int argc, char *argv[])
{
    struct sigaction sa;

    // Устанавливаем обработчик для SIGTSTP, если он не игнорируется
    if (sigaction(SIGTSTP, NULL, &sa) == -1)
        errExit("sigaction");

    if (sa.sa_handler != SIG_IGN) {
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = tstpHandler;
        if (sigaction(SIGTSTP, &sa, NULL) == -1)
            errExit("sigaction");
    }

    for (;;) {  // Ожидание сигналов
        pause();
        printf("Main\n");
    }
}

/*
 * Основное:
 * - Этот код демонстрирует корректный способ обработки SIGTSTP с последующим восстановлением.
 * - Программа печатает сообщения при перехвате SIGTSTP и после SIGCONT.
 * - Поведение программы позволяет родительскому процессу видеть, что она была приостановлена.
 */

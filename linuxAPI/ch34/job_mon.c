/* job_mon.c

   Эта программа полезна для:

      - демонстрации порядка создания процессов в конвейере shell и назначения
        группы процессов этим процессам;

      - мониторинга сигналов управления заданиями, отправляемых процессу (группе).

   Программа отображает свой PID, PID родителя и группу процессов.

   Попробуйте запустить конвейер, состоящий из таких команд:

        job_mon | job_mon | job_mon

   Это продемонстрирует назначение групп процессов и PID каждому процессу в конвейере.

   Попробуйте запустить конвейер в разных оболочках, а также в фоновом режиме
   (pipeline &), чтобы увидеть эффект.

   Вы также можете попробовать нажать Ctrl-C (^C), чтобы продемонстрировать,
   что терминальный драйвер интерпретирует это как сигнал для всех процессов
   в группе переднего плана.
*/

#define _GNU_SOURCE     /* Для получения объявления strsignal() из <string.h> */
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

static int cmdNum;  /* Номер команды в конвейере */

// Обработчик для различных сигналов
static void handler(int sig)
{
    /* НЕБЕЗОПАСНО: обработчик использует небезопасные для сигналов функции
       (fprintf(), strsignal(); см. Раздел 21.1.2) */

    if (getpid() == getpgrp())  // Если процесс является лидером группы
        fprintf(stderr, "Terminal FG process group: %ld\n",
                (long) tcgetpgrp(STDERR_FILENO));
    fprintf(stderr, "Process %ld (%d) received signal %d (%s)\n",
                (long) getpid(), cmdNum, sig, strsignal(sig));

    // Если поймали SIGTSTP, нужно явно остановить процесс
    if (sig == SIGTSTP)
        raise(SIGSTOP);
}

int main(int argc, char *argv[])
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
        errExit("sigaction");
    if (sigaction(SIGCONT, &sa, NULL) == -1)
        errExit("sigaction");

    // Если stdin является терминалом, это первый процесс в конвейере
    if (isatty(STDIN_FILENO)) {
        fprintf(stderr, "Terminal FG process group: %ld\n",
                (long) tcgetpgrp(STDIN_FILENO));
        fprintf(stderr, "Command   PID  PPID  PGRP   SID\n");
        cmdNum = 0;

    } else {  // Если не первый процесс, читаем номер команды из канала
        if (read(STDIN_FILENO, &cmdNum, sizeof(cmdNum)) <= 0)
            fatal("read got EOF or error");
    }

    cmdNum++;
    fprintf(stderr, "%4d    %5ld %5ld %5ld %5ld\n", cmdNum,
            (long) getpid(), (long) getppid(),
            (long) getpgrp(), (long) getsid(0));

    // Если это не последний процесс, передаем сообщение следующему
    if (!isatty(STDOUT_FILENO))  // Если не терминал, то должен быть канал
        if (write(STDOUT_FILENO, &cmdNum, sizeof(cmdNum)) == -1)
            errMsg("write");

    for (;;)  // Ожидание сигналов
        pause();
}

/*
 * Основное:
 * - Программа полезна для изучения управления заданиями и конвейеров shell.
 * - Сигналы SIGINT, SIGTSTP и SIGCONT обрабатываются для демонстрации взаимодействия процессов в группе.
 * - Для конвейера из нескольких процессов каждый последующий процесс получает номер команды через канал.
 * - ^C (SIGINT) демонстрирует отправку сигнала всем процессам в группе переднего плана.
 *
 * Рекомендуется использовать с конвейерами и в фоновом режиме для изучения поведения процессов.
 */

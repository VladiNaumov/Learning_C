/* orphaned_pgrp_SIGHUP.c

   Программа демонстрирует поведение системы, когда процессная группа становится
   осиротевшей. Используется для изучения отправки сигналов SIGHUP и SIGCONT
   осиротевшим процессным группам, содержащим остановленные процессы.

   Пример использования:
       orphaned_pgrp_SIGHUP s p
       orphaned_pgrp_SIGHUP p p
*/

#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include "tlpi_hdr.h"

/* Обработчик сигналов */
static void handler(int sig)
{
    printf("PID=%ld: caught signal %d (%s)\n", 
           (long) getpid(), sig, strsignal(sig));  /* НЕБЕЗОПАСНО */
}

int main(int argc, char *argv[])
{
    int j;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s {s|p} ...\n", argv[0]);

    setbuf(stdout, NULL);  /* Отмена буферизации stdout */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGHUP, &sa, NULL) == -1)
        errExit("sigaction");
    if (sigaction(SIGCONT, &sa, NULL) == -1)
        errExit("sigaction");

    printf("parent: PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n",
           (long) getpid(), (long) getppid(),
           (long) getpgrp(), (long) getsid(0));

    /* Создание дочерних процессов по количеству аргументов */
    for (j = 1; j < argc; j++) {
        switch (fork()) {
        case -1:
            errExit("fork");

        case 0:  /* Дочерний процесс */
            printf("child:  PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n",
                   (long) getpid(), (long) getppid(),
                   (long) getpgrp(), (long) getsid(0));

            if (argv[j][0] == 's') {  /* Остановка через сигнал */
                printf("PID=%ld stopping\n", (long) getpid());
                raise(SIGSTOP);
            } else {  /* Ожидание сигнала */
                alarm(60);  /* Таймер для завершения при отсутствии SIGHUP */
                printf("PID=%ld pausing\n", (long) getpid());
                pause();
            }

            _exit(EXIT_SUCCESS);

        default:  /* Родительский процесс продолжает */
            break;
        }
    }

    /* Родительский процесс завершает работу, создавая сиротскую группу */
    sleep(3);  /* Дать детям время на инициализацию */
    printf("parent exiting\n");
    exit(EXIT_SUCCESS);
}

/*
 * Основное:
 * - Родитель создает группу дочерних процессов.
 * - Родитель завершает работу, оставляя детей в осиротевшей процессной группе.
 * - Если в группе есть остановленные процессы, система отправляет SIGHUP
 *   и SIGCONT всем процессам в группе.
 * 
 * Попробуйте различные комбинации аргументов `s` и `p` для изучения поведения.
 */

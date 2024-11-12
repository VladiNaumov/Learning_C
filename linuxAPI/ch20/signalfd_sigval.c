/* signalfd_sigval.c

   Usage: signalfd_sigval sig-num...

   Демонстрация использования signalfd() для получения сигналов через файловый дескриптор.

   Эта программа специфична для Linux. API signalfd поддерживается с ядра
   2.6.22.
*/
#include <sys/signalfd.h>
#include <signal.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    sigset_t mask;
    int sfd, j;
    struct signalfd_siginfo fdsi;
    ssize_t s;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sig-num...\n", argv[0]);  // Показать ошибку использования, если нет аргументов

    printf("%s: PID = %ld\n", argv[0], (long) getpid());

    sigemptyset(&mask);  // Инициализация маски сигналов
    for (j = 1; j < argc; j++)
        sigaddset(&mask, atoi(argv[j]));  // Добавление сигналов из аргументов командной строки в маску

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)  // Блокируем указанные сигналы
        errExit("sigprocmask");

    sfd = signalfd(-1, &mask, 0);  // Создаём файловый дескриптор для обработки сигналов
    if (sfd == -1)
        errExit("signalfd");

    for (;;) {
        s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));  // Чтение сигнала из файлового дескриптора
        if (s != sizeof(struct signalfd_siginfo))  // Проверка на успешное чтение
            errExit("read");

        printf("%s: got signal %d", argv[0], fdsi.ssi_signo);  // Выводим номер полученного сигнала
        if (fdsi.ssi_code == SI_QUEUE) {  // Если сигнал был отправлен с помощью sigqueue
            printf("; ssi_pid = %d; ", fdsi.ssi_pid);  // Выводим PID процесса, отправившего сигнал
            printf("ssi_int = %d", fdsi.ssi_int);  // Выводим дополнительное целое значение
        }
        printf("\n");
    }
}

/*
 **Резюме**:
   - Эта программа использует Linux-specific API `signalfd` для получения сигналов через файловый дескриптор. 
   Она демонстрирует, как блокировать определённые сигналы и обрабатывать их с помощью чтения данных из файлового дескриптора, 
   который связан с сигнальными событиями. Программа непрерывно читает сигналы, и если сигнал был отправлен с помощью `sigqueue`, 
   дополнительно выводится информация о процессе, отправившем сигнал, и дополнительном целочисленном значении.
   */
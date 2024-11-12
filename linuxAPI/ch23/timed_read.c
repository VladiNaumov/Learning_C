/* timed_read.c

   Демонстрация использования таймера для установки таймаута на блокирующий системный вызов
   (в данном случае read(2)).
*/

#include <signal.h>
#include "tlpi_hdr.h"

#define BUF_SIZE 200

static void     /* Обработчик SIGALRM: прерывает блокирующий системный вызов */
handler(int sig) {
    printf("Caught signal\n");          /* НЕБЕЗОПАСНО (см. Раздел 21.1.2) */
}

int main(int argc, char *argv[]) {
    struct sigaction sa;
    char buf[BUF_SIZE];
    ssize_t numRead;
    int savedErrno;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [num-secs [restart-flag]]\n", argv[0]);

    /* Настройка обработчика для SIGALRM. Разрешение прерывать системные вызовы,
       если второй аргумент командной строки не был передан. */

    sa.sa_flags = (argc > 2) ? SA_RESTART : 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (sigaction(SIGALRM, &sa, NULL) == -1)
        errExit("sigaction");

    alarm((argc > 1) ? getInt(argv[1], GN_NONNEG, "num-secs") : 10);

    numRead = read(STDIN_FILENO, buf, BUF_SIZE);

    savedErrno = errno;                 /* Сохраняем errno на случай, если alarm() его изменит */
    alarm(0);                           /* Выключаем таймер */
    errno = savedErrno;

    /* Определение результата read() */

    if (numRead == -1) {
        if (errno == EINTR)
            printf("Read timed out\n");
        else
            errMsg("read");
    } else {
        printf("Successful read (%ld bytes): %.*s",
               (long) numRead, (int) numRead, buf);
    }

    exit(EXIT_SUCCESS);
}

/*
### Объяснение:
- `handler` — обработчик сигнала `SIGALRM`, вызываемый при истечении таймера.
- Таймер `alarm` посылает сигнал `SIGALRM` через заданное время, прерывая блокирующий вызов `read` в случае истечения времени.
- Если чтение прервано (ошибка `EINTR`), выводится сообщение о тайм-ауте. В случае успешного чтения данные отображаются.
*/
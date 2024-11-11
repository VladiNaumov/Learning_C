/* sig_sender.c

   Использование: sig_sender PID num-sigs sig [sig2]

   Отправка сигналов программе sig_receiver.c.

   Отправляет 'num-sigs' сигналов типа 'sig' процессу с указанным PID.
   Если передан четвертый аргумент командной строки, отправляется один сигнал
   этого типа после отправки предыдущих сигналов.
*/
#include <signal.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int numSigs, sig, j;
    pid_t pid;

    if (argc < 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pid num-sigs sig-num [sig-num-2]\n", argv[0]);

    pid = getLong(argv[1], 0, "PID");
    numSigs = getInt(argv[2], GN_GT_0, "num-sigs");
    sig = getInt(argv[3], 0, "sig-num");

    /* Отправка сигналов приемнику */

    printf("%s: отправка сигнала %d процессу %ld %d раз\n",
            argv[0], sig, (long) pid, numSigs);

    for (j = 0; j < numSigs; j++)
        if (kill(pid, sig) == -1)
            errExit("kill");

    /* Если указан четвертый аргумент командной строки, отправить этот сигнал */

    if (argc > 4)
        if (kill(pid, getInt(argv[4], 0, "sig-num-2")) == -1)
            errExit("kill");

    printf("%s: завершение работы\n", argv[0]);
    exit(EXIT_SUCCESS);
}
/*

### Резюме кода

Программа `sig_sender.c` отправляет сигналы процессу с указанным PID. 
В командной строке задается количество сигналов (`num-sigs`), которые отправляются в виде сигнала `sig`. 
Если передан четвертый аргумент, программа отправляет дополнительный сигнал типа `sig2` после основного числа сигналов. 
Для отправки сигналов используется функция `kill()`.
*/
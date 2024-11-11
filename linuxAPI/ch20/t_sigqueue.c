/* t_sigqueue.c

   Демонстрация использования sigqueue() для отправки (реального) сигнала.

   Usage: t_sigqueue sig pid data num-sigs

   Отправить 'num-sigs' экземпляров сигнала 'sig' (указан как целое число), с
   сопутствующими данными 'data' (целое число), процессу с PID 'pid'.
*/
#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int sig, numSigs, j, sigData;
    union sigval sv;

    if (argc < 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pid sig-num data [num-sigs]\n", argv[0]);

    /* Отображаем наш PID и UID, чтобы их можно было сравнить с
       соответствующими полями аргумента siginfo_t, который будет передан
       в обработчик в принимающем процессе */
    printf("%s: PID = %ld, UID = %ld\n", argv[0],
            (long) getpid(), (long) getuid());

    sig = getInt(argv[2], 0, "sig-num");  // Чтение номера сигнала
    sigData = getInt(argv[3], GN_ANY_BASE, "data");  // Чтение данных для сигнала
    numSigs = (argc > 4) ? getInt(argv[4], GN_GT_0, "num-sigs") : 1;  // Чтение количества сигналов

    /* Цикл отправки сигналов */
    for (j = 0; j < numSigs; j++) {
        sv.sival_int = sigData + j;  // Заполнение данных сигнала с увеличением на каждом шаге
        if (sigqueue(getLong(argv[1], 0, "pid"), sig, sv) == -1)  // Отправка сигнала с данными
            errExit("sigqueue %d", j);
    }

    exit(EXIT_SUCCESS);
}

/*
 **Резюме**:
   - Программа демонстрирует использование функции `sigqueue()` для отправки сигналов с дополнительными данными. 
   Она принимает аргументы командной строки, включая PID получателя, номер сигнала, данные и количество отправляемых сигналов. 
   Для каждого сигнала данные увеличиваются на 1, и сигнал отправляется через `sigqueue()`. 
   В программе также отображаются PID и UID отправляющего процесса, которые могут быть полезны для анализа в принимающем процессе.
   */
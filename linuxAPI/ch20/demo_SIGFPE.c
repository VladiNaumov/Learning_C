/* demo_SIGFPE.c

   Демонстрация генерации сигнала SIGFPE.

   Использование: demo_SIGFPE [optstr]

   Основная программа выполняет код, который вызывает сигнал SIGFPE. Перед этим
   программа при необходимости игнорирует и/или блокирует SIGFPE. Если строка 'optstr'
   содержит 'i', то SIGFPE игнорируется, иначе он обрабатывается обработчиком.
   Если 'optstr' содержит 'b', то SIGFPE блокируется перед его доставкой.
   Поведение, возникающее при генерации SIGFPE, зависит от версии ядра 
   (Linux 2.6 отличается от Linux 2.4 и более ранних версий).

   ПРИМЕЧАНИЕ: Не компилируйте эту программу с оптимизацией, так как нижеуказанная
   арифметическая операция может быть полностью оптимизирована, и SIGFPE не будет получен.
*/
#define _GNU_SOURCE     /* Для объявления strsignal() из <string.h> */
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include "tlpi_hdr.h"

static void
sigfpeCatcher(int sig)
{
    printf("Пойман сигнал %d (%s)\n", sig, strsignal(sig));
                                /* НЕБЕЗОПАСНО (см. раздел 21.1.2) */
    sleep(1);                   /* Замедление выполнения обработчика */
}

int
main(int argc, char *argv[])
{
    /* Если аргументы командной строки не заданы, ловить SIGFPE, иначе игнорировать его */

    if (argc > 1 && strchr(argv[1], 'i') != NULL) {
        printf("Игнорирование SIGFPE\n");
        if (signal(SIGFPE, SIG_IGN) == SIG_ERR)     errExit("signal");
    } else {
        printf("Обработка SIGFPE\n");

        struct sigaction sa;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = sigfpeCatcher;
        if (sigaction(SIGFPE, &sa, NULL) == -1)
            errExit("sigaction");
    }

    bool blocking = argc > 1 && strchr(argv[1], 'b') != NULL;
    sigset_t prevMask;
    if (blocking) {
        printf("Блокировка SIGFPE\n");

        sigset_t blockSet;
        sigemptyset(&blockSet);
        sigaddset(&blockSet, SIGFPE);
        if (sigprocmask(SIG_BLOCK, &blockSet, &prevMask) == -1)
            errExit("sigprocmask");
    }

    printf("Генерация SIGFPE\n");
    int x, y;
    y = 0;
    x = 1 / y;  // Данное выражение приводит к делению на ноль, вызывая SIGFPE
    y = x;      /* Предотвращение предупреждения "gcc -Wunused-but-set-variable" */

    if (blocking) {
        printf("Ожидание перед разблокировкой\n");
        sleep(2);
        printf("Разблокировка SIGFPE\n");
        if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
            errExit("sigprocmask");
    }

    printf("Этого сообщения не должно быть!\n");
    exit(EXIT_FAILURE);
}

/* 
### Дополнительный комментарий

    x = 1 / y;  // Данное выражение приводит к делению на ноль, вызывая SIGFPE


### Резюме коду

Программа демонстрирует генерацию и обработку сигнала **SIGFPE** (ошибка арифметики, такая как деление на ноль). 
В зависимости от аргументов командной строки она может игнорировать, обрабатывать или блокировать сигнал **SIGFPE**. 
Если программа вызвана с `i`, то **SIGFPE** игнорируется, если без `i` — перехватывается. Аргумент `b` блокирует сигнал, пока не будет разблокирован, 
что моделирует асинхронную обработку. Программа показывает, как установить обработчик сигнала с `sigaction`,
 как временно блокировать сигнал с `sigprocmask`, а также демонстрирует поведение при делении на ноль.
 
 */
/* sigmask_longjmp.c

   Демонстрация различных эффектов от longjmp() и siglongjmp()
   на маску сигналов процесса.

   По умолчанию эта программа использует setjmp() + longjmp().
   Для использования sigsetjmp() + siglongjmp() скомпилируйте с флагом -DUSE_SIGSETJMP.
*/
#define _GNU_SOURCE     /* Получаем объявление strsignal() из <string.h> */
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include "signal_functions.h"           /* Объявление функции printSigMask() */
#include "tlpi_hdr.h"

static volatile sig_atomic_t canJump = 0;
                        /* Устанавливается в 1, как только буфер "env" был
                           инициализирован с помощью [sig]setjmp() */
#ifdef USE_SIGSETJMP
static sigjmp_buf senv;
#else
static jmp_buf env;
#endif

static void
handler(int sig)
{
    /* НЕБЕЗОПАСНО: Этот обработчик использует функции, не безопасные для асинхронных сигналов
       (printf(), strsignal(), printSigMask(); см. Раздел 21.1.2) */

    printf("Получен сигнал %d (%s), маска сигналов:\n", sig,
            strsignal(sig));
    printSigMask(stdout, NULL);

    if (!canJump) {
        printf("Буфер 'env' ещё не установлен, выполняем простой возврат\n");
        return;
    }

#ifdef USE_SIGSETJMP
    siglongjmp(senv, 1);
#else
    longjmp(env, 1);
#endif
}

int
main(int argc, char *argv[])
{
    struct sigaction sa;

    printSigMask(stdout, "Маска сигналов при старте:\n");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

#ifdef USE_SIGSETJMP
    printf("Вызов sigsetjmp()\n");
    if (sigsetjmp(senv, 1) == 0)
#else
    printf("Вызов setjmp()\n");
    if (setjmp(env) == 0)
#endif
        canJump = 1;                    /* Выполняется после [sig]setjmp() */

    else                                /* Выполняется после [sig]longjmp() */
        printSigMask(stdout, "После возврата из обработчика, маска сигналов:\n" );

    for (;;)                            /* Ожидаем сигналы до завершения */
        pause();
}
/*

### Резюме:
Этот код демонстрирует использование `longjmp()` и `siglongjmp()` для управления контекстом выполнения программы и влияния этих функций на маску сигналов.

1. **Основная логика**:
   - Используется обработчик для перехвата сигнала SIGINT. В обработчике выводится информация о сигнале и текущей маске сигналов.
   - При получении сигнала и после инициализации буфера (через `sigsetjmp()` или `setjmp()`), программа использует `siglongjmp()` или `longjmp()` для возврата в точку, где была выполнена установка контекста.

2. **Маска сигналов**:
   - Программа выводит маску сигналов на старте и после того, как выполнен возврат через `siglongjmp()` или `longjmp()`.
   - В отличие от обычного `longjmp()`, `siglongjmp()` восстанавливает не только состояние программы, но и маску сигналов.

3. **Использование флага `USE_SIGSETJMP`**:
   - Если компиляция происходит с флагом `-DUSE_SIGSETJMP`, используется `sigsetjmp()` и `siglongjmp()`, которые поддерживают более сложное управление сигналами, чем стандартные `setjmp()` и `longjmp()`.

### Важные моменты:
- Этот код демонстрирует влияние разных методов управления контекстом на маску сигналов. Программы, использующие `siglongjmp()`, могут вернуть управление с учетом более сложных состояний сигналов, чем при использовании обычного `longjmp()`.
- Некоторые функции (например, `printf()` и `strsignal()`) не являются безопасными для вызова в обработчиках сигналов, и их использование в данном контексте может привести к проблемам.
*/
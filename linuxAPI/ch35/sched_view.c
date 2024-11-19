/* sched_view.c

   Описание:
   Программа отображает текущую политику планирования и приоритет для указанных PID.

   Использование:
   - Передайте PID процессов через аргументы командной строки.
   - Пример: `./sched_view 1234 5678`

   Примечания:
   - Политики: SCHED_OTHER, SCHED_RR, SCHED_FIFO, SCHED_BATCH (Linux), SCHED_IDLE (Linux).
   - Вывод: PID, политика, приоритет.
*/

#include <sched.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int j, pol;
    struct sched_param sp;

    for (j = 1; j < argc; j++) {
        /* Получение текущей политики планирования для PID */
        pol = sched_getscheduler(getLong(argv[j], 0, "pid"));
        if (pol == -1)
            errExit("sched_getscheduler");

        /* Получение текущего приоритета планирования для PID */
        if (sched_getparam(getLong(argv[j], 0, "pid"), &sp) == -1)
            errExit("sched_getparam");

        /* Вывод информации: PID, политика, приоритет */
        printf("%s: %-5s ", argv[j],
                (pol == SCHED_OTHER) ? "OTHER" :
                (pol == SCHED_RR) ? "RR" :
                (pol == SCHED_FIFO) ? "FIFO" :
#ifdef SCHED_BATCH              /* Linux-specific */
                (pol == SCHED_BATCH) ? "BATCH" :
#endif
#ifdef SCHED_IDLE               /* Linux-specific */
                (pol == SCHED_IDLE) ? "IDLE" :
#endif
                "???");
        printf("%2d\n", sp.sched_priority);
    }

    exit(EXIT_SUCCESS);
}
/*

### Объяснение:
1. **`sched_getscheduler(pid)`**:
   - Возвращает текущую политику планирования для указанного процесса (`pid`).
   - Возможные значения:
     - `SCHED_OTHER` — стандартная политика.
     - `SCHED_RR` — round-robin.
     - `SCHED_FIFO` — first-in, first-out.
     - `SCHED_BATCH` и `SCHED_IDLE` — специфичные для Linux.
2. **`sched_getparam(pid, &sp)`**:
   - Заполняет структуру `sched_param`, содержащую текущий приоритет (`sched_priority`) процесса.
3. **Вывод**:
   - PID процесса.
   - Название политики планирования.
   - Текущий приоритет.

### Пример запуска:

$ ./sched_view 1234 5678
1234: OTHER  0
5678: RR     10

*/
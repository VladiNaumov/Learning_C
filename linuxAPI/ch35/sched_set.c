/* sched_set.c

   Программа для установки политики планирования и приоритета процессов.

   Использование: sched_set policy priority pid...

   Аргументы:
     - policy: Политика планирования. Может быть:
         'r' (RR - Round Robin),
         'f' (FIFO - First In First Out),
         'o' (OTHER - стандартная),
         'b' (BATCH - партия, Linux-специфичная, начиная с ядра 2.6.16),
         'i' (IDLE - простой, Linux-специфичная, начиная с ядра 2.6.23).
     - priority: Приоритет планирования (целое число).
     - pid...: Один или несколько идентификаторов процессов (PID).

   Пример использования:
     ./sched_set r 10 1234 5678
     Устанавливает политику RR с приоритетом 10 для процессов с PID 1234 и 5678.
*/

#include <sched.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int j, pol;                  /* Политика планирования */
    struct sched_param sp;       /* Структура для хранения приоритета */

    /* Проверка корректности аргументов */
    if (argc < 3 || strchr("rfo"
#ifdef SCHED_BATCH              /* Linux-специфичная поддержка SCHED_BATCH */
                "b"
#endif
#ifdef SCHED_IDLE               /* Linux-специфичная поддержка SCHED_IDLE */
                "i"
#endif
                , argv[1][0]) == NULL)
        usageErr("%s policy priority [pid...]\n"
                 "    policy is 'r' (RR), 'f' (FIFO), "
#ifdef SCHED_BATCH
                 "'b' (BATCH), "
#endif
#ifdef SCHED_IDLE
                 "'i' (IDLE), "
#endif
                 "or 'o' (OTHER)\n",
                 argv[0]);

    /* Определение политики на основе аргумента */
    pol = (argv[1][0] == 'r') ? SCHED_RR :
          (argv[1][0] == 'f') ? SCHED_FIFO :
#ifdef SCHED_BATCH
          (argv[1][0] == 'b') ? SCHED_BATCH :
#endif
#ifdef SCHED_IDLE
          (argv[1][0] == 'i') ? SCHED_IDLE :
#endif
          SCHED_OTHER;

    /* Установка приоритета */
    sp.sched_priority = getInt(argv[2], 0, "priority");

    /* Обработка списка процессов */
    for (j = 3; j < argc; j++) {
        pid_t pid = getLong(argv[j], 0, "pid");
        if (sched_setscheduler(pid, pol, &sp) == -1)
            errExit("sched_setscheduler");
    }

    exit(EXIT_SUCCESS);
}

/*

### Краткое объяснение:
1. **`sched_setscheduler`**: Позволяет задать политику планирования и приоритет указанного процесса. 
2. **Политики**:
   - **SCHED_RR**: Круговое обслуживание (подходит для задач реального времени).
   - **SCHED_FIFO**: Обслуживание в порядке очереди.
   - **SCHED_OTHER**: Основная политика по умолчанию.
   - **SCHED_BATCH**: Для фоновых задач с малым приоритетом.
   - **SCHED_IDLE**: Для задач, выполняемых только в режиме простоя.
3. **Входные данные**:
   - Первые два аргумента задают политику и приоритет.
   - Остальные аргументы — это PID процессов, для которых будут применены изменения.

### Примечание:
- Для политики `SCHED_OTHER` приоритет всегда равен 0 и игнорируется.

*/
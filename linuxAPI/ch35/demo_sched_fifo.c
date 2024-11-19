/* demo_sched_fifo.c

   Демонстрация использования политики реального времени SCHED_FIFO.
   Программа создает два процесса, которые используют эту политику.

   Описание:
   - Каждый процесс выводит сообщение каждые 0.25 секунды CPU-времени.
   - После 1 секунды CPU-времени процесс вызывает `sched_yield()`, чтобы уступить CPU другому процессу.
   - Оба процесса завершаются после использования 3 секунд CPU-времени.

   Примечания:
   - Требуется запуск от суперпользователя или настройка RLIMIT_RTPRIO.
   - Оба процесса привязаны к одному CPU для предотвращения параллельной работы.
*/

#define _GNU_SOURCE
#include <sched.h>
#include <sys/resource.h>
#include <sys/times.h>
#include "tlpi_hdr.h"

#define CSEC_STEP 25  /* Шаг вывода сообщений в CPU-центрисекундах */

static void useCPU(char *msg)
{
    int prevStep = 0;
    int prevSec = 0;

    for (;;) {
        struct tms tms;
        if (times(&tms) == -1)
            errExit("times");

        int cpuCentisecs = (tms.tms_utime + tms.tms_stime) * 100 / sysconf(_SC_CLK_TCK);

        if (cpuCentisecs >= prevStep + CSEC_STEP) {
            prevStep += CSEC_STEP;
            printf("%s (PID %ld) cpu=%0.2f\n", msg, (long)getpid(), cpuCentisecs / 100.0);
        }

        if (cpuCentisecs > 300) /* Завершение через 3 секунды CPU-времени */
            break;

        if (cpuCentisecs >= prevSec + 100) { /* Уступить CPU раз в секунду */
            prevSec = cpuCentisecs;
            sched_yield();
        }
    }
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL); /* Отключение буферизации stdout */

    /* Привязка процессов к одному CPU */
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(1, &set);

    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
        errExit("sched_setaffinity");

    /* Ограничение CPU-времени (50 секунд) */
    struct rlimit rlim;
    rlim.rlim_cur = rlim.rlim_max = 50;
    if (setrlimit(RLIMIT_CPU, &rlim) == -1)
        errExit("setrlimit");

    /* Установка минимального приоритета для SCHED_FIFO */
    struct sched_param sp;
    sp.sched_priority = sched_get_priority_min(SCHED_FIFO);
    if (sp.sched_priority == -1)
        errExit("sched_get_priority_min");

    if (sched_setscheduler(0, SCHED_FIFO, &sp) == -1)
        errExit("sched_setscheduler");

    /* Создание дочернего процесса */
    switch (fork()) {
    case -1:
        errExit("fork");

    case 0: /* Дочерний процесс */
        useCPU("child ");
        exit(EXIT_SUCCESS);

    default: /* Родительский процесс */
        useCPU("parent");
        exit(EXIT_SUCCESS);
    }
}

/*

### Объяснение:
1. **`sched_setaffinity`**:
   - Привязывает процесс к конкретному CPU, предотвращая параллельное выполнение на нескольких ядрах.
2. **`RLIMIT_CPU`**:
   - Ограничивает общее CPU-время, доступное процессу. Это защита от бесконечных циклов в реальном времени.
3. **`sched_yield`**:
   - Процесс добровольно уступает CPU, позволяя другому процессу выполнять свои задачи.
4. **`sched_get_priority_min`**:
   - Получает минимальный приоритет для политики SCHED_FIFO. SCHED_FIFO работает без вытеснения: процесс занимает CPU, пока не завершится или не вызовет `sched_yield`.

### Особенности:
- Запускать программу нужно с привилегиями суперпользователя.
- Программа подходит для демонстрации поведения политики SCHED_FIFO.

*/
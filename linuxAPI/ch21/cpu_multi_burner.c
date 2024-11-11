/* cpu_multi_burner.c

   Использование: cpu_multi_burner period...

   Программа создает один дочерний процесс на каждый аргумент командной строки.
   Каждый процесс зацикливается, потребляя CPU, и, после 'period' секунд, 
   сообщает свой process ID и процент использования CPU с момента последнего отчета.

   Для некоторых экспериментов можно зафиксировать все процессы на одном CPU
   с помощью taskset(1). Например:

        taskset 0x1 ./cpu_multi_burner 2 2

   См. также cpu_multithread_burner.c.
*/

#include <time.h>
#include "tlpi_hdr.h"

#define NANO 1000000000L // Количество наносекунд в одной секунде

/* 
 * Функция для вычисления разницы между двумя значениями timespec в наносекундах.
 * Параметры:
 *   - a: начальное время
 *   - b: конечное время
 * Возвращает:
 *   Разницу в наносекундах между временем b и временем a.
 */
static long timespecDiff(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) * NANO + b.tv_nsec - a.tv_nsec;
}

/* 
 * Функция, зацикливающая процесс для потребления CPU и 
 * отображающая процент использования CPU каждую 'period' секунду.
 * Параметры:
 *   - period: интервал в секундах, через который выводится информация
 */
static void burnCPU(float period) {
    long step_size = NANO * period;
    long prev_step = 0;

    struct timespec base_real;
    if (clock_gettime(CLOCK_REALTIME, &base_real) == -1)
        errExit("clock_gettime");

    struct timespec prev_real = base_real;
    struct timespec prev_cpu;
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &prev_cpu) == -1)
        errExit("clock_gettime");

    while (1) {
        struct timespec curr_real;
        if (clock_gettime(CLOCK_REALTIME, &curr_real) == -1)
            errExit("clock_gettime");

        long elapsed_real_nsec = timespecDiff(base_real, curr_real);
        long elapsed_real_steps = elapsed_real_nsec / step_size;

        if (elapsed_real_steps > prev_step) {
            struct timespec curr_cpu;
            if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &curr_cpu) == -1)
                errExit("clock_gettime");

            long diff_real_nsec = timespecDiff(prev_real, curr_real);
            long diff_cpu_nsec = timespecDiff(prev_cpu, curr_cpu);

            printf("%ld", (long) getpid());
            printf("  [t=%.2f (delta: %.2f)]",
                    (double) elapsed_real_nsec / NANO,
                    (double) diff_real_nsec / NANO);
            printf("  %%CPU = %5.2f",
                    (double) diff_cpu_nsec / diff_real_nsec * 100);
            /*
            printf("   totCPU = %ld.%03ld",
                    (long) curr_cpu.tv_sec, curr_cpu.tv_nsec / 1000000);
            */
            printf("\n");

            prev_cpu = curr_cpu;
            prev_real = curr_real;
            prev_step = elapsed_real_steps;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s [period]...\n"
                 "Создает процесс на каждый переданный аргумент, выводящий "
                 "потребление CPU каждые 'period' секунд.\n"
                 "'period' может быть числом с плавающей запятой\n", argv[0]);

    int nproc = argc - 1;

    /* Создание дочерних процессов для каждого аргумента командной строки */
    for (int j = 0; j < nproc; j++) {
        float period;

        switch (fork()) {
        case 0:
            sscanf(argv[j + 1], "%f", &period);
            burnCPU(period);
            exit(EXIT_SUCCESS);  /* Завершение дочернего процесса */

        case -1:
            errExit("fork");

        default:
            break;
        }
    }

    pause(); // Ожидание завершения дочерних процессов

    exit(EXIT_SUCCESS);
}
/*
### Комментарии:
- `burnCPU` зацикливает процесс, рассчитывая и выводя процент использования CPU каждые `period` секунд.
- `timespecDiff` возвращает разницу во времени в наносекундах между двумя значениями `timespec`.
- `main` создает дочерний процесс для каждого аргумента `period`, затем основной процесс остается в состоянии ожидания, используя `pause`.
*/
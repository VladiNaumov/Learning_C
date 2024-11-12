/* cpu_multithread_burner.c

   Использование: cpu_multithread_burner period...

   Программа создает один поток на каждый аргумент командной строки. Каждый поток
   зацикливается, потребляя CPU, и, после 'period' секунд, сообщает свой
   thread ID и процент использования CPU с момента последнего отчета.

   Для некоторых экспериментов можно зафиксировать все потоки на одном CPU
   с помощью taskset(1). Например:

        taskset 0x1 ./cpu_multithread_burner 2 2

   См. также cpu_multi_burner.c.
*/

#define _GNU_SOURCE
#include <sys/syscall.h>
#include <sched.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "tlpi_hdr.h"

#if !defined(__GLIBC__) || __GLIBC_MINOR__ < 30

/* Обходной путь для glibc, где отсутствует gettid() обертка */

static pid_t gettid(void) {
    return syscall(SYS_gettid);
}
#endif

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
 * Функция потока, которая зацикливается и потребляет CPU.
 * Параметры:
 *   - arg: период в секундах для вывода статистики
 * Возвращает:
 *   NULL.
 */
static void *threadFunc(void *arg) {
    float period;
    char *sarg = arg;
    sscanf(sarg, "%f", &period);

    long step_size = NANO * period;
    long prev_step = 0;

    struct timespec base_real;
    if (clock_gettime(CLOCK_REALTIME, &base_real) == -1)
        errExit("clock_gettime");

    struct timespec prev_real = base_real;
    struct timespec prev_cpu;
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &prev_cpu) == -1)
        errExit("clock_gettime");

    while (1) {
        struct timespec curr_real;
        if (clock_gettime(CLOCK_REALTIME, &curr_real) == -1)
            errExit("clock_gettime");

        long elapsed_real_nsec = timespecDiff(base_real, curr_real);
        long elapsed_real_steps = elapsed_real_nsec / step_size;

        if (elapsed_real_steps > prev_step) {
            struct timespec curr_cpu;
            if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &curr_cpu) == -1)
                errExit("clock_gettime");

            long diff_real_nsec = timespecDiff(prev_real, curr_real);
            long diff_cpu_nsec = timespecDiff(prev_cpu, curr_cpu);

            printf("%ld  [t=%.2f (delta: %.2f)]   %%CPU = %5.2f\n",
                    (long) gettid(),
                    (double) elapsed_real_nsec / NANO,
                    (double) diff_real_nsec / NANO,
                    (double) diff_cpu_nsec / diff_real_nsec * 100);

            prev_cpu = curr_cpu;
            prev_real = curr_real;
            prev_step = elapsed_real_steps;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thr;
    int j, s;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s [period]...\n"
                 "Создает поток на каждый переданный аргумент, выводящий "
                 "потребление CPU каждые 'period' секунд.\n"
                 "'period' может быть числом с плавающей запятой\n", argv[0]);

    /* Создание потоков для каждого аргумента командной строки */
    for (j = 1; j < argc; j++) {
        s = pthread_create(&thr, NULL, threadFunc, argv[j]);
        if (s != 0)
            errExitEN(s, "pthread_create");
    }

    pause();

    exit(EXIT_SUCCESS);
}

/*
### Комментарии:
- `threadFunc` создает цикл для каждого потока, который расходует CPU и каждые `period` секунд выводит процент его использования.
- `timespecDiff` возвращает разницу между двумя значениями `timespec` в наносекундах, чтобы упрощать расчет времени.
- В `main` создается по одному потоку для каждого аргумента `period`.
*/
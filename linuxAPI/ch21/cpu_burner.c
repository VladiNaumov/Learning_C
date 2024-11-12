/* cpu_burner.c

   Программа, которая потребляет CPU-время, отображая процент использования
   CPU каждую секунду.
*/

#include <time.h>
#include "tlpi_hdr.h"

#define NANO 1000000000L // Количество наносекунд в одной секунде

/* 
 * Функция для вычисления разницы между двумя значениями типа timespec.
 * Параметры:
 *   - a: начальное время
 *   - b: конечное время
 * Возвращает:
 *   Разницу в наносекундах между временем b и временем a.
 */
static long timespecDiff(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) * NANO + b.tv_nsec - a.tv_nsec;
}

int main(int argc, char *argv[]) {
    struct timespec curr_real, prev_real, prev_cpu;

    // Получение начальных значений реального времени и CPU-времени процесса
    if (clock_gettime(CLOCK_REALTIME, &prev_real) == -1)
        errExit("clock_gettime");
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &prev_cpu) == -1)
        errExit("clock_gettime");

    int cnt = 0; // Счетчик итераций

    /* Бесконечный цикл, потребляющий CPU-время */
    while (1) {
        // Получение текущего реального времени
        if (clock_gettime(CLOCK_REALTIME, &curr_real) == -1)
            errExit("clock_gettime");

        // Вычисление прошедшего времени в наносекундах
        long elapsed_real_nsec = timespecDiff(prev_real, curr_real);

        /* Когда прошла секунда, выводим процент использования CPU за интервал */
        if (elapsed_real_nsec >= NANO) {
            struct timespec curr_cpu;

            // Получение текущего CPU-времени процесса
            if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &curr_cpu) == -1)
                errExit("clock_gettime");

            // Вычисление прошедшего CPU-времени в наносекундах
            long elapsed_cpu_nsec = timespecDiff(prev_cpu, curr_cpu);

            // Вывод информации о проценте использования CPU
            printf("[%ld]  %%CPU = %5.2f (%d)\n", (long)getpid(),
                    (double)elapsed_cpu_nsec / elapsed_real_nsec * 100.0,
                    cnt);

            // Обновление предыдущих значений времени
            prev_real = curr_real;
            prev_cpu = curr_cpu;
            cnt++;
        }
    }

    exit(EXIT_SUCCESS);
}

/*
### Комментарии:
- Основная часть кода организована в бесконечном цикле, где каждую секунду вычисляется и выводится процентное использование CPU.
- Функция `timespecDiff` облегчает вычисление разницы во времени, возвращая её в наносекундах.
*/

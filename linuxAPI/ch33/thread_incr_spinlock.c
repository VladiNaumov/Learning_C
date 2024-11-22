/* thread_incr_spinlock.c

   Пример использования двух POSIX-потоков, которые инкрементируют одну
   глобальную переменную, синхронизируя доступ с помощью spinlock.
*/

#include <pthread.h>
#include "tlpi_hdr.h"

/* Глобальная переменная, разделяемая между потоками */
static volatile int glob = 0;

/* Spinlock для синхронизации */
static pthread_spinlock_t splock;

/*
 * threadFunc: Функция потока.
 * 
 * Аргумент: количество итераций (int).
 * В каждой итерации поток захватывает spinlock, обновляет glob и
 * освобождает spinlock.
 */
static void *threadFunc(void *arg) {
    int loops = *((int *) arg);
    int loc, s;

    for (int j = 0; j < loops; j++) {
        s = pthread_spin_lock(&splock); // Захват spinlock
        if (s != 0)
            errExitEN(s, "pthread_spin_lock");

        loc = glob;
        loc++;
        glob = loc; // Обновление глобальной переменной

        s = pthread_spin_unlock(&splock); // Освобождение spinlock
        if (s != 0)
            errExitEN(s, "pthread_spin_unlock");
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

    /* Инициализация spinlock */
    int s = pthread_spin_init(&splock, 0);
    if (s != 0)
        errExitEN(s, "pthread_spin_init");

    pthread_t t1, t2;

    /* Создание потоков */
    s = pthread_create(&t1, NULL, threadFunc, &loops);
    if (s != 0)
        errExitEN(s, "pthread_create");
    s = pthread_create(&t2, NULL, threadFunc, &loops);
    if (s != 0)
        errExitEN(s, "pthread_create");

    /* Ожидание завершения потоков */
    s = pthread_join(t1, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");
    s = pthread_join(t2, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    printf("glob = %d\n", glob); // Ожидаемое значение: loops * 2
    exit(EXIT_SUCCESS);
}

/*
 * Описание:
 * 
 * - Глобальная переменная `glob` инкрементируется двумя потоками.
 * - Для синхронизации используется `pthread_spinlock_t`.
 * - Spinlock отличается от мьютекса тем, что поток активно "ждет"
 *   освобождения блокировки, постоянно проверяя ее состояние.
 * 
 * Особенности:
 * - Spinlock эффективен в системах с высокой конкуренцией, но его использование
 *   может привести к избыточному потреблению процессорного времени.
 * - Подходит для сценариев, где ожидается короткий период удержания блокировки.
 * 
 * Практическое применение:
 * - Spinlock может быть полезен в приложениях реального времени, где мьютексы
 *   могут приводить к ненужным переключениям контекста.
 */

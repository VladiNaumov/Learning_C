/* thread_incr_rwlock.c

   Программа демонстрирует использование двух POSIX-потоков, которые инкрементируют
   одну глобальную переменную с синхронизацией доступа через read/write-блокировку.
*/

#include <pthread.h>
#include "tlpi_hdr.h"

/* Глобальная переменная, разделяемая между потоками */
static volatile int glob = 0;

/* Read/Write-блокировка для синхронизации */
static pthread_rwlock_t rwlock;

/*
 * threadFunc: Функция потока.
 * 
 * Аргумент: количество итераций (int).
 * В каждой итерации поток захватывает блокировку на запись,
 * увеличивает значение glob и освобождает блокировку.
 */
static void *threadFunc(void *arg) {
    int loops = *((int *) arg);
    int loc, s;

    for (int j = 0; j < loops; j++) {
        s = pthread_rwlock_wrlock(&rwlock); // Захват блокировки на запись
        if (s != 0)
            errExitEN(s, "pthread_rwlock_wrlock");

        loc = glob;
        loc++;
        glob = loc; // Обновление глобальной переменной

        s = pthread_rwlock_unlock(&rwlock); // Освобождение блокировки
        if (s != 0)
            errExitEN(s, "pthread_rwlock_unlock");
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

    /* Инициализация read/write-блокировки */
    int s = pthread_rwlock_init(&rwlock, NULL);
    if (s != 0)
        errExitEN(s, "pthread_rwlock_init");

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
 * - Используется `pthread_rwlock_t` для синхронизации доступа.
 * - Потоки захватывают блокировку на запись (`pthread_rwlock_wrlock`) для
 *   исключительного доступа.
 * 
 * Особенности:
 * - Read/Write-блокировки удобны, если требуется частый доступ для чтения
 *   и редкий доступ для записи.
 * 
 * Практическое применение:
 * - Такой подход полезен в приложениях, где данные читаются значительно
 *   чаще, чем изменяются.
 */

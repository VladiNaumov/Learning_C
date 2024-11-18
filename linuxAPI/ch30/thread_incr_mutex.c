/* thread_incr_mutex.c

   Эта программа использует два потока POSIX для увеличения одной и той же
   глобальной переменной, синхронизируя доступ к ней с помощью мьютекса.
   Благодаря этому обновления переменной не теряются.
   Сравните с thread_incr.c, thread_incr_spinlock.c и thread_incr_rwlock.c.
*/

#include <pthread.h>
#include "tlpi_hdr.h"

/* Глобальная переменная, которую увеличивают потоки */
static volatile int glob = 0;

/* Мьютекс для синхронизации доступа */
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

/* Функция, выполняемая потоком: увеличивает глобальную переменную 'glob' */
static void *
threadFunc(void *arg)
{
    int loops = *((int *) arg); // Количество циклов
    int loc, j, s;

    for (j = 0; j < loops; j++) {
        /* Блокируем мьютекс перед доступом к глобальной переменной */
        s = pthread_mutex_lock(&mtx);
        if (s != 0)
            errExitEN(s, "pthread_mutex_lock");

        loc = glob; // Читаем текущее значение глобальной переменной
        loc++;
        glob = loc; // Обновляем значение

        /* Освобождаем мьютекс после обновления */
        s = pthread_mutex_unlock(&mtx);
        if (s != 0)
            errExitEN(s, "pthread_mutex_unlock");
    }

    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t t1, t2; // Идентификаторы потоков
    int loops, s;

    /* Получаем количество итераций (по умолчанию 10 000 000) */
    loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

    /* Создаём первый поток */
    s = pthread_create(&t1, NULL, threadFunc, &loops);
    if (s != 0)
        errExitEN(s, "pthread_create");

    /* Создаём второй поток */
    s = pthread_create(&t2, NULL, threadFunc, &loops);
    if (s != 0)
        errExitEN(s, "pthread_create");

    /* Ожидаем завершения первого потока */
    s = pthread_join(t1, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    /* Ожидаем завершения второго потока */
    s = pthread_join(t2, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    /* Выводим значение глобальной переменной */
    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}

/*
Резюме:
Эта программа демонстрирует использование мьютекса для синхронизации доступа к
глобальной переменной между двумя потоками.

Основные моменты:
1. Два потока параллельно увеличивают одну и ту же глобальную переменную.
2. Мьютекс гарантирует, что доступ к переменной происходит атомарно, предотвращая
   потерю обновлений.
3. Количество итераций задаётся аргументом командной строки (по умолчанию 10 миллионов).
4. Результат всегда корректен, так как потоки синхронизированы.
*/
```
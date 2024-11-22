/* pthread_barrier_demo.c

   Демонстрация использования API барьеров POSIX.

   Использование: pthread_barrier_demo num-barriers num-threads

   Программа создаёт 'num-threads' потоков, каждый из которых проходит 
   через барьер 'num-barriers' раз.
*/

#include <pthread.h>
#include "tlpi_hdr.h"

/* Глобальная переменная для барьера */
static pthread_barrier_t barrier;

/* Количество проходов через барьер */
static int numBarriers;

/* Функция, выполняемая потоками */
static void *
threadFunc(void *arg)
{
    long threadNum = (long) arg; // Номер текущего потока

    printf("Thread %ld started\n", threadNum);

    /* Инициализация генератора случайных чисел для каждого потока */
    srandom(time(NULL) + threadNum);

    /* Основной цикл работы потока */
    for (int j = 0; j < numBarriers; j++) {
        int nsecs = random() % 5 + 1; // Генерация случайного времени ожидания (1–5 секунд)
        sleep(nsecs); // Симуляция работы потока

        /* Поток ждёт на барьере */
        printf("Thread %ld about to wait on barrier %d "
               "after sleeping %d seconds\n", threadNum, j, nsecs);
        int s = pthread_barrier_wait(&barrier);

        /* Проверка результата вызова pthread_barrier_wait */
        if (s == 0) {
            printf("Thread %ld passed barrier %d: return value was 0\n",
                   threadNum, j);
        } else if (s == PTHREAD_BARRIER_SERIAL_THREAD) {
            printf("Thread %ld passed barrier %d: return value was "
                   "PTHREAD_BARRIER_SERIAL_THREAD\n", threadNum, j);
            usleep(100000); // Задержка для разделения вывода
            printf("\n");
        } else {
            errExitEN(s, "pthread_barrier_wait (%ld)", threadNum);
        }
    }

    usleep(200000); // Небольшая задержка перед завершением
    printf("Thread %ld terminating\n", threadNum);

    return NULL;
}

int
main(int argc, char *argv[])
{
    /* Проверка аргументов командной строки */
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-barriers num-threads\n", argv[0]);

    numBarriers = atoi(argv[1]); // Количество проходов через барьер
    int numThreads = atoi(argv[2]); // Количество потоков

    /* Выделение памяти под идентификаторы потоков */
    pthread_t *tid = calloc(numThreads, sizeof(pthread_t));
    if (tid == NULL)
        errExit("calloc");

    /* Инициализация барьера */
    int s = pthread_barrier_init(&barrier, NULL, numThreads);
    if (s != 0)
        errExitEN(s, "pthread_barrier_init");

    /* Создание потоков */
    for (long threadNum = 0; threadNum < numThreads; threadNum++) {
        s = pthread_create(&tid[threadNum], NULL, threadFunc, (void *) threadNum);
        if (s != 0)
            errExitEN(s, "pthread_create");
    }

    usleep(100000); // Задержка для оформления вывода
    printf("\n");

    /* Ожидание завершения потоков */
    for (int threadNum = 0; threadNum < numThreads; threadNum++) {
        s = pthread_join(tid[threadNum], NULL);
        if (s != 0)
            errExitEN(s, "pthread_join");
    }

    /* Очистка ресурсов барьера */
    s = pthread_barrier_destroy(&barrier);
    if (s != 0)
        errExitEN(s, "pthread_barrier_destroy");

    exit(EXIT_SUCCESS);
}

/*
Резюме:
Эта программа демонстрирует использование барьеров POSIX для синхронизации потоков.

Основные моменты:
1. Барьер (`pthread_barrier_t`) используется для синхронизации заданного числа потоков.
2. Потоки ждут на барьере, пока все потоки не достигнут этой точки.
3. Функция `pthread_barrier_wait()` возвращает `PTHREAD_BARRIER_SERIAL_THREAD` для одного из потоков, что можно использовать для выполнения уникальных задач.
4. Программа иллюстрирует, как потоки могут работать параллельно, синхронизируя свои действия через барьер.
*/
```
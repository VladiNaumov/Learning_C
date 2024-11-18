/* thread_multijoin.c

   Эта программа создаёт один поток для каждого аргумента командной строки.
   Каждый поток "работает" заданное количество секунд (указанное в аргументе),
   после чего завершается.

   Основные задачи программы:
   - Создать массив, где хранится информация о каждом потоке (ID, состояние, время сна).
   - Потоки сигнализируют главному потоку через условную переменную 'threadDied'
     о своём завершении.
   - Главный поток ожидает завершения всех потоков, используя эту переменную.
   - После завершения всех потоков программа завершает свою работу.
*/

#include <pthread.h>
#include "tlpi_hdr.h"

/* Глобальные переменные */
static int totThreads = 0;      // Общее количество потоков
static int numLive = 0;         // Количество живых потоков или завершённых, но не "собранных"
static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER; // Условная переменная
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER; // Мьютекс для синхронизации
static int numUnjoined = 0;     // Количество завершённых, но не "собранных" потоков

/* Состояния потоков */
enum tstate {                   
    TS_ALIVE,                   // Поток жив
    TS_TERMINATED,              // Поток завершён, но ещё не "собран"
    TS_JOINED                   // Поток завершён и "собран"
};

/* Информация о каждом потоке */
static struct {
    pthread_t tid;              // ID потока
    enum tstate state;          // Состояние потока
    int sleepTime;              // Время сна перед завершением
} *thread;

/* Функция, выполняемая потоком */
static void *
threadFunc(void *arg)
{
    int idx = (int) arg;  // Индекс потока в массиве
    int s;

    sleep(thread[idx].sleepTime);       // Имитируем "работу"
    printf("Thread %d terminating\n", idx);

    /* Обновляем состояние потока и сигнализируем главному потоку */
    s = pthread_mutex_lock(&threadMutex);
    if (s != 0)
        errExitEN(s, "pthread_mutex_lock");

    numUnjoined++;
    thread[idx].state = TS_TERMINATED;

    s = pthread_mutex_unlock(&threadMutex);
    if (s != 0)
        errExitEN(s, "pthread_mutex_unlock");
    s = pthread_cond_signal(&threadDied);
    if (s != 0)
        errExitEN(s, "pthread_cond_signal");

    return NULL;
}

/* Главная функция */
int
main(int argc, char *argv[])
{
    int s, idx;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-secs...\n", argv[0]);

    /* Создаём массив для хранения информации о потоках */
    thread = calloc(argc - 1, sizeof(*thread));
    if (thread == NULL)
        errExit("calloc");

    /* Создаём потоки */
    for (idx = 0; idx < argc - 1; idx++) {
        thread[idx].sleepTime = getInt(argv[idx + 1], GN_NONNEG, NULL);
        thread[idx].state = TS_ALIVE;
        s = pthread_create(&thread[idx].tid, NULL, threadFunc, (void *) idx);
        if (s != 0)
            errExitEN(s, "pthread_create");
    }

    totThreads = argc - 1;
    numLive = totThreads;

    /* Главный поток ждёт завершения всех потоков */
    while (numLive > 0) {
        s = pthread_mutex_lock(&threadMutex);
        if (s != 0)
            errExitEN(s, "pthread_mutex_lock");

        while (numUnjoined == 0) {  // Ждём сигналов о завершении потоков
            s = pthread_cond_wait(&threadDied, &threadMutex);
            if (s != 0)
                errExitEN(s, "pthread_cond_wait");
        }

        /* Собираем завершённые потоки */
        for (idx = 0; idx < totThreads; idx++) {
            if (thread[idx].state == TS_TERMINATED) {
                s = pthread_join(thread[idx].tid, NULL);
                if (s != 0)
                    errExitEN(s, "pthread_join");

                thread[idx].state = TS_JOINED;
                numLive--;
                numUnjoined--;

                printf("Reaped thread %d (numLive=%d)\n", idx, numLive);
            }
        }

        s = pthread_mutex_unlock(&threadMutex);
        if (s != 0)
            errExitEN(s, "pthread_mutex_unlock");
    }

    exit(EXIT_SUCCESS);
}

/*
Резюме:
1. Программа создаёт потоки, каждый из которых "работает" заданное время.
2. Информация о потоках хранится в глобальном массиве (ID, состояние, время сна).
3. Потоки сообщают главному потоку о своём завершении через условную переменную.
4. Главный поток "собирает" завершённые потоки с помощью pthread_join().
5. Программа завершает выполнение, когда все потоки завершены и собраны.
*/
```
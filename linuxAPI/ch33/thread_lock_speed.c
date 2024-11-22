/* thread_lock_speed.c

   Эта программа использует POSIX потоки для инкрементирования одной и той же глобальной
   переменной, синхронизируя их доступ с помощью мьютекса или spinlock.
   Аргументы командной строки позволяют пользователю указать:

   * Количество потоков, которые будут инкрементировать глобальную переменную.
   * Количество "внешних циклов", которые каждый поток выполнит, используя
     API lock/unlock (мьютексы или spinlock).
   * Количество "внутренних циклов", выполняемых для каждого шага "внешнего цикла".
     Каждая итерация внутреннего цикла инкрементирует глобальную переменную на 1.

   По умолчанию потоки используют мьютексы для синхронизации их доступа к глобальной переменной.
   Указание опции "-s" вызывает использование spinlock вместо мьютексов.

   Идея программы - изменить количество потоков и количество внутренних циклов, 
   измеряя время, затраченное программой (реальное и CPU-время) с помощью команды time(1). 
   В некоторых случаях (например, много потоков, большие значения "внутреннего цикла")
   мьютексы будут работать лучше, в то время как в других случаях (мало потоков, маленькие значения
   "внутреннего цикла") spinlock, вероятно, покажет лучшие результаты.
*/

#include <pthread.h>
#include <stdbool.h>
#include "tlpi_hdr.h"

// Глобальная переменная, которая инкрементируется потоками
static volatile int glob = 0;

// Мьютекс и spinlock для синхронизации потоков
static pthread_spinlock_t splock;
static pthread_mutex_t mtx;

// Переменная для выбора типа блокировки (мьютекс или spinlock)
static bool useMutex;

// Количество внешних и внутренних циклов
static int numOuterLoops;
static int numInnerLoops;

/*
 * Функция потока, которая выполняет инкрементирование глобальной переменной
 * с использованием либо мьютекса, либо spinlock в зависимости от параметра useMutex.
 */
static void *threadFunc(void *arg)
{
    int s;

    // Внешний цикл, выполняемый каждый поток
    for (int j = 0; j < numOuterLoops; j++) {
        // Выбор типа блокировки
        if (useMutex) {
            s = pthread_mutex_lock(&mtx);  // Блокировка с использованием мьютекса
            if (s != 0)
                errExitEN(s, "pthread_mutex_lock");
        } else {
            s = pthread_spin_lock(&splock);  // Блокировка с использованием spinlock
            if (s != 0)
                errExitEN(s, "pthread_spin_lock");
        }

        // Внутренний цикл, инкрементирующий глобальную переменную
        for (int k = 0; k < numInnerLoops; k++)
            glob++;

        // Освобождение блокировки
        if (useMutex) {
            s = pthread_mutex_unlock(&mtx);
            if (s != 0)
                errExitEN(s, "pthread_mutex_unlock");
        } else {
            s = pthread_spin_unlock(&splock);
            if (s != 0)
                errExitEN(s, "pthread_spin_unlock");
        }
    }

    return NULL;
}

/*
 * Функция для отображения ошибок использования программы
 */
static void usageError(char *pname)
{
    fprintf(stderr,
            "Usage: %s [-s] num-threads "
            "[num-inner-loops [num-outer-loops]]\n", pname);
    fprintf(stderr,
            "    -q   Don't print verbose messages\n");
    fprintf(stderr,
            "    -s   Use spin locks (instead of the default mutexes)\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    /* Предотвращаем бесконечное выполнение программы, если она зависнет */
    alarm(120);         /* Необработанный сигнал SIGALRM убьет процесс */

    useMutex = true;    // По умолчанию используем мьютексы
    bool verbose = true; // По умолчанию выводим подробные сообщения
    int opt;

    // Обработка опций командной строки
    while ((opt = getopt(argc, argv, "qs")) != -1) {
        switch (opt) {
        case 'q':   // Отключение подробных сообщений
            verbose = false;
            break;
        case 's':   // Использование spinlock вместо мьютексов
            useMutex = false;
            break;
        default:
            usageError(argv[0]);
        }
    }

    if (optind >= argc)
        usageError(argv[0]);

    // Извлечение количества потоков и циклов из аргументов
    int numThreads = atoi(argv[optind]);
    numInnerLoops = (optind + 1 < argc) ? atoi(argv[optind + 1]) : 1;
    numOuterLoops = (optind + 2 < argc) ? atoi(argv[optind + 2]) : 10000000;

    // Вывод конфигурации, если включен режим verbose
    if (verbose) {
        printf("Using %s\n", useMutex ? "mutexes" : "spin locks");
        printf("\tthreads: %d; outer loops: %d; inner loops: %d\n",
                numThreads, numOuterLoops, numInnerLoops);
    }

    // Выделение памяти для массивов потоков
    pthread_t *thread = calloc(numThreads, sizeof(pthread_t));
    if (thread == NULL)
        errExit("calloc");

    int s;

    // Инициализация мьютекса или spinlock в зависимости от выбора
    if (useMutex) {
        s = pthread_mutex_init(&mtx, NULL);
        if (s != 0)
            errExitEN(s, "pthread_mutex_init");
    } else {
        s = pthread_spin_init(&splock, 0);
        if (s != 0)
            errExitEN(s, "pthread_spin_init");
    }

    // Создание потоков
    for (int j = 0; j < numThreads; j++) {
        s = pthread_create(&thread[j], NULL, threadFunc, NULL);
        if (s != 0)
            errExitEN(s, "pthread_create");
    }

    // Ожидание завершения всех потоков
    for (int j = 0; j < numThreads; j++) {
        s = pthread_join(thread[j], NULL);
        if (s != 0)
            errExitEN(s, "pthread_join");
    }

    // Вывод значения глобальной переменной, если включен режим verbose
    if (verbose)
        printf("glob = %d\n", glob);

    exit(EXIT_SUCCESS);
}

/*
 * Резюме:
 * 
 * - Эта программа создает несколько потоков, которые инкрементируют одну глобальную переменную.
 * - Для синхронизации доступа потоков используется либо мьютекс (по умолчанию), либо spinlock.
 * - Количество потоков, количество циклов и тип блокировки выбираются через аргументы командной строки.
 * - Время выполнения программы зависит от выбора синхронизации и параметров потока.
 * - Мьютексы эффективны в сценариях с высокой конкуренцией, в то время как spinlock может быть быстрее
 *   при меньшем числе потоков и коротких циклах.
 */

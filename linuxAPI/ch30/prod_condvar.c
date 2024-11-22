/* prod_condvar.c

   Пример использования условной переменной POSIX для синхронизации
   между потоками "производитель-потребитель".
*/

#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include "tlpi_hdr.h"

/* Глобальные переменные синхронизации */
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;  // Мьютекс для защиты доступа к `avail`
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   // Условная переменная

static int avail = 0;  // Количество доступных для потребления единиц

/* Функция производителя */
static void *producer(void *arg) {
    int cnt = atoi((char *) arg);  // Количество единиц, которые нужно произвести

    for (int j = 0; j < cnt; j++) {
        sleep(1);  // Имитация времени на производство

        /* Производство единицы (код упрощён) */

        int s = pthread_mutex_lock(&mtx);  // Блокировка мьютекса
        if (s != 0)
            errExitEN(s, "pthread_mutex_lock");

        avail++;  // Увеличиваем счётчик доступных единиц

        s = pthread_mutex_unlock(&mtx);  // Разблокировка мьютекса
        if (s != 0)
            errExitEN(s, "pthread_mutex_unlock");

        s = pthread_cond_signal(&cond);  // Уведомляем потребителя
        if (s != 0)
            errExitEN(s, "pthread_cond_signal");
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    time_t t = time(NULL);  // Время начала программы

    int totRequired = 0;  // Общее количество единиц, которые должны быть произведены

    /* Создание всех потоков производителей */
    for (int j = 1; j < argc; j++) {
        totRequired += atoi(argv[j]);  // Суммируем количество единиц от каждого производителя

        pthread_t tid;
        int s = pthread_create(&tid, NULL, producer, argv[j]);  // Создание потока
        if (s != 0)
            errExitEN(s, "pthread_create");
    }

    /* Цикл потребителя */
    int numConsumed = 0;  // Количество потреблённых единиц
    bool done = false;    // Флаг завершения работы

    for (;;) {
        int s = pthread_mutex_lock(&mtx);  // Блокировка мьютекса
        if (s != 0)
            errExitEN(s, "pthread_mutex_lock");

        while (avail == 0) {  // Ожидание доступных единиц
            s = pthread_cond_wait(&cond, &mtx);  // Освобождает мьютекс и ждёт уведомления
            if (s != 0)
                errExitEN(s, "pthread_cond_wait");
        }

        /* На этом этапе мьютекс снова заблокирован */

        while (avail > 0) {  // Потребление всех доступных единиц
            numConsumed++;
            avail--;
            printf("T=%ld: numConsumed=%d\n", (long) (time(NULL) - t), numConsumed);

            done = numConsumed >= totRequired;  // Проверяем, достигли ли цели
        }

        s = pthread_mutex_unlock(&mtx);  // Разблокировка мьютекса
        if (s != 0)
            errExitEN(s, "pthread_mutex_unlock");

        if (done)
            break;  // Завершаем цикл, если все единицы потреблены

        /* Здесь можно выполнять другую работу без блокировки мьютекса */
    }

    exit(EXIT_SUCCESS);  // Успешное завершение программы
}

/*
### Основные моменты:
1. **Мьютекс (`pthread_mutex_t`)** защищает доступ к общим данным (`avail`).
2. **Условная переменная (`pthread_cond_t`)** используется для уведомления потребителя, когда доступны новые единицы.
3. **`pthread_cond_wait()`**:
   - Освобождает мьютекс и переводит поток в состояние ожидания.
   - Возвращает управление, когда поток просыпается (например, после `pthread_cond_signal()`).
4. **Производители** увеличивают `avail` и уведомляют потребителя.
5. **Потребитель** потребляет все доступные единицы, пока они есть.

### Пример запуска:

./prod_condvar 3 2 4

Этот запуск создаст 3 потока, производящих соответственно 3, 2 и 4 единицы. Потребитель будет обрабатывать их по мере поступления.

*/
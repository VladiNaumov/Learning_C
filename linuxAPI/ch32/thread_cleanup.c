/* thread_cleanup.c

   Пример отмены потоков с использованием POSIX threads API.
   Демонстрируется использование pthread_cancel() и обработчиков очистки.
*/

#include <pthread.h>
#include "tlpi_hdr.h"

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // Условная переменная
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // Мьютекс для синхронизации
static int glob = 0;                    // Переменная условия

/* 
 * cleanupHandler: Функция обработчика очистки.
 * 
 * Освобождает память, переданную в 'arg', и снимает блокировку мьютекса.
 */
static void cleanupHandler(void *arg) {
    int s;

    printf("cleanup: освобождение памяти по адресу %p\n", arg);
    free(arg);

    printf("cleanup: разблокировка мьютекса\n");
    s = pthread_mutex_unlock(&mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_unlock");
}

/* 
 * threadFunc: Потоковая функция.
 * 
 * Ожидает условной переменной и освобождает ресурсы при отмене потока.
 */
static void *threadFunc(void *arg) {
    int s;
    void *buf = NULL; // Буфер, выделяемый потоком

    buf = malloc(0x10000); // Выделение памяти
    printf("thread: память выделена по адресу %p\n", buf);

    s = pthread_mutex_lock(&mtx); // Блокировка мьютекса
    if (s != 0)
        errExitEN(s, "pthread_mutex_lock");

    pthread_cleanup_push(cleanupHandler, buf);

    // Ожидание сигнала
    while (glob == 0) {
        s = pthread_cond_wait(&cond, &mtx); // Точка отмены
        if (s != 0)
            errExitEN(s, "pthread_cond_wait");
    }

    printf("thread: цикл ожидания завершен\n");
    pthread_cleanup_pop(1); // Выполняет обработчик очистки
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thr;
    void *res;
    int s;

    // Создаем поток
    s = pthread_create(&thr, NULL, threadFunc, NULL);
    if (s != 0)
        errExitEN(s, "pthread_create");

    sleep(2); // Даем потоку время для запуска

    if (argc == 1) { // Отмена потока
        printf("main: собираемся отменить поток\n");
        s = pthread_cancel(thr);
        if (s != 0)
            errExitEN(s, "pthread_cancel");
    } else { // Отправка сигнала условной переменной
        printf("main: собираемся послать сигнал условной переменной\n");

        s = pthread_mutex_lock(&mtx); // Блокируем мьютекс
        if (s != 0)
            errExitEN(s, "pthread_mutex_lock");

        glob = 1;

        s = pthread_mutex_unlock(&mtx); // Разблокируем мьютекс
        if (s != 0)
            errExitEN(s, "pthread_mutex_unlock");

        s = pthread_cond_signal(&cond); // Посылаем сигнал
        if (s != 0)
            errExitEN(s, "pthread_cond_signal");
    }

    // Ожидание завершения потока
    s = pthread_join(thr, &res);
    if (s != 0)
        errExitEN(s, "pthread_join");
    if (res == PTHREAD_CANCELED)
        printf("main: поток был отменен\n");
    else
        printf("main: поток завершился нормально\n");

    exit(EXIT_SUCCESS);
}

/*
 * Резюме:
 * 
 * Данный пример демонстрирует работу с потоками POSIX, их отмену и обработку ресурсов
 * при завершении. Поток выделяет память и ждет сигнала от условной переменной.
 * В случае отмены потока или завершения ожидания, обработчик очистки освобождает память
 * и разблокирует мьютекс.
 * 
 * Ключевые моменты:
 * 1. Использование pthread_cancel() для отмены потока.
 * 2. Обработчик очистки (pthread_cleanup_push/pop) для освобождения ресурсов.
 * 3. Ожидание сигнала через pthread_cond_wait().
 * 
 * Потокобезопасный подход и правильное управление ресурсами делают данный код
 * устойчивым к ошибкам и утечкам памяти.
 */

/* one_time_init.c

   Функция one_time_init(), реализованная здесь, выполняет ту же задачу,
   что и библиотечная функция POSIX pthread_once().

   Эта программа тестирует однократную инициализацию в многопоточной среде.
*/

#include <pthread.h>
#include "tlpi_hdr.h"

/* Структура, эквивалентная pthread_once_t */
struct once_struct {
    pthread_mutex_t mtx;  // Мьютекс для синхронизации
    int called;           // Флаг, указывающий, была ли уже вызвана функция
};

/* Инициализатор для структуры once_struct */
#define ONCE_INITIALIZER { PTHREAD_MUTEX_INITIALIZER, 0 }

/* Глобальная переменная для тестирования */
struct once_struct once = ONCE_INITIALIZER;

/* 
 * one_time_init: Функция для однократного вызова init() в многопоточной среде
 * 
 * Параметры:
 *   once_control - указатель на структуру управления
 *   init - функция, которую нужно вызвать однократно
 * 
 * Возвращает:
 *   0 при успешной работе
 */
static int one_time_init(struct once_struct *once_control, void (*init)(void)) {
    int s;

    /* Захватываем мьютекс, чтобы защитить критическую секцию */
    s = pthread_mutex_lock(&(once_control->mtx));
    if (s != 0)  // Ошибка захвата мьютекса
        errExitEN(s, "pthread_mutex_lock");

    /* Проверяем, была ли функция уже вызвана */
    if (!once_control->called) {
        (*init)();  // Вызываем функцию инициализации
        once_control->called = 1;  // Устанавливаем флаг вызова
    }

    /* Освобождаем мьютекс */
    s = pthread_mutex_unlock(&(once_control->mtx));
    if (s != 0)  // Ошибка освобождения мьютекса
        errExitEN(s, "pthread_mutex_unlock");

    return 0;
}

/* 
 * init_func: Тестовая функция для демонстрации
 * 
 * Эта функция выводит сообщение. Она должна быть вызвана только один раз.
 */
static void init_func() {
    /* Это сообщение должно быть выведено только один раз */
    printf("Called init_func()\n");
}

/* 
 * threadFunc: Функция для выполнения в потоках
 * 
 * Она вызывает one_time_init() дважды, чтобы проверить,
 * что init_func() вызывается только один раз.
 */
static void *threadFunc(void *arg) {
    one_time_init(&once, init_func);
    one_time_init(&once, init_func);  // Повторный вызов (не должен повторно инициализировать)
    return NULL;
}

/* 
 * main: Главная функция программы
 * 
 * Создаёт два потока, каждый из которых вызывает one_time_init().
 * Ждёт завершения потоков и завершает выполнение программы.
 */
int main(int argc, char *argv[]) {
    pthread_t t1, t2;
    int s;

    /* Создаём два потока, вызывающих one_time_init() */
    s = pthread_create(&t1, NULL, threadFunc, (void *)1);
    if (s != 0)
        errExitEN(s, "pthread_create");

    s = pthread_create(&t2, NULL, threadFunc, (void *)2);
    if (s != 0)
        errExitEN(s, "pthread_create");

    /* Ожидаем завершения потоков */
    s = pthread_join(t1, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");
    printf("First thread returned\n");

    s = pthread_join(t2, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");
    printf("Second thread returned\n");

    exit(EXIT_SUCCESS);
}

/* 
 * Резюме:
 * 
 * Данная программа реализует механизм однократной инициализации (аналог pthread_once).
 * Глобальная структура `once` используется для контроля вызова функции `init_func`.
 * Потоки безопасно синхронизируются с помощью мьютекса. В результате, функция `init_func`
 * вызывается только один раз, независимо от количества вызовов `one_time_init()` 
 * из разных потоков.
 */

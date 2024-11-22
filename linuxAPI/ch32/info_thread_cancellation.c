/* 32 Threads: Thread Cancellation */

#include <pthread.h>

/*
 * pthread_cancel - Отправляет запрос на отмену выполнения потока.
 * thread - Идентификатор потока, который нужно завершить.
 * Возвращает 0 при успешном запросе, ненулевое значение в случае ошибки.
 */
int pthread_cancel(pthread_t thread);

/*
 * pthread_setcancelstate - Устанавливает состояние отмены для вызывающего потока.
 * state - Новое состояние отмены (PTHREAD_CANCEL_ENABLE или PTHREAD_CANCEL_DISABLE).
 * oldstate - Указатель для сохранения предыдущего состояния отмены (может быть NULL).
 * Возвращает 0 при успехе, ненулевое значение в случае ошибки.
 */
int pthread_setcancelstate(int state, int *oldstate);

/*
 * pthread_setcanceltype - Устанавливает тип отмены для вызывающего потока.
 * type - Новый тип отмены (PTHREAD_CANCEL_DEFERRED или PTHREAD_CANCEL_ASYNCHRONOUS).
 * oldtype - Указатель для сохранения предыдущего типа отмены (может быть NULL).
 * Возвращает 0 при успехе, ненулевое значение в случае ошибки.
 */
int pthread_setcanceltype(int type, int *oldtype);

/*
 * pthread_testcancel - Проверяет наличие запроса на отмену.
 * Если запрос на отмену существует, поток завершается с кодом PTHREAD_CANCELED.
 * Не возвращает значения.
 */
void pthread_testcancel(void);

/*
 * pthread_cleanup_push - Добавляет обработчик очистки в стек очистки потока.
 * routine - Функция, которая будет выполнена при отмене потока.
 * arg - Аргумент, передаваемый в функцию обработки.
 * Не возвращает значения.
 */
void pthread_cleanup_push(void (*routine)(void *), void *arg);

/*
 * pthread_cleanup_pop - Удаляет обработчик очистки из стека очистки потока.
 * execute - Если ненулевое, обработчик выполняется с указанным аргументом.
 * Не возвращает значения.
 */
void pthread_cleanup_pop(int execute);


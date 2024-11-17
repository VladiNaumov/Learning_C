/* 29 Threads: Introduction */

/*
 * pthread_create - создает новый поток.
 * thread - указатель для хранения идентификатора созданного потока.
 * attr - указатель на атрибуты потока (можно указать NULL для использования стандартных настроек).
 * start - функция, которая будет выполняться в новом потоке.
 * arg - аргумент, передаваемый функции start.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
#include <pthread.h>
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start)(void *), void *arg);

/*
 * pthread_exit - завершает текущий поток.
 * retval - указатель на значение, которое поток возвращает вызывающему потоку.
 * Эта функция не возвращает значение.
 */
#include <pthread.h>
void pthread_exit(void *retval);

/*
 * pthread_self - возвращает идентификатор текущего потока.
 * Возвращает значение типа pthread_t.
 */
#include <pthread.h>
pthread_t pthread_self(void);

/*
 * pthread_equal - сравнивает два идентификатора потоков.
 * t1, t2 - идентификаторы потоков.
 * Возвращает ненулевое значение, если идентификаторы равны, иначе 0.
 */
#include <pthread.h>
int pthread_equal(pthread_t t1, pthread_t t2);

/*
 * pthread_join - ожидает завершения указанного потока.
 * thread - идентификатор потока, завершения которого нужно ожидать.
 * retval - указатель для получения значения, возвращенного потоком через pthread_exit (можно указать NULL).
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
#include <pthread.h>
int pthread_join(pthread_t thread, void **retval);

/*
 * pthread_detach - переводит поток в "отсоединенное" состояние.
 * thread - идентификатор потока.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
#include <pthread.h>
int pthread_detach(pthread_t thread);


/* 30 Threads: Thread Synchronization */

#include <pthread.h>
#include <time.h>

/* Мьютексы */

/*
 * pthread_mutex_lock - блокирует мьютекс.
 * mutex - указатель на мьютекс.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_mutex_lock(pthread_mutex_t *mutex);

/*
 * pthread_mutex_unlock - разблокирует мьютекс.
 * mutex - указатель на мьютекс.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_mutex_unlock(pthread_mutex_t *mutex);

/*
 * pthread_mutex_init - инициализирует мьютекс.
 * mutex - указатель на мьютекс.
 * attr - указатель на атрибуты мьютекса (NULL для стандартных настроек).
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

/*
 * pthread_mutex_destroy - уничтожает мьютекс.
 * mutex - указатель на мьютекс.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_mutex_destroy(pthread_mutex_t *mutex);

/* Условные переменные */

/*
 * pthread_cond_signal - разблокирует один поток, ожидающий на условной переменной.
 * cond - указатель на условную переменную.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_cond_signal(pthread_cond_t *cond);

/*
 * pthread_cond_broadcast - разблокирует все потоки, ожидающие на условной переменной.
 * cond - указатель на условную переменную.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_cond_broadcast(pthread_cond_t *cond);

/*
 * pthread_cond_wait - ожидает, пока условие станет истинным.
 * cond - указатель на условную переменную.
 * mutex - указатель на мьютекс, который должен быть заблокирован перед вызовом.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

/*
 * pthread_cond_timedwait - ждет на условной переменной с таймаутом.
 * cond - указатель на условную переменную.
 * mutex - указатель на мьютекс, который должен быть заблокирован перед вызовом.
 * abstime - указатель на структуру timespec, задающую абсолютное время ожидания.
 * Возвращает 0 при успехе, ETIMEDOUT при истечении времени, или другое ненулевое значение при ошибке.
 */
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);

/*
 * pthread_cond_init - инициализирует условную переменную.
 * cond - указатель на условную переменную.
 * attr - указатель на атрибуты условной переменной (NULL для стандартных настроек).
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);

/*
 * pthread_cond_destroy - уничтожает условную переменную.
 * cond - указатель на условную переменную.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_cond_destroy(pthread_cond_t *cond);

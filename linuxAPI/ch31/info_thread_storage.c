/* 31 Threads: Thread Safety and Per-Thread Storage */

#include <pthread.h>

/*
 * pthread_once - обеспечивает выполнение функции инициализации только один раз
 * на поток независимо от количества вызовов.
 * once_control - указатель на статическую переменную управления.
 * init - указатель на функцию инициализации.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_once(pthread_once_t *once_control, void (*init)(void));

/*
 * pthread_key_create - создает ключ для потокоспецифичных данных.
 * key - указатель на переменную для сохранения созданного ключа.
 * destructor - функция-деструктор, вызываемая при завершении потока.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));

/*
 * pthread_setspecific - устанавливает значение для ключа в текущем потоке.
 * key - ключ, созданный с помощью pthread_key_create.
 * value - указатель на данные, ассоциируемые с ключом.
 * Возвращает 0 при успехе, ненулевое значение при ошибке.
 */
int pthread_setspecific(pthread_key_t key, const void *value);

/*
 * pthread_getspecific - получает значение, ассоциированное с ключом, для текущего потока.
 * key - ключ, созданный с помощью pthread_key_create.
 * Возвращает указатель на данные или NULL, если значение не установлено.
 */
void *pthread_getspecific(pthread_key_t key);




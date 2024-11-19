/* strerror_tsd.c

   Реализация функции strerror(), которая является потокобезопасной
   благодаря использованию данных, специфичных для потока (TSD).

   Смотрите также strerror_tls.c.
*/

#define _GNU_SOURCE  /* Подключает '_sys_nerr' и '_sys_errlist' из <stdio.h> */
#include <stdio.h>
#include <string.h>   /* Подключает объявление strerror() */
#include <pthread.h>
#include "tlpi_hdr.h"

static pthread_once_t once = PTHREAD_ONCE_INIT;  /* Для однократной инициализации */
static pthread_key_t strerrorKey;               /* Ключ для данных, специфичных для потока */

#define MAX_ERROR_LEN 256  /* Максимальная длина строки, возвращаемой strerror() */

/* 
 * destructor: Освобождает буфер данных, специфичный для потока
 * 
 * Вызывается автоматически при завершении потока.
 */
static void destructor(void *buf) {
    free(buf);
}

/* 
 * createKey: Функция для создания ключа данных, специфичных для потока
 * 
 * Вызывается один раз для всех потоков.
 */
static void createKey(void) {
    int s;

    /* Создаём уникальный ключ и регистрируем деструктор для освобождения памяти */
    s = pthread_key_create(&strerrorKey, destructor);
    if (s != 0)
        errExitEN(s, "pthread_key_create");
}

/* 
 * strerror: Возвращает строку с описанием ошибки на основе её кода.
 * 
 * Потокобезопасная реализация благодаря использованию TSD.
 * 
 * Параметры:
 *   err - код ошибки.
 * 
 * Возвращает:
 *   Указатель на строку с описанием ошибки, специфичный для текущего потока.
 */
char *strerror(int err) {
    int s;
    char *buf;

    /* Убедимся, что ключ создан при первом вызове */
    s = pthread_once(&once, createKey);
    if (s != 0)
        errExitEN(s, "pthread_once");

    /* Получаем буфер, специфичный для текущего потока */
    buf = pthread_getspecific(strerrorKey);
    if (buf == NULL) {  /* Если первый вызов из данного потока, выделяем буфер */
        buf = malloc(MAX_ERROR_LEN);
        if (buf == NULL)
            errExit("malloc");

        /* Сохраняем указатель на буфер в TSD */
        s = pthread_setspecific(strerrorKey, buf);
        if (s != 0)
            errExitEN(s, "pthread_setspecific");
    }

    /* Формируем строку описания ошибки */
    if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL) {
        snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
    } else {
        strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
        buf[MAX_ERROR_LEN - 1] = '\0';  /* Гарантируем нуль-терминацию */
    }

    return buf;
}

/* 
 * Резюме:
 * 
 * Данная реализация strerror() является потокобезопасной благодаря использованию
 * данных, специфичных для потока (Thread-Specific Data, TSD). Для каждого потока
 * создаётся отдельный буфер, который автоматически освобождается при завершении
 * потока. Однократная инициализация ключа TSD выполняется с помощью pthread_once().
 * 
 * Потокобезопасность позволяет избежать конфликтов между потоками, которые вызывают
 * strerror() одновременно.
 */

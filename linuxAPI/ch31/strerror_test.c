/* strerror_test.c

   Программа для проверки потокобезопасности реализации strerror().
*/

#include <stdio.h>
#include <string.h>   /* Подключает объявление strerror() */
#include <pthread.h>
#include "tlpi_hdr.h"

/* 
 * threadFunc: Функция, выполняемая в потоке
 * 
 * Вызывает strerror() и выводит результат. 
 */
static void *threadFunc(void *arg) {
    char *str;

    printf("Other thread about to call strerror()\n");
    str = strerror(EPERM);  // Вызываем strerror() с кодом ошибки EPERM
    printf("Other thread: str (%p) = %s\n", str, str);

    return NULL;
}

/* 
 * main: Главная функция программы
 * 
 * Проверяет, изменяются ли результаты вызова strerror() из разных потоков.
 */
int main(int argc, char *argv[]) {
    pthread_t t;
    int s;
    char *str;

    /* Вызываем strerror() из основного потока */
    str = strerror(EINVAL);
    printf("Main thread has called strerror()\n");

    /* Создаём второй поток */
    s = pthread_create(&t, NULL, threadFunc, NULL);
    if (s != 0)
        errExitEN(s, "pthread_create");

    /* Ожидаем завершения второго потока */
    s = pthread_join(t, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    /* Проверяем, изменился ли результат strerror() в основном потоке */
    printf("Main thread:  str (%p) = %s\n", str, str);

    exit(EXIT_SUCCESS);
}

/* 
 * Резюме:
 * 
 * Эта программа проверяет потокобезопасность реализации strerror() путём её вызова
 * из двух разных потоков. Если strerror() использует общий статический буфер, результаты
 * её вызова могут перезаписываться. В этом случае строки, возвращённые в основном и 
 * другом потоке, будут идентичными, а указатели на строки совпадут. Это демонстрирует,
 * что реализация strerror() не потокобезопасна.
 */

/* 07. Memory Allocation*/

/*
brk() и sbrk() — это системные вызовы, используемые для управления размером сегмента данных (heap) процесса, позволяя ему увеличивать или уменьшать выделенную память.

brk(addr) — устанавливает новую границу сегмента данных процесса на адрес addr. Этот вызов изменяет размер области памяти, выделенной процессу, если это возможно. 
Если addr находится за пределами допустимой области, вызов может завершиться ошибкой.

sbrk(increment) — изменяет текущую границу сегмента данных на указанное значение increment. 
Если increment положительное, то граница сдвигается вперёд, увеличивая выделенную память; 
если отрицательное — назад, уменьшая выделение. Например, вызов sbrk(0) возвращает текущий адрес конца сегмента данных.

Обычно brk() и sbrk() используются на более низком уровне, тогда как библиотеки, такие как malloc(), оборачивают эти вызовы для удобного выделения памяти.
 */
 


/*
 * brk - устанавливает конец сегмента данных процесса.
 * end_data_segment - новый конец сегмента.
 * Возвращает 0 при успешном выполнении или -1 при ошибке.
 */
int brk(void *end_data_segment);

/*
 * sbrk - изменяет конец сегмента данных процесса.
 * increment - изменение в байтах.
 * Возвращает указатель на начало нового сегмента или (void*)-1 при ошибке.
 */
void *sbrk(intptr_t increment);

/*
 * malloc - выделяет память заданного размера.
 * size - количество байтов.
 * Возвращает указатель на выделенную память или NULL при ошибке.
 */
void *malloc(size_t size);

/*
 * free - освобождает ранее выделенную память.
 * ptr - указатель на память, выделенную malloc, calloc или realloc.
 */
void free(void *ptr);

/*
 * calloc - выделяет и инициализирует память.
 * numitems - количество элементов.
 * size - размер каждого элемента.
 * Возвращает указатель на выделенную память или NULL при ошибке.
 */
void *calloc(size_t numitems, size_t size);

/*
 * realloc - изменяет размер ранее выделенного блока памяти.
 * ptr - указатель на ранее выделенную память.
 * size - новый размер в байтах.
 * Возвращает указатель на новый блок или NULL при ошибке.
 */
void *realloc(void *ptr, size_t size);

/*
 * memalign - выделяет память с заданным выравниванием.
 * boundary - граница выравнивания (должна быть степенью двойки).
 * size - размер выделяемой памяти.
 * Возвращает указатель на выделенную память или NULL при ошибке.
 */
void *memalign(size_t boundary, size_t size);

/*
 * posix_memalign - выделяет память с заданным выравниванием.
 * memptr - указатель для возврата адреса выделенной памяти.
 * alignment - выравнивание (степень двойки, кратное sizeof(void*)).
 * size - размер выделяемой памяти.
 * Возвращает 0 при успешном выполнении или код ошибки.
 */
int posix_memalign(void **memptr, size_t alignment, size_t size);

/*
 * alloca - выделяет память в стеке.
 * size - размер выделяемой памяти.
 * Возвращает указатель на выделенную память.
 */
void *alloca(size_t size);


#include <unistd.h>
#include <stdio.h>

int main() {
    void *initial_brk = sbrk(0); // Получаем текущий конец сегмента данных
    printf("Initial program break: %p\n", initial_brk);

    if (sbrk(1024) == (void *) -1) { // Увеличиваем сегмент данных на 1024 байта
        perror("sbrk error");
    } else {
        printf("Increased program break by 1024 bytes.\n");
    }

    // Возвращаемся к изначальному положению
    if (brk(initial_brk) == -1) {
        perror("brk error");
    } else {
        printf("Program break reset to initial position.\n");
    }
    return 0;
}

/*
 * Пример использования malloc() и free():
 * malloc() позволяет выделить блок памяти, который можно освободить с помощью free().
 */

#include <stdlib.h>
#include <stdio.h>

int main() {
    int *arr = (int *)malloc(10 * sizeof(int)); // Выделяем память для 10 целых чисел
    if (!arr) {
        perror("malloc failed");
        return 1;
    }

    // Используем память, выделенную malloc
    for (int i = 0; i < 10; i++) {
        arr[i] = i * i;
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr); // Освобождаем память
    return 0;
}

/*
 * calloc() полезен, когда нужно выделить память и сразу инициализировать её нулями.
 */

#include <stdlib.h>
#include <stdio.h>

int main() {
    int *arr = (int *)calloc(10, sizeof(int)); // Выделяем память и инициализируем нулями
    if (!arr) {
        perror("calloc failed");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]); // Будет выводить все нули
    }
    printf("\n");

    free(arr); // Освобождаем память
    return 0;
}

/*
 * realloc() позволяет увеличить или уменьшить размер уже выделенного блока памяти,
 * при этом сохранится содержимое исходного блока до новой длины (в случае уменьшения)
 * или до конца исходного блока (в случае увеличения).
 */

#include <stdlib.h>
#include <stdio.h>

int main() {
    int *arr = (int *)malloc(5 * sizeof(int));
    if (!arr) {
        perror("malloc failed");
        return 1;
    }

    for (int i = 0; i < 5; i++) arr[i] = i + 1;

    // Увеличиваем размер блока
    arr = (int *)realloc(arr, 10 * sizeof(int));
    if (!arr) {
        perror("realloc failed");
        return 1;
    }

    for (int i = 5; i < 10; i++) arr[i] = i + 1;

    for (int i = 0; i < 10; i++) printf("%d ", arr[i]);
    printf("\n");

    free(arr);
    return 0;
}

/*
 * memalign() полезен для выделения памяти с заданным выравниванием, например,
 * когда адрес памяти должен быть кратен определённому числу, что иногда важно для SIMD-инструкций.
 */

#include <malloc.h>
#include <stdio.h>

int main() {
    void *ptr = memalign(16, 64); // Выделяем 64 байта с выравниванием на 16 байт
    if (!ptr) {
        perror("memalign failed");
        return 1;
    }

    printf("Allocated memory at address: %p\n", ptr);
    free(ptr); // Освобождаем выделенную память
    return 0;
}

/*
 * Использование alloca() полезно для быстрого выделения памяти на стеке.
 * Однако память, выделенная alloca(), автоматически освобождается,
 * когда функция возвращает значение, поэтому alloca() не рекомендуется
 * использовать для больших блоков памяти.
 */

#include <alloca.h>
#include <stdio.h>

int main() {
    int *arr = (int *)alloca(10 * sizeof(int)); // Выделяем 10 целых чисел на стеке
    for (int i = 0; i < 10; i++) arr[i] = i + 1;

    for (int i = 0; i < 10; i++) printf("%d ", arr[i]);
    printf("\n");

    // Нет необходимости в free(), alloca() освобождает память при выходе из функции
    return 0;
}


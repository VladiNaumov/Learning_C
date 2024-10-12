#include <stdio.h>
#include <stdlib.h>

// Определяем простую структуру для примера
struct my_struct {
    int id;
    char name[20];
};

// Макрос для выделения памяти под структуру
#define ALLOC_STRUCT() (struct my_struct *)malloc(sizeof(struct my_struct))

// Макрос для освобождения выделенной памяти
#define FREE_STRUCT(ptr) free(ptr)

// Макрос для получения указателя на структуру (в нашем случае просто возвращаем его)
#define GET_STRUCT(ptr) (ptr)

int main() {
    // Выделяем память для структуры my_struct с помощью макроса
    struct my_struct *my_instance = ALLOC_STRUCT();
    if (!my_instance) {
        printf("Не удалось выделить память для структуры.\n");
        return -1;
    }

    // Заполняем поля структуры
    my_instance->id = 1;
    snprintf(my_instance->name, sizeof(my_instance->name), "Учебный пример");

    // Получаем указатель на структуру (имитация получения thread_info)
    struct my_struct *retrieved_instance = GET_STRUCT(my_instance);
    if (retrieved_instance) {
        printf("ID: %d, Имя: %s\n", retrieved_instance->id, retrieved_instance->name);
    }

    // Освобождаем память
    FREE_STRUCT(my_instance);
    printf("Память для структуры освобождена.\n");

    return 0;
}

/*
Пояснение:
ALLOC_STRUCT: Этот макрос выделяет память для структуры my_struct с помощью стандартной функции malloc и приводит результат к указателю на my_struct.

FREE_STRUCT: Освобождает выделенную память с помощью функции free.

GET_STRUCT: Просто возвращает указатель на структуру (как аналог task_thread_info
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Создание массива структур Person на heap */

typedef struct person
{
    char* name;  // Имя человека
    int age;     // Возраст человека
} person;

// Функция для инициализации массива структур
void initialize_people(person arr[], const char* names[], const int ages[], int size)
{
    for (int i = 0; i < size; ++i) {
        // Выделение памяти для имени и копирование строки
        arr[i].name = (char*)malloc(strlen(names[i]) + 1);
        if (arr[i].name != NULL) {
            strcpy(arr[i].name, names[i]);
        } else {
            // Обработка ошибки выделения памяти
            fprintf(stderr, "Не удалось выделить память для name[%d]\n", i);
            arr[i].name = NULL; // Обработка ошибки
        }
        arr[i].age = ages[i];
    }
}

// Функция для вывода значений массива структур
void print_people(const person arr[], int size)
{
    for (int i = 0; i < size; ++i) {
        printf("Person[%d]: address=%p \t name=%s \t age=%d \n", i, (void*)&arr[i], arr[i].name, arr[i].age);
    }
}

// Функция для освобождения памяти, выделенной под имена
void free_people(person arr[], int size)
{
    for (int i = 0; i < size; ++i) {
        free(arr[i].name);
    }
}

// Основная функция, которая инициализирует и обрабатывает массив структур
int array()
{
    const char* names[] = {"Bob", "Lisp", "Kate", "Tom"};
    const int ages[] = {30, 25, 28, 22};
    int size = sizeof(names) / sizeof(names[0]);

    person arr[size];  // Массив структур Person

    initialize_people(arr, names, ages, size);  // Инициализация
    print_people(arr, size);  // Вывод

    free_people(arr, size);  // Освобождение памяти

    return 0;
}

int main(void)
{
    return array();
}

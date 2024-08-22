/*
2. Определение структуры в исходном файле
В исходном файле (.c) определяйте структуру и функции, которые с ней работают.
Это позволяет вам скрыть детали реализации от пользователя заголовочного файла.
*/

// mymodule.c
#include "mymodule.h"
#include <stdlib.h>

// Определение структуры
struct MyStruct {
    int value;
};

// Функция для создания и инициализации структуры
MyStruct* create_mystruct(int value) {
    MyStruct* ms = (MyStruct*)malloc(sizeof(MyStruct));
    if (ms != NULL) {
        ms->value = value;
    }
    return ms;
}

// Функция для освобождения памяти
void destroy_mystruct(MyStruct* ms) {
    free(ms);
}

// Функция для установки значения
void set_value(MyStruct* ms, int value) {
    if (ms != NULL) {
        ms->value = value;
    }
}

// Функция для получения значения
int get_value(const MyStruct* ms) {
    return (ms != NULL) ? ms->value : 0;
}

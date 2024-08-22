/*
1. Объявление только указателя на структуру
В заголовочном файле (.h) объявляйте тип структуры как typedef для указателя на структуру.
Это скрывает детали внутренней реализации структуры, оставляя только интерфейс для работы с объектами этого типа.

*/



// mymodule.h
#ifndef MYMODULE_H
#define MYMODULE_H

// Объявление типа структуры как указателя
typedef struct MyStruct MyStruct;

// Функции для работы со структурой
MyStruct* create_mystruct(int value);
void destroy_mystruct(MyStruct* ms);
void set_value(MyStruct* ms, int value);
int get_value(const MyStruct* ms);

#endif // MYMODULE_H

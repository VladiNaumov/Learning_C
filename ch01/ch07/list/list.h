// File name: ExtremeC_examples_chapter6_3.h
// Description: The header file which exposes public attributes
//              and behavior functions related to a list object.
//              As you see, the attribute structure does not
//              disclose any of its members.

#ifndef LIST_H
#define LIST_H

#include <unistd.h>

// структура атрибутов без публично доступных полей
struct list_t;

// функция выделения памяти
struct list_t* list_malloc();

// конструктор и деструктор
void list_init(struct list_t*);
void list_destroy(struct list_t*);

// Public behavior functions (публичные поведенческие функции)
int list_add(struct list_t*, int);
int list_get(struct list_t*, int, int*);
void list_clear(struct list_t*);
size_t list_size(struct list_t*);
void list_print(struct list_t*);

#endif


/*
Здесь вы можете	видеть, как	мы делаем атрибуты приватными. Если другой исходный	файл
(например, содержащий функцию main) подключит данный заголовок,то не будет иметь доступ	к атрибутам	внутри типа	list_t.
Это	легко объяснить:
list_t — просто	объявление без определения, которое	не позволяет обращаться к полям	структуры.
Вы даже	не сможете объявить	переменную на основе данного типа.
Таким образом, мы соблюдаем	принцип	сокрытия.
*/
#ifndef NODE_H
#define NODE_H

#include <stdlib.h>

struct Node;

// указатель на первый элемент списка. Вначале списка нет и указатель ссылается на NULL.
struct Node *head;

// добавления нового узла
void push(struct Node **head, int data);

// она удаляет элемент, на который указывает head и возвращает его значение.
int pop(struct Node **head);

//перебор всех элементов списка
struct Node* getNth(struct Node* head, int n);

// Для нахождение последнего элемента
struct Node* getLast(struct Node *head);

// pushBack (её ещё принято называть shift или enqueue), которая добавляет новый элемент в конец списка
void pushBack(struct Node *head, int value);

// popBack (unshift, или dequeue), которая удаляет последний элемент списка и возвращает его значение.
int popBack(struct Node **head);

// функцию insert, которая вставляет на n-е место новое значение.
void insert(struct Node *head, unsigned n, int val);

// Функция удаления элемента списка
int deleteNth(struct Node **head, int n);

// удаление
void deleteList(struct Node **head);

//  Первая - создать список из массива.
void fromArray(struct Node **head, int *arr, size_t size);

// И обратная функция, которая возвратит массив элементов, хранящихся в списке.
int* toArray(struct Node *head);

// функция, которая будет печатать содержимое списка
void printLinkedList(struct Node *head);


#endif

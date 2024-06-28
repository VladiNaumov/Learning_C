#include <stdio.h>
#include <stdlib.h>

// Ссылка структуры на саму себя
typedef struct node
{
    char* value;          // значение
    struct node* next;    // указатель на следующий узел

}node;


// указатель на первый элемент списка. Вначале списка нет и указатель ссылается на NULL.
struct node* head = NULL;

/* Для добавления нового узла необходимо

1) Выделить под него память.
2) Задать ему значение
3) Сделать так, чтобы он ссылался на предыдущий элемент (или на NULL, если его не было)
4) Перекинуть указатель head на новый узел.

*/


// Allocates memory for a node object (выделяет память для объекта)
void push(node** head, char* data) {
    node* tmp = (node*) malloc(sizeof(node));
    tmp->value = data;
    tmp->next = (*head);
    (*head) = tmp;
}



// Destructor of a node object
void node_destroy(node* pointer)
{
    // Deallocates the allocated memory
    free(pointer->next);
    free(pointer->value);
}


void iterator(node* pointer)
{

    while(pointer != NULL)
    {
        printf("address=%p \t name=%s \t address_next=%p \n", pointer, pointer->value, pointer->next);
        pointer = pointer->next;     // переходим к следующему объекту
    }

}





#include <stdio.h>
#include <stdlib.h>

// Ссылка структуры на саму себя
typedef struct
{
    char* value;          // значение
    struct node* next;    // указатель на следующий узел

}node;

node arr[100];

// Allocates memory for a node object (выделяет память для объекта)
node* node_malloc()
{
    return (node*)malloc(sizeof(node));
}

// Constructor of a node object
void node_new(node* pointer, char* value)
{
    /* не поддерживается стандартом ISO C99 */
    // strcopy(node->value, name);

    pointer->value = value;
    pointer->next = NULL;

}

// Destructor of a node object
void node_destroy(node* pointer)
{
    // Deallocates the allocated memory
    free(pointer->next);
    free(pointer->value);
}

void getAddress(node* pointer)
{

    printf("address=%p \t name=%s \t address_next=%p \n", pointer, pointer->value, pointer->next);
}

void node_linc(node* pointer, void* p)
{
    /* не поддерживается стандартом ISO C99 */
    // strcopy(node->next, (void*) p);
    pointer->next = (void*) p;
}

void iterator(node* pointer)
{

    while(pointer != NULL)
    {
        printf("address=%p \t name=%s \t address_next=%p \n", pointer, pointer->value, pointer->next);
        pointer = pointer->next;     // переходим к следующему объекту
    }

}

void addArray_(node* pointer, int var)
{
        arr[var] = *pointer;

}

void getArr(int num)
{

    for (int var = 0; var < num; ++var) {

        void* address = arr + var;
        node next = *(arr + var);
        node value = *(arr + var);

        printf("array[%d]: address=%p \t name=%s \t address_next=%p \n", var, address, next, value);

    }

}


#include <stdio.h>

// Ссылка структуры на саму себя
typedef struct
{
    char* value;          // значение
    struct node* next;    // указатель на следующий узел

}node;

void pointer(){
    // создание экземпляра  структуры

    node bob = {"Bob", NULL};
    node lisp = {"Lisp", NULL};
    node kate = {"Kate", NULL};
    node tom = {"Tom", NULL};

    // устанавливаем указатель на экземпляр структуры
    node* p1 = &bob;
    p1->next = &lisp;

    node* p2 = &lisp;
    p2->next = &kate;

    node* p3 = &kate;
    p3->next = &tom;

    node* p4 = &tom;
    p4->next = NULL;


    printf("Variable: address=%p \t name=%s \t address=%p \n", (void*) p1,  *p1, p1->next);
    printf("Variable: address=%p \t name=%s \t address=%p \n", (void*) p2,  *p2, p2->next);
    printf("Variable: address=%p \t name=%s \t address=%p \n", (void*) p3,  *p3, p3->next);
    printf("Variable: address=%p \t name=%s \t address=%p \n", (void*) p4,  *p4, p4->next);


}

int main(void)
{
    pointer();

}
#include <stdio.h>

// Ссылка структуры на саму себя
typedef struct
{
    char* value;          // значение
    struct node* next;    // указатель на следующий узел

}node;


void pointer(){
    // создание экземпляра  структуры
    node bob;
    node lisp;
    node kate;
    node tom;

    // устанавливаем указатель на экземпляр структуры
    node *p1 = &bob;
    node *p2 = &lisp;
    node *p3 = &kate;
    node *p4 = &tom;


    printf("Variable: address=%p \t name=%s \t address=%p \n", (void*) p1,  *p1, p1->next);
    printf("Variable: address=%p \t name=%s \t address=%p \n", (void*) p2,  *p2, p2->next);
    printf("Variable: address=%p \t name=%s \t address=%p \n", (void*) p3,  *p3, p3->next);
    printf("Variable: address=%p \t name=%s \t address=%p \n", (void*) p4,  *p4, p4->next);


}

int main(void)
{
pointer();

}
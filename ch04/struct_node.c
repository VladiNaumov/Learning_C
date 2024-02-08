#include <stdio.h>

// Ссылка структуры на саму себя
typedef struct
{
    char* value;          // значение
    struct node* next;    // указатель на следующий узел

}node;

void write_node(node* node, char* p, char* name)
{
    node->value = name;
    node->next = (void*) p;

}

void pointer(){

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


int p_node(){

    node bob;
    node lisp;
    node kate;
    node tom;


    write_node(&bob, &lisp, "Bob");
    write_node(&lisp, &kate, "Lisp");
    write_node(&kate, &tom, "Kate");
    write_node(&tom, 0, "Tim");


    // устанавливаем указатель на первую структуру в цепочке
    node *pointer = &bob;
    while(pointer != NULL)
    {
        printf("value = %s \n", pointer->value);
        pointer = pointer->next;     // переходим к следующему объекту
    }

    return 0;

}

int main(void)
{

    p_node();


}

/*
Здесь определяем три переменных структуры: kate, tom и bob. У объекта kate элемент next указывает на объект tom:


kate.next = &tom;
А у объекта tom элемент next указывает на объект bob:


tom.next = &bob;
Таким образом, фактически мы получим цепочку Kate - Tom - Bob или грубо говоря список объектов структуры node.

Используя эти указатели, мы можем перемещаться вперед по этому списку. Для этого сначала устанавливаем указатель на первую структуру:

struct node * pointer = &kate;
Далее в цикле while пока указатель pointer не будет указать ни на какой объект структуры (то есть будет равен NULL) выводим значение value текущего объекта, на который указывает pointer, и затем присваиваем ему значение указателя pointer->next, то есть переходим к следуюшей структуре в списке:

while(pointer != NULL)
{
    printf("value = %s \n", pointer->value);
    pointer = pointer->next;
}
Таким образом, можно определять связанные наборы объектов, не прибегая к массивам.

*/
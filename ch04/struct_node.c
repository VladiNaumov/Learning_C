#include <stdio.h>

// Ссылка структуры на саму себя
typedef struct
{
    char* value;          // значение
    struct node* next;    // указатель на следующий узел

}node;


void node_constructor(node* node, char* p, char* name)
{
    /* не поддерживается стандартом ISO C99 */
    // strcopy(node->value, name);
    // strcopy(node->next, (void*) p);

    node->value = name;
    node->next = (void*) p;

}


int p_node(){

    // создание экземпляра структуры
    node bob;
    node lisp;
    node kate;
    node tom;

    node_constructor(&bob, &lisp, "Bob");
    node_constructor(&lisp, &kate, "Lisp");
    node_constructor(&kate, &tom, "Kate");
    node_constructor(&tom, 0, "Tim");

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

Далее в цикле while пока указатель pointer не будет указать ни на какой объект структуры (то есть будет равен NULL)
выводим значение value текущего объекта, на который указывает pointer, и затем присваиваем ему значение указателя pointer->next,
то есть переходим к следуюшей структуре в списке:

while(pointer != NULL)
{
    printf("value = %s \n", pointer->value);
    pointer = pointer->next;
}
Таким образом, можно определять связанные наборы объектов, не прибегая к массивам.

*/
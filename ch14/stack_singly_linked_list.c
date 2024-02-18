#include <stdio.h>
#include <stdlib.h>

/*
 * PUSH – кладёт элемент на вершину стека
 * POP – снимает элемент с вершины стека, перемещая вершину к следующему элементу */

/* Реализация стека на односвязном списке */

//Никакого максимального и минимального размеров у нас не будет (хотя в общем случае может быть). Каждый новый элемент создаётся заново. Для начала определим структуру узел

#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY   -102

typedef int T;

typedef struct Node_tag {
    T value;
    struct Node_tag *next;
} Node_t;

//Функция вставки первого элемента проста: создаём новый узел. Указатель next кидаем на старый узел. Далее указатель на вершину стека перекидываем на вновь созданный узел. Теперь вершина стека указывает на новый узел.

    void push(Node_t **head, T value) {
    Node_t *tmp = malloc(sizeof(Node_t));
    if (tmp == NULL) {
        exit(STACK_OVERFLOW);
    }
    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
}

    //Функция pop берёт первый элемент (тот, на который указывает вершина), перекидывает указатель на следующий элемент и возвращает первый. Здесь есть два варианта – можно вернуть узел или значение. Если вернём значение, то придётся удалять узел внутри функции


    Node_t* pop1(Node_t **head) {
    Node_t *out;
    if ((*head) == NULL) {
        exit(STACK_UNDERFLOW);
    }
    out = *head;
    *head = (*head)->next;
    return out;
}

    T pop2(Node_t **head) {
    Node_t *out;
    T value;
    if (*head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    out = *head;
    *head = (*head)->next;
    value = out->value;
    free(out);
    return value;
}

//Теперь вместо проверки на длину массива везде используется проверка на равенство NULL вершины стека.

    //Простая функция peek

    T peek(const Node_t* head) {
    if (head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    return head->value;
}

//Итерирование достаточно интересное. Просто переходим от одного узла к другому, пока не дойдём до конца


    void printStack(const Node_t* head) {
    printf("stack >");
    while (head) {
        printf("%d ", head->value);
        head = head->next;
    }
}

//И ещё одна проблема – теперь нельзя просто посмотреть размер стека. Нужно пройти от начала до конца и посчитать все элементы. Например, так

    size_t getSize(const Node_t *head) {
    size_t size = 0;
    while (head) {
        size++;
        head = head->next;
    }
    return size;
}


//Конечно, можно хранить размер отдельно, можно обернуть стек со всеми данными ещё в одну структуру и т.д.


int main()
{
    int i;
    Node_t *head = NULL;
    for (i = 0; i < 300; i++) {
        push(&head, i);
    }
    printf("size = %d\n", getSize(head));
    while (head) {
        printf("%d ", peek(head));
        printf("%d ", pop2(&head));
    }
    //  _getch(); ISO 99


 /*

    int i;
    Node_t *head = NULL;
    Node_t *tmp;
    for (i = 0; i < 300; i++) {
        push(&head, i);
    }
    printf("size = %d\n", getSize(head));
    while (head) {
        printf("%d ", peek(head));
        tmp = pop1(&head);
        printf("%d ", tmp->value);
        free(tmp);
    }
   // _getch(); ISO 99

*/


    return 0;

}
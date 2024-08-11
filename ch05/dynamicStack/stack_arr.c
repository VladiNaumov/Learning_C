#include <stdio.h>
#include <stdlib.h>

/*
 * PUSH – кладёт элемент на вершину стека
 * POP – снимает элемент с вершины стека, перемещая вершину к следующему элементу */

/* Стек фиксированного размера, построенный на массиве */
#define STACK_MAX_SIZE 20
typedef int T;

//Теперь сама структура
typedef struct Stack_tag {
    T data[STACK_MAX_SIZE];
    size_t size;
} Stack_t;

#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101

/*Здесь переменная size – это количество элементов, и вместе с тем указатель на вершину стека.
 * Вершина будет указывать на следующий элемент массива, в который будет занесено значение. */

void push(Stack_t *stack, const T value) {
    if (stack->size >= STACK_MAX_SIZE) {
        exit(STACK_OVERFLOW);
    }
    stack->data[stack->size] = value;
    stack->size++;
}


//Аналогично, определим операцию Pop, которая возвращает элемент с вершины и переходит к следующему
T pop(Stack_t *stack) {
    if (stack->size == 0) {
        exit(STACK_UNDERFLOW);
    }
    stack->size--;
    return stack->data[stack->size];
}

//И функция peek, возвращающая текущий элемент с вершины
T peek(const Stack_t *stack) {
    if (stack->size <= 0) {
        exit(STACK_UNDERFLOW);
    }
    return stack->data[stack->size - 1];
}

//Ещё одно важное замечание – у нас нет функции создания стека, поэтому необходимо вручную обнулять значение size

//Вспомогательные функции для печати элементов стека
void printStackValue(const T value) {
    printf("%d", value);
}

void printStack(const Stack_t *stack, void (*printStackValue)(const T)) {
    int i;
    int len = stack->size - 1;
    printf("stack %d > ", stack->size);
    for (i = 0; i < len; i++) {
        printStackValue(stack->data[i]);
        printf(" | ");
    }
    if (stack->size != 0) {
        printStackValue(stack->data[i]);
    }
    printf("\n");
}

int main()
{
    Stack_t stack;
    stack.size = 0;

    push(&stack, 3);
    printStack(&stack, printStackValue);
    push(&stack, 5);
    printStack(&stack, printStackValue);
    push(&stack, 7);
    printStack(&stack, printStackValue);
    printf("%d\n", pop(&stack));
    printStack(&stack, printStackValue);
    printf("%d\n", pop(&stack));
    printStack(&stack, printStackValue);
    printf("%d\n", pop(&stack));
    printStack(&stack, printStackValue);

    //_getch(); ICO 99

    return 0;

}

#include <stdio.h>
#include <stdlib.h>

/*
 * PUSH – кладёт элемент на вершину стека
 * POP – снимает элемент с вершины стека, перемещая вершину к следующему элементу */

/* Динамически растущий стек на массиве */

#define INIT_SIZE 10
#define MULTIPLIER 2
typedef int T;

//Теперь сама структура
typedef struct Stack_tag {
    T *data;
    size_t size;
    size_t top;
} Stack_t;


#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY   -102

Stack_t* createStack() {
    Stack_t *out = NULL;
    out = malloc(sizeof(Stack_t));
    if (out == NULL) {
        exit(OUT_OF_MEMORY);
    }
    out->size = INIT_SIZE;
    out->data = malloc(out->size * sizeof(T));
    if (out->data == NULL) {
        free(out);
        exit(OUT_OF_MEMORY);
    }
    out->top = 0;
    return out;
}

void deleteStack(Stack_t **stack) {
    free((*stack)->data);
    free(*stack);
    *stack = NULL;
}

// Теперь напишем вспомогательную функцию изменения размера.
    void resize(Stack_t *stack) {
    stack->size *= MULTIPLIER;
    stack->data = realloc(stack->data, stack->size * sizeof(T));
    if (stack->data == NULL) {
        exit(STACK_OVERFLOW);
    }
}


// Функция push проверяет, вышли ли мы за пределы массива. Если да, то увеличиваем его размер
    void push(Stack_t *stack, T value) {
    if (stack->top >= stack->size) {
        resize(stack);
    }
    stack->data[stack->top] = value;
    stack->top++;
}

// Функции pop и peek аналогичны тем, которые использовались для массива фиксированного размера
        T pop(Stack_t *stack) {
        if (stack->top == 0) {
            exit(STACK_UNDERFLOW);
        }
        stack->top--;
        return stack->data[stack->top];
    }
    T peek(const Stack_t *stack) {
        if (stack->top <= 0) {
            exit(STACK_UNDERFLOW);
        }
        return stack->data[stack->top - 1];
    }


 /*
 Напишем ещё одну функцию, implode, которая уменьшает массив до размера, равного числу элементов в массиве.
 Она может быть использована тогда, когда уже известно, что больше элементов вставлено не будет, и память может быть частично освобождена.
 */

void implode(Stack_t *stack) {
    stack->size = stack->top;
    stack->data = realloc(stack->data, stack->size * sizeof(T));
}

//Можем использовать в нашем случае
for (i = 0; i < 300; i++) {
    push(s, i);
}
implode(s);
for (i = 0; i < 300; i++) {
    printf("%d ", peek(s));
    printf("%d ", pop(s));
}

int main()
{
    int i;
    Stack_t *s = createStack();

    for (i = 0; i < 300; i++) {
        push(s, i);
    }
    for (i = 0; i < 300; i++) {
        printf("%d ", peek(s));
        printf("%d ", pop(s));
    }

    deleteStack(&s);
//    _getch(); ISO 99

    return 0;

}
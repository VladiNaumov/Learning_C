#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Вставляет новый узел со значением `info` на вершину стека
void push(StackNodePtr *topPtr, int info) {
    StackNodePtr newPtr = malloc(sizeof(StackNode));

    if (newPtr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed while pushing %d onto the stack.\n", info);
        return;
    }

    newPtr->data = info;
    newPtr->nextPtr = *topPtr;
    *topPtr = newPtr;
}

// Удаляет узел с вершины стека и возвращает его значение
int pop(StackNodePtr *topPtr) {
    if (isEmpty(*topPtr)) {
        fprintf(stderr, "Error: Stack underflow. Attempt to pop from an empty stack.\n");
        exit(EXIT_FAILURE);
    }

    StackNodePtr tempPtr = *topPtr;
    int popValue = tempPtr->data;
    *topPtr = tempPtr->nextPtr;
    free(tempPtr);

    return popValue;
}

// Выводит содержимое стека
void printStack(StackNodePtr currentPtr) {
    if (currentPtr == NULL) {
        puts("The stack is empty.");
        return;
    }

    puts("The stack is:");
    while (currentPtr != NULL) {
        printf("%d -> ", currentPtr->data);
        currentPtr = currentPtr->nextPtr;
    }
    puts("NULL");
}

// Проверяет, пуст ли стек
int isEmpty(StackNodePtr topPtr) {
    return topPtr == NULL;
}

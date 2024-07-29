#ifndef STACK_H
#define STACK_H

// Определение структуры узла стека
typedef struct stackNode {
    int data;                  // Данные узла
    struct stackNode *nextPtr; // Указатель на следующий узел
} StackNode;

// Указатель на узел стека
typedef StackNode *StackNodePtr;

// Прототипы функций
void push(StackNodePtr *topPtr, int info);
int pop(StackNodePtr *topPtr);
int isEmpty(StackNodePtr topPtr);
void printStack(StackNodePtr currentPtr);
void instructions(void);

#endif // STACK_H

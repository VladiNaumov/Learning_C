#include <stdio.h>
#include <stdlib.h>
#include "stack.h"


// Структура, ссылающаяся на саму себя
typedef struct stackNode {
    int data;                  // определить данные с типом int
    struct stackNode *nextPtr; // указатель на структуру stackNode
}StackNode;

typedef StackNode *StackNodePtr;    // синоним для StackNode*

// вставляет узел на вершину стека
void push( StackNodePtr *topPtr, int info )
{
    StackNodePtr newPtr; // указатель на новый узел

    newPtr = malloc( sizeof( StackNode ) );

    // вставить узел на вершину стека
    if ( newPtr != NULL ) {
        newPtr->data = info;
        newPtr->nextPtr = *topPtr;
        *topPtr = newPtr;
    }
    else {
        printf( "%d not inserted. No memory available.\n", info );
    }
}

// удаляет узел с вершины стека
int pop( StackNodePtr *topPtr )
{
    StackNodePtr tempPtr; // временный указатель на узел
    int popValue;         // значение узла

    tempPtr = *topPtr;
    popValue = ( *topPtr )->data;
    *topPtr = ( *topPtr )->nextPtr;
    free( tempPtr );
    return popValue;
}

// выводит содержимое стека
void printStack( StackNodePtr currentPtr )
{
    // если стек пуст
    if ( currentPtr == NULL ) {
        puts( "The stack is empty.\n" );
    }
    else {
        puts( "The stack is:" );

        // пока не достигнут конец стека
        while ( currentPtr != NULL ) {
            printf( "%d --> ", currentPtr->data );
            currentPtr = currentPtr->nextPtr;
        }
        puts( "NULL\n" );
    }
}
// возвращает 1, если стек пуст, 0 - в противном случае
int isEmpty( StackNodePtr topPtr )
{
    return topPtr == NULL;
}


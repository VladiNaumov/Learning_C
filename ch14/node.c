#include <stdio.h>
#include <stdlib.h>
#include "node.h"

typedef struct listNode {
    char data;                // каждая структура listNode содержит символ
    struct listNode *nextPtr; // указатель на следующий узел
}ListNode;

typedef ListNode *ListNodePtr;



// вставляет новое значение в список в порядке сортировки
void insert( ListNodePtr *sPtr, char value )
{
    ListNodePtr newPtr;      // указатель на новый узел
    ListNodePtr previousPtr; // указатель на предыдущий узел в списке
    ListNodePtr currentPtr;  // указатель на текущий узел в списке

    newPtr = malloc( sizeof( ListNode ) ); // создать узел

    if ( newPtr != NULL ) {
        newPtr->data = value;   // записать значение в узел
        newPtr->nextPtr = NULL; // узел пока не связан с другим узлом

        previousPtr = NULL;
        currentPtr = *sPtr;

        // найти место в списке для вставки нового узла
        while ( currentPtr != NULL && value > currentPtr->data ) {
            previousPtr = currentPtr;        // перейти к ...
            currentPtr = currentPtr->nextPtr; // ... следующему узлу
        }

        // вставить новый узел в начало списка
        if ( previousPtr == NULL ) {
            newPtr->nextPtr = *sPtr;
            *sPtr = newPtr;
        }
        else { // вставить новый узел между previousPtr и currentPtr
            previousPtr->nextPtr = newPtr;
            newPtr->nextPtr = currentPtr;
            }
        }
    else {
        printf( "%c not inserted. No memory available.\n", value );
         }
    }

// удаляет элемент из списка
char delete( ListNodePtr *sPtr, char value )
{
    ListNodePtr previousPtr; // указатель на предыдущий узел в списке125
    ListNodePtr currentPtr;  // указатель на текущий узел в списке
    ListNodePtr tempPtr;     // временный указатель на узел

    // удалить первый узел
    if ( value == ( *sPtr )->data ) {
        tempPtr = *sPtr; // сохранить указатель на удаляемый узел
        *sPtr = ( *sPtr )->nextPtr; // исключить узел из списка
        free( tempPtr ); // освободить память, занимаемую исключенным узлом
        return value;
    } // конец if
    else {
        previousPtr = *sPtr;
        currentPtr = ( *sPtr )->nextPtr;

        // найти элемент списка с указанным символом
        while ( currentPtr != NULL && currentPtr->data != value ) {
            previousPtr = currentPtr;         // перейти к ...
            currentPtr = currentPtr->nextPtr; // ... следующему узлу
        }

        // удалить узел currentPtr
        if ( currentPtr != NULL ) {
            tempPtr = currentPtr;
            previousPtr->nextPtr = currentPtr->nextPtr;
            free( tempPtr );
            return value;
        } // конец if
    }

    return '\0';
}

// возвращает 1, если список пуст, 0 - в противном случае
int isEmpty( ListNodePtr sPtr )
{
    return sPtr == NULL;
}

// выводит список
void printList( ListNodePtr currentPtr )
{
    // если список пуст
    if ( isEmpty( currentPtr ) ) {
        puts( "List is empty.\n" );
    }
    else {
        puts( "The list is:" );

        // пока не достигнут конец списка
        while ( currentPtr != NULL ) {
            printf( "%c --> ", currentPtr->data );
            currentPtr = currentPtr->nextPtr;
        }

        puts( "NULL\n" );
    }
}

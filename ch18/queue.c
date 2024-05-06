#include <stdio.h>
#include <stdlib.h>

// структура, ссылающаяся на саму себя
struct queueNode {
    char data;                 // определить данные с типом char
    struct queueNode *nextPtr; // указатель на структуру queueNode
};

typedef struct queueNode QueueNode;
typedef QueueNode *QueueNodePtr;

int isEmpty( QueueNodePtr headPtr);

// вставляет узел в хвост очереди
void enqueue( QueueNodePtr *headPtr, QueueNodePtr *tailPtr,char value )
{
    QueueNodePtr newPtr; // указатель на новый узел

    newPtr = malloc( sizeof( QueueNode ) );

    if ( newPtr != NULL ) { // память доступна
        newPtr->data = value;
        newPtr->nextPtr = NULL;

        // если очередь пуста, вставить узел в голову очереди
        if ( isEmpty( *headPtr ) ) {
            *headPtr = newPtr;
        }
        else {
            ( *tailPtr )->nextPtr = newPtr;
        }
        *tailPtr = newPtr;
    }
    else {
        printf( "%c not inserted. No memory available.\n", value );
    }
}


// удаляет узел из головы очереди
char dequeue( QueueNodePtr *headPtr, QueueNodePtr *tailPtr )
{
    char value;           // значение узла
    QueueNodePtr tempPtr; // временный указатель на узел

    value = ( *headPtr )->data;
    tempPtr = *headPtr;
    *headPtr = ( *headPtr )->nextPtr;

    // если очередь пуста
    if ( *headPtr == NULL ) {
        *tailPtr = NULL;
    }
    free( tempPtr );
    return value;
}

// возвращает 1, если очередь пуста, 0 – в противном случае
int isEmpty( QueueNodePtr headPtr )
{
    return headPtr == NULL;
}

// выводит содержимое очереди
void printQueue( QueueNodePtr currentPtr )
{
    // если очередь пуста
    if ( currentPtr == NULL ) {
        puts( "Queue is empty.\n" );
    }
    else {
        puts( "The queue is:" );

        // пока не достигнут конец очереди
        while ( currentPtr != NULL ) {
            printf( "%c --> ", currentPtr->data );
            currentPtr = currentPtr->nextPtr;
        }

        puts( "NULL\n" );
    }
}

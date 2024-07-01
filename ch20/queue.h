#ifndef QUEUE_H
#define QUEUE_H


// структура, ссылающаяся на саму себя
struct queueNode;

typedef struct queueNode QueueNode;
typedef QueueNode *QueueNodePtr;

// прототипы функций
void printQueue( QueueNodePtr currentPtr );
int isEmpty( QueueNodePtr headPtr );
char dequeue( QueueNodePtr *headPtr, QueueNodePtr *tailPtr );
void enqueue( QueueNodePtr *headPtr, QueueNodePtr *tailPtr,char value );
void instructions( void );


#endif // QUEUE_H

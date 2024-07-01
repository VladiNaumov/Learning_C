#ifndef STACK_H
#define STACK_H


struct stackNode;

typedef struct stackNode StackNode; // синоним для struct stackNode
typedef StackNode *StackNodePtr;    // синоним для StackNode*

// прототипы
void push( StackNodePtr *topPtr, int info );
int pop( StackNodePtr *topPtr );
int isEmpty( StackNodePtr topPtr );
void printStack( StackNodePtr currentPtr );
void instructions( void );



#endif // STACK_H

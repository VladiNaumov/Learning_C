#ifndef NODE_H
#define NODE_H


struct listNode;

typedef struct listNode ListNode; // синоним для struct listNode
typedef ListNode *ListNodePtr;    // синоним для ListNode*

// прототипы
void insert( ListNodePtr *sPtr, char value );
char delete( ListNodePtr *sPtr, char value );
int isEmpty( ListNodePtr sPtr );
void printList( ListNodePtr currentPtr );
void instructions( void );


#endif // NODE_H

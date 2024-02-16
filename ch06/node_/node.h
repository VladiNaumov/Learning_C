#ifndef NODE_H
#define NODE_H

// структура атрибутов без публично доступных полей
struct node;
struct Node* head;

// функция выделения памяти
void push(struct node **head, char* data);


// деструктор
void node_destroy(struct node*);


void iterator(struct node* pointer);



#endif

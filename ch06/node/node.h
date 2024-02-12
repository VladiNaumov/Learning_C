#ifndef NODE_H
#define NODE_H

// структура атрибутов без публично доступных полей
struct node;


// функция выделения памяти
struct node* node_malloc();

// конструктор
void node_new(struct node* pointer, char* value);

// деструктор
void node_destroy(struct node*);

void node_linc(struct node* pointer, void* p);

void iterator(struct node* pointer);

void addArray_(struct node* pointer, int var);

void getArr(int num);

void getAddress(struct node* pointer);

#endif

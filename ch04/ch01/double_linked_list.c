#include <stdio.h>
#include <stdlib.h>

// Определение структуры узла для двусвязного списка
struct Node {
    int data;
    struct Node* next;   // Указатель на следующий элемент
    struct Node* prev;   // Указатель на предыдущий элемент
};

// Функция для создания нового узла
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

// Функция для печати двусвязного списка
void printList(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

// Функция для освобождения памяти списка
void freeList(struct Node* head) {
    struct Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    // Создаем узлы списка
    struct Node* node1 = createNode(10);
    struct Node* node2 = createNode(20);
    struct Node* node3 = createNode(30);

    // Связываем узлы
    node1->next = node2;
    node2->prev = node1;
    node2->next = node3;
    node3->prev = node2;

    // Печатаем список
    printf("Двунаправленный список: ");
    printList(node1);

    // Освобождаем память
    freeList(node1);

    return 0;
}

/*

Структура Node:

Содержит целое число data, указатель на следующий узел next и указатель на предыдущий узел prev.
Функция createNode:

Выделяет память для нового узла, инициализирует его значением и возвращает указатель на этот узел.
Функция printList:

Проходит по списку от головы до конца, печатает данные каждого узла.
Функция freeList:

Освобождает память, выделенную для каждого узла в списке.
main:

Создает три узла, связывает их в двусвязный список, печатает список и освобождает память.
*/
#include <stdio.h>
#include <stdlib.h>

// Определяем структуру Node
struct Node {
    int data;
    struct Node* next;  // Указатель на следующий элемент типа Node
};

int main() {
    // Создаем экземпляр структуры Node
    struct Node* node1 = (struct Node*)malloc(sizeof(struct Node));
    struct Node* node2 = (struct Node*)malloc(sizeof(struct Node));

    // Инициализируем данные структуры
    node1->data = 10;
    node1->next = node2;

    node2->data = 20;
    node2->next = NULL;  // Последний элемент, поэтому next указывает на NULL

    // Выводим значения
    printf("Node 1 data: %d\n", node1->data);
    printf("Node 2 data: %d\n", node2->data);

    // Освобождаем память
    free(node1);
    free(node2);

    return 0;
}

/*

Определение структуры: Мы определили структуру Node, которая содержит целое число data и указатель на следующий узел типа Node.

Создание экземпляров: Мы выделяем память для двух экземпляров Node с помощью malloc.

Инициализация: Присваиваем значения полям data и next. Первый узел (node1) указывает на второй узел (node2).

Освобождение памяти: В конце программы освобождаем память, выделенную для узлов, чтобы предотвратить утечки памяти.
*/
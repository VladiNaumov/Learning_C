#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char* value;           // значение
    struct node* next;     // указатель на следующий узел
} node;

// Конструктор узла
void node_constructor(node* pointer, node* next, const char* name)
{
    pointer->value = (char*)malloc(strlen(name) + 1);
    if (pointer->value != NULL) {
        strcpy(pointer->value, name);
    } else {
        fprintf(stderr, "Не удалось выделить память для строки.\n");
    }
    pointer->next = next;
}

// Функция для добавления нового узла в конец списка
void add_node_to_end(node* head, const char* name)
{
    node* new_node = (node*)malloc(sizeof(node)); // Выделение памяти для нового узла
    if (new_node == NULL) {
        fprintf(stderr, "Не удалось выделить память для нового узла.\n");
        return;
    }

    // Инициализация нового узла
    node_constructor(new_node, NULL, name);

    // Поиск последнего узла
    node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    // Присоединение нового узла к последнему узлу
    current->next = new_node;
}

// Итерация по списку и вывод значений
void iterator(node* pointer)
{
    while (pointer != NULL)
    {
        printf("value = %s \n", pointer->value);
        pointer = pointer->next;  // переходим к следующему узлу
    }
}

// Очистка памяти, выделенной под узлы
void free_list(node* head)
{
    node* current = head;
    node* next;

    while (current != NULL) {
        next = current->next;
        free(current->value); // Освобождение памяти под значение
        free(current);         // Освобождение памяти под узел
        current = next;
    }
}

int main(void)
{
    // Создание и инициализация первого узла (головы списка)
    node head;
    node_constructor(&head, NULL, "Bob");

    // Добавление новых узлов в конец списка
    add_node_to_end(&head, "Lisp");
    add_node_to_end(&head, "Kate");
    add_node_to_end(&head, "Tom");
    add_node_to_end(&head, "Alice");

    // Итерация по списку и вывод значений
    iterator(&head);

    // Освобождение памяти
    free_list(&head);

    return 0;
}

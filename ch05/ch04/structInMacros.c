#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для представления человека
struct person {
    char name[50];
    int age;
    struct person *next;
};

// Макрос для обхода списка
#define for_each_person(p, head) \
    for (p = head; p != NULL; p = p->next)

int main() {
    // Создаем несколько элементов списка
    struct person *head = NULL;
    struct person *person1 = malloc(sizeof(struct person));
    struct person *person2 = malloc(sizeof(struct person));
    struct person *person3 = malloc(sizeof(struct person));

    // Инициализация данных
    strcpy(person1->name, "Alice");
    person1->age = 30;
    person1->next = person2;

    strcpy(person2->name, "Bob");
    person2->age = 25;
    person2->next = person3;

    strcpy(person3->name, "Charlie");
    person3->age = 20;
    person3->next = NULL;

    // Указываем голову списка
    head = person1;

    // Используем макрос для обхода списка
    struct person *p;
    for_each_person(p, head) {
        printf("Name: %s, Age: %d\n", p->name, p->age);
    }

    // Освобождаем выделенную память
    free(person1);
    free(person2);
    free(person3);

    return 0;
}

/*
Объяснение кода:
Структура person:

Представляет человека с именем (name), возрастом (age) и указателем на следующий элемент списка (next).
Макрос for_each_person:

Это простой макрос, который берет указатель на текущий элемент (p) и указатель на голову списка (head).
Макрос выполняет цикл for, который начинается с головы списка и продолжается, пока указатель p не станет равным NULL.
На каждой итерации указатель p обновляется, указывая на следующий элемент списка.
Основная программа:

Создаются три элемента списка (person1, person2, person3).
Элементы инициализируются данными и связываются друг с другом через поле next.
Указатель на голову списка (head) указывает на первый элемент (person1).
Используется макрос for_each_person для обхода и вывода данных каждого элемента списка.
В конце освобождается выделенная память.
Этот пример демонстрирует, как можно использовать макрос для обхода произвольного списка структур, что аналогично макросу for_each_process в ядре Linux.
*/
/* Пример кода, реализующего второй способ, где в массиве хранятся указатели на структуры Person: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 50

typedef struct {
    char name[NAME_LENGTH];
    int age;
    char address[100];
} Person;

void addPerson(Person ***people, int *count, int *capacity) {
    if (*count >= *capacity) {
        *capacity *= 2;
        *people = realloc(*people, *capacity * sizeof(Person *));
        if (*people == NULL) {
            printf("Ошибка выделения памяти!\n");
            exit(1);
        }
    }

    (*people)[*count] = malloc(sizeof(Person));
    if ((*people)[*count] == NULL) {
        printf("Ошибка выделения памяти для новой структуры!\n");
        exit(1);
    }

    printf("Введите имя: ");
    scanf("%s", (*people)[*count]->name);

    printf("Введите возраст: ");
    scanf("%d", &(*people)[*count]->age);

    printf("Введите адрес: ");
    scanf("%s", (*people)[*count]->address);

    (*count)++;
    printf("Человек добавлен!\n");
}

void listPeople(Person **people, int count) {
    if (count == 0) {
        printf("Картотека пуста.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("ID: %d\n", i + 1);
        printf("Имя: %s\n", people[i]->name);
        printf("Возраст: %d\n", people[i]->age);
        printf("Адрес: %s\n", people[i]->address);
        printf("-------------------\n");
    }
}

void deletePerson(Person **people, int *count) {
    if (*count == 0) {
        printf("Картотека пуста.\n");
        return;
    }

    int id;
    printf("Введите ID человека для удаления: ");
    scanf("%d", &id);

    if (id < 1 || id > *count) {
        printf("Неверный ID!\n");
        return;
    }

    id--; // Преобразуем в индекс массива

    // Освобождаем память для удаляемой структуры
    free(people[id]);

    // Смещаем все элементы массива на одну позицию влево
    for (int i = id; i < *count - 1; i++) {
        people[i] = people[i + 1];
    }

    (*count)--;
    printf("Человек удалён!\n");
}

void editPerson(Person **people, int count) {
    if (count == 0) {
        printf("Картотека пуста.\n");
        return;
    }

    int id;
    printf("Введите ID человека для редактирования: ");
    scanf("%d", &id);

    if (id < 1 || id > count) {
        printf("Неверный ID!\n");
        return;
    }

    id--; // Преобразуем в индекс массива

    printf("Редактирование человека ID %d:\n", id + 1);
    printf("Текущее имя: %s\n", people[id]->name);
    printf("Введите новое имя: ");
    scanf("%s", people[id]->name);

    printf("Текущий возраст: %d\n", people[id]->age);
    printf("Введите новый возраст: ");
    scanf("%d", &people[id]->age);

    printf("Текущий адрес: %s\n", people[id]->address);
    printf("Введите новый адрес: ");
    scanf("%s", people[id]->address);

    printf("Данные человека обновлены!\n");
}

int main() {
    int capacity = 2; // Начальная ёмкость массива
    Person **people = malloc(capacity * sizeof(Person *));
    if (people == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    int count = 0;
    int choice;

    while (1) {
        printf("\nМеню:\n");
        printf("1. Добавить нового человека\n");
        printf("2. Показать всех людей\n");
        printf("3. Удалить человека\n");
        printf("4. Изменить данные человека\n");
        printf("5. Выйти\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPerson(&people, &count, &capacity);
                break;
            case 2:
                listPeople(people, count);
                break;
            case 3:
                deletePerson(people, &count);
                break;
            case 4:
                editPerson(people, count);
                break;
            case 5:
                printf("Выход...\n");
                // Освобождение памяти перед выходом
                for (int i = 0; i < count; i++) {
                    free(people[i]);
                }
                free(people);
                return 0;
            default:
                printf("Неверный выбор!\n");
        }
    }

    return 0;
}

/*
Массив указателей на структуры:

Массив people теперь представляет собой массив указателей на структуры Person (тип Person **people).
Каждый элемент массива people[i] является указателем на отдельную структуру Person.
Выделение памяти для каждой структуры:

В функции addPerson сначала выделяется память для массива указателей, а затем для каждой новой структуры Person.
Удаление элементов:

В функции deletePerson освобождается память, выделенная для удаляемой структуры, и затем элементы массива смещаются.
Освобождение памяти при завершении программы:

Перед завершением программы освобождается память, выделенная для всех структур Person и самого массива указателей.
*/
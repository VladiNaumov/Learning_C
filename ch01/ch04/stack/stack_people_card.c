/* пример кода на языке C для создания простой картотеки людей с возможностью добавления нового человека: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PEOPLE 100
#define NAME_LENGTH 50

typedef struct {
    char name[NAME_LENGTH];
    int age;
    char address[100];
} Person;

void addPerson(Person *people, int *count) {
    if (*count >= MAX_PEOPLE) {
        printf("Картотека заполнена!\n");
        return;
    }

    printf("Введите имя: ");
    scanf("%s", people[*count].name);

    printf("Введите возраст: ");
    scanf("%d", &people[*count].age);

    printf("Введите адрес: ");
    scanf("%s", people[*count].address);

    (*count)++;
    printf("Человек добавлен!\n");
}

void listPeople(Person *people, int count) {
    if (count == 0) {
        printf("Картотека пуста.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("ID: %d\n", i + 1);
        printf("Имя: %s\n", people[i].name);
        printf("Возраст: %d\n", people[i].age);
        printf("Адрес: %s\n", people[i].address);
        printf("-------------------\n");
    }
}

void deletePerson(Person *people, int *count) {
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

    // Смещение всех людей на одну позицию влево, начиная с удалённого
    for (int i = id - 1; i < *count - 1; i++) {
        people[i] = people[i + 1];
    }

    (*count)--;
    printf("Человек удалён!\n");
}

void editPerson(Person *people, int count) {
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

    id--;  // Преобразуем в индекс массива

    printf("Редактирование человека ID %d:\n", id + 1);
    printf("Текущее имя: %s\n", people[id].name);
    printf("Введите новое имя: ");
    scanf("%s", people[id].name);

    printf("Текущий возраст: %d\n", people[id].age);
    printf("Введите новый возраст: ");
    scanf("%d", &people[id].age);

    printf("Текущий адрес: %s\n", people[id].address);
    printf("Введите новый адрес: ");
    scanf("%s", people[id].address);

    printf("Данные человека обновлены!\n");
}

int main() {
    Person people[MAX_PEOPLE]; // // Массив структур Person
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
                addPerson(people, &count);
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
                return 0;
            default:
                printf("Неверный выбор!\n");
        }
    }

    return 0;
}

/*
Описание:
Структура Person: Описывает человека с полями name, age и address.
Функция addPerson: Добавляет нового человека в картотеку.
Функция listPeople: Выводит список всех людей в картотеке.
Главная функция main: Обеспечивает меню для взаимодействия с пользователем.
Функция deletePerson: Удаляет человека по указанному ID (индексу в массиве).
После удаления всех последующих людей сдвигаются на одну позицию, чтобы заполнить пробел.
Функция editPerson: Позволяет редактировать данные человека (имя, возраст и адрес) по указанному ID.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "group.h"

// Функция для создания новой группы в куче
struct Group* createGroup() {
    struct Group *group = (struct Group *)malloc(sizeof(struct Group));
    if (group == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    group->count = 0;
    memset(group->people, 0, sizeof(group->people)); // Инициализация указателей в массиве
    return group;
}

// Функция для добавления человека в группу
void addPersonToGroup(struct Group *group, struct Person *person) {
    if (group->count < 100) {
        group->people[group->count] = person;
        group->count++;
    } else {
        printf("Group is full!\n");
    }
}

// Функция для вывода информации о группе
void printGroup(const struct Group *group) {
    for (int i = 0; i < group->count; i++) {
        printf("Name: %s, Age: %d, Gender: %s\n",
               group->people[i]->name, group->people[i]->age, group->people[i]->gender);
    }
}

// Функция для освобождения памяти, выделенной для людей в группе
void freeGroup(struct Group *group) {
    for (int i = 0; i < group->count; i++) {
        free(group->people[i]);
        group->people[i] = NULL; // Установка указателя в NULL после освобождения памяти
    }
    free(group); // Освобождение памяти для самой структуры Group
}

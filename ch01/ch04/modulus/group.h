#ifndef GROUP_H
#define GROUP_H

#include "person.h"

// Вторая структура, содержащая массив указателей на людей
struct Group {
    struct Person *people[100]; // Массив указателей на людей
    int count; // Количество людей в группе
};

// Функция для создания новой группы в куче
struct Group* createGroup();

// Функция для добавления человека в группу
void addPersonToGroup(struct Group *group, struct Person *person);

// Функция для вывода информации о группе
void printGroup(const struct Group *group);

// Функция для освобождения памяти, выделенной для людей в группе
void freeGroup(struct Group *group);

#endif // GROUP_H

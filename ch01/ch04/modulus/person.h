#ifndef PERSON_H
#define PERSON_H

// Структура для представления человека
struct Person {
    char name[50];
    int age;
    char gender[10];
};

// Функция для создания нового человека в куче
struct Person* createPerson(const char *name, int age, const char *gender);

#endif // PERSON_H

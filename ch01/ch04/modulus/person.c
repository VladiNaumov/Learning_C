#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"

// Функция для создания нового человека в куче
struct Person* createPerson(const char *name, int age, const char *gender) {
    struct Person *newPerson = (struct Person *)malloc(sizeof(struct Person));
    if (newPerson == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    strcpy(newPerson->name, name);
    newPerson->age = age;
    strcpy(newPerson->gender, gender);

    return newPerson;
}

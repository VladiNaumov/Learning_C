#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для представления человека
struct Person {
    char name[50];
    int age;
    char gender[10];
};

// Вторая структура, содержащая массив указателей на людей
struct Group {
    struct Person *people[100]; // Массив указателей на людей
    int count; // Количество людей в группе
};

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

// Основная функция, которая управляет всей логикой программы
void runProgram() {
    struct Group *group = createGroup();
    if (group == NULL) {
        return; // Не удалось создать группу, завершение работы функции
    }

    struct Person *person1 = createPerson("John Doe", 30, "Male");
    struct Person *person2 = createPerson("Jane Smith", 25, "Female");
    struct Person *person3 = createPerson("Alice Johnson", 28, "Female");

    if (person1 == NULL || person2 == NULL || person3 == NULL) {
        freeGroup(group); // Освобождение памяти для группы при ошибке
        return;
    }

    addPersonToGroup(group, person1);
    addPersonToGroup(group, person2);
    addPersonToGroup(group, person3);

    printGroup(group);

    freeGroup(group); // Освобождение памяти, выделенной для группы и ее содержимого
}

int main() {
    runProgram(); // Вызов основной функции программы
    return 0;
}

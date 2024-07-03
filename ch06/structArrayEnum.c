#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Определение перечисления Profession
typedef enum {
    ENGINEER,
    DOCTOR,
    TEACHER,
    NURSE
} Profession;

// Функция для получения строкового представления профессии
const char* get_profession_string(Profession profession) {
    switch (profession) {
    case ENGINEER: return "Engineer";
    case DOCTOR: return "Doctor";
    case TEACHER: return "Teacher";
    case NURSE: return "Nurse";
    default: return "Unknown";
    }
}

// Определение структуры Person
typedef struct {
    char *name;
    int age;
    Profession profession;
} Person;


// Функция для создания объекта Person
Person* create_person(char* name, int age, Profession profession) {
    Person* person = (Person*)malloc(sizeof(Person));
    if (person != NULL) {
        person->name = name;
        person->age = age;
        person->profession = profession;
    }
    return person;
}

// Функция для печати объекта Person
void print_person(const Person *person) {
    printf("Name: %s, Age: %d, Profession: %s\n", person->name, person->age, get_profession_string(person->profession));
}

// Функция для освобождения памяти объекта Person
void free_person(Person *person) {
    if (person != NULL) {
        free(person->name);
        free(person);
    }
}

// Функция для вывода всех людей по профессиям
void print_people_by_profession(Person **people, int count) {
    // Перечисляем все профессии
    Profession professions[] = {ENGINEER, DOCTOR, TEACHER, NURSE};
    int profession_count = sizeof(professions) / sizeof(professions[0]);

    // Печатаем людей по каждой профессии
    for (int p = 0; p < profession_count; p++) {
        printf("\n%s:\n", get_profession_string(professions[p]));
        for (int i = 0; i < count; i++) {
            if (people[i]->profession == professions[p]) {
                print_person(people[i]);
            }
        }
    }
}

// Функция для вывода людей определенной профессии
void print_people_by_single_profession(Person **people, int count, Profession profession) {
    printf("\n%s:\n", get_profession_string(profession));
    for (int i = 0; i < count; i++) {
        if (people[i]->profession == profession) {
            print_person(people[i]);
        }
    }
}

int main() {
    // Создание массива указателей на объекты Person
    int count = 10;
    Person **people = (Person**)malloc(count * sizeof(Person*));

    // Добавление объектов в массив
    people[0] = create_person("Alice", 30, ENGINEER);
    people[1] = create_person("Bob", 25, DOCTOR);
    people[2] = create_person("Charlie", 28, TEACHER);
    people[3] = create_person("David", 35, ENGINEER);
    people[4] = create_person("Eve", 22, NURSE);
    people[5] = create_person("Frank", 32, TEACHER);
    people[6] = create_person("Grace", 29, DOCTOR);
    people[7] = create_person("Hank", 31, ENGINEER);
    people[8] = create_person("Ivy", 27, NURSE);
    people[9] = create_person("Jack", 24, TEACHER);

    // Печать всех людей по профессиям
    print_people_by_profession(people, count);

    // Печать людей определенной профессии
    print_people_by_single_profession(people, count, DOCTOR);

    // Освобождение памяти всех объектов
    for (int i = 0; i < count; i++) {
        free_person(people[i]);
    }
    free(people);

    return 0;
}

/*
Пояснение кода
Перечисление Profession и функция get_profession_string:

Определяет возможные профессии и строковое представление каждой профессии.
Структура Person и вспомогательные функции:

create_string, create_person, print_person, и free_person используются для создания, печати и освобождения памяти объектов Person.
Функция print_people_by_profession:

Печатает всех людей, сгруппированных по профессиям, проходя по массиву всех людей и проверяя их профессии.
Новая функция print_people_by_single_profession:

Печатает людей только одной указанной профессии, проходя по массиву всех людей и проверяя их профессии.
Главная функция:

Создает массив указателей на объекты Person и инициализирует их.
Печатает всех людей по профессиям.
Печатает людей определенной профессии (в данном случае, DOCTOR).
Освобождает память всех объектов и массива указателей.
*/
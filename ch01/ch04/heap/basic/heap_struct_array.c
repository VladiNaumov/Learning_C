#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Определение структуры Person через typedef
typedef struct {
    char name[50];
    int age;
} Person;

// Функция для выделения памяти под массив структур Person
Person* allocatePersonsArray(int numPersons) {
    Person *persons = (Person *)malloc(numPersons * sizeof(Person));
    if (persons == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
    }
    return persons;
}

// Функция для вывода информации о каждом объекте Person
void printPersons(const Person *persons, int numPersons) {
    for (int i = 0; i < numPersons; ++i) {
        printf("Person %d: %s, возраст %d лет\n", i + 1, persons[i].name, persons[i].age);
    }
}

// Функция для освобождения памяти, выделенной под массив Person
void freePersonsArray(Person *persons) {
    free(persons);
}

int main() {
    int numPersons = 5; // Количество элементов в массиве

    // Выделение памяти под массив объектов Person
    Person *persons = allocatePersonsArray(numPersons);
    if (persons == NULL) {
        return 1; // В случае ошибки выделения памяти завершаем программу
    }

    // Инициализация массива объектов Person
    strcpy(persons[0].name, "Иван Иванов");
    persons[0].age = 30;

    strcpy(persons[1].name, "Петр Петров");
    persons[1].age = 25;

    strcpy(persons[2].name, "Мария Сидорова");
    persons[2].age = 35;

    strcpy(persons[3].name, "Елена Николаева");
    persons[3].age = 28;

    strcpy(persons[4].name, "Алексей Васильев");
    persons[4].age = 40;

    // Вывод информации о каждом объекте Person
    printPersons(persons, numPersons);

    // Освобождение памяти, выделенной под массив объектов Person
    freePersonsArray(persons);

    return 0;
}

/*
 у нас есть структура Person, и мы создаём массив структур Person. Этот массив структур представляет собой набор объектов, каждый из которых содержит два поля: имя (name) и возраст (age). Таким образом, массив структур Person — это структура данных, где каждый элемент массива является экземпляром структуры Person.

В коде, который я показал, мы используем функцию malloc для выделения памяти под массив структур Person, а затем инициализируем каждый элемент этого массива в функции main.

Вот как это выглядит:

Person *persons = allocatePersonsArray(numPersons); — создаётся указатель persons на массив структур Person, размер которого определяется количеством элементов (numPersons).
Затем мы инициализируем каждый элемент этого массива (например, persons[0], persons[1] и так далее).
Итак, структура Person описывает отдельного человека (имя и возраст), а массив структур Person позволяет нам хранить несколько таких объектов в одной переменной (в массиве).
*/
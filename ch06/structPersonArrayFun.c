#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Определение структуры Person с помощью typedef
typedef struct {
    char name[50];
    int age;
} Person;

// Функция для выделения памяти под массив указателей на объекты Person
Person** allocatePersonsArray(size_t numPersons) {
    // Выделение памяти под массив указателей на объекты Person
    Person **persons = malloc(numPersons * sizeof(Person*));
    // Проверка, была ли успешной операция выделения памяти
    if (persons == NULL) {
        // Вывод сообщения об ошибке в поток ошибок (stderr)
        fprintf(stderr, "Ошибка выделения памяти.\n");
    }
    // Возврат указателя на выделенный массив указателей на объекты Person
    return persons;
}

/*
Выделение памяти под массив указателей на объекты Person
Эта строка кода выделяет память под массив указателей на объекты типа Person. Рассмотрим её подробнее:

Person **persons: Это двойной указатель на Person. Он используется для хранения массива указателей на объекты Person.

malloc(numPersons * sizeof(Person*)): Функция malloc выделяет память размером numPersons * sizeof(Person*) байтов. 
Здесь numPersons - это количество элементов (то есть указателей на Person), которые вы хотите выделить, а sizeof(Person*) - размер указателя на объект типа Person в байтах.

азъяснение двойного указателя
В C и C++, двойные указатели (например, Person **) используются для работы с массивами указателей или для передачи указателей на функции. 
В данном случае, persons представляет собой массив указателей на объекты типа Person. 
Каждый элемент массива persons[i] будет указывать на объект типа Person.

*/


// Функция для создания объекта Person
Person* createPerson(const char *name, int age) {
    Person *newPerson = malloc(sizeof(Person));
    if (newPerson != NULL) {
        // Копирование имени в поле name с использованием strncpy_s
        strncpy_s(newPerson->name, sizeof(newPerson->name), name, _TRUNCATE);
        // Установка возраста
        newPerson->age = age;
    }
    return newPerson;
}

// Функция для вывода информации о Person
void printPerson(const Person *person) {
    printf("%s is %d years old.\n", person->name, person->age);
}

// c для заполнения массива объектов Person
void fillPersonsArray(Person **persons, size_t numPersons) {
    persons[0] = createPerson("Alice", 25);
    persons[1] = createPerson("Bob", 30);
    persons[2] = createPerson("Charlie", 28);
}

// Функция для освобождения памяти, выделенной для массива объектов Person
void freePersonsArray(Person **persons, size_t numPersons) {
    for (size_t i = 0; i < numPersons; ++i) {
        free(persons[i]);
    }
    free(persons);
}

// Функция для выполнения всей логики работы с массивом объектов Person
int processPersonsArray(size_t numPersons) {
    // Выделение памяти под массив указателей на объекты Person
    Person **persons = allocatePersonsArray(numPersons);
    if (persons == NULL) {
        return 1;
    }

    // Заполнение массива объектов Person
    fillPersonsArray(persons, numPersons);

    // Вывод информации о каждом Person
    for (size_t i = 0; i < numPersons; ++i) {
        printPerson(persons[i]);
    }

    // Освобождение памяти, выделенной для массива объектов Person
    freePersonsArray(persons, numPersons);

    return 0;
}

// Главная функция
int main() {
    size_t numPersons = 3;
    
    // Вызов функции, выполняющей всю логику работы с массивом объектов Person
    return processPersonsArray(numPersons);
}


/* 
typedef struct Person: Определяет структуру Person с использованием typedef, что упрощает работу с типом Person.

allocatePersonsArray: Функция для выделения памяти под массив указателей на объекты Person. Если выделение памяти не удалось, функция возвращает NULL.

createPerson: Функция для создания нового объекта Person с заданными именем и возрастом. Выделяет память для структуры Person и копирует имя.

printPerson: Функция для вывода информации о объекте Person.

fillPersonsArray: Функция для заполнения массива объектов Person вызовом createPerson.

freePersonsArray: Функция для освобождения памяти, выделенной для массива указателей на объекты Person.

processPersonsArray: Функция, включающая в себя всю логику работы с массивом объектов Person (выделение памяти, заполнение массива, вывод информации и освобождение памяти).

main: Главная функция, где происходит вызов функции processPersonsArray для выполнения всей работы.

*/
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


// Функция для создания объекта Person
Person* createPerson(const char *name, int age) {
    // Выделение памяти под новый объект Person
    Person *newPerson = malloc(sizeof(Person));
    if (newPerson != NULL) {
        // Копирование имени в новый объект Person
        strncpy(newPerson->name, name, sizeof(newPerson->name) - 1);
        newPerson->name[sizeof(newPerson->name) - 1] = '\0'; // Убедимся, что строка завершена нулевым символом
        // Установка возраста нового объекта Person
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
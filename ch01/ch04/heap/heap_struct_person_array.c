#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*   данный пример показывает как сделать массив структур Person   */

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

// Функция для инициализации массива объектов Person
void initializePersons(Person *persons, int numPersons) {
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

    /*

    Инициализация данных:
    Каждый элемент массива persons инициализируется с помощью функции strcpy, 
    которая копирует строку имени в поле name, а затем устанавливает возраст в поле age.
    
    */
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
    initializePersons(persons, numPersons);

    // Вывод информации о каждом объекте Person
    printPersons(persons, numPersons);

    // Освобождение памяти, выделенной под массив объектов Person
    freePersonsArray(persons);

    return 0;
}


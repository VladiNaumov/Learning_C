#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Определение структуры Person через typedef
typedef struct {
    char name[50];
    int age;
} Person;

int main() {
    int numPersons = 5; // Количество элементов в массиве
    Person *persons = (Person *)malloc(numPersons * sizeof(Person));

    // Проверка, выделена ли память успешно
    if (persons == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return 1;
    }

    // Пример инициализации массива объектов Person
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

    // Пример обращения к элементам массива объектов Person
    for (int i = 0; i < numPersons; ++i) {
        printf("Person %d: %s, возраст %d лет\n", i+1, persons[i].name, persons[i].age);
    }

    // Освобождение памяти после использования
    free(persons);

    return 0;
}


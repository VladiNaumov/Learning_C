#include <stdio.h>
#include <string.h>

// Определение структуры Person через typedef
typedef struct {
    char name[50];
    int age;
} Person;

// Функция для вывода информации о каждом объекте Person
void printPersons(const Person *persons, int numPersons) {
    for (int i = 0; i < numPersons; ++i) {
        printf("Person %d: %s, возраст %d лет\n", i + 1, persons[i].name, persons[i].age);
    }
}

int main() {
    int numPersons = 5; // Количество элементов в массиве

    // Создание массива структур Person на стеке
    Person persons[numPersons];

    // Инициализация массива объектов Person прямо в main
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

    return 0;
}

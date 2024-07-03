#include <stdio.h>
#include <string.h>


/*

Для того чтобы поместить объекты различных структур в один массив, необходимо использовать объединения (union). 
Объединения позволяют хранить в одном и том же месте памяти значения разных типов. Мы также можем использовать перечисление (enum) для отслеживания типа данных, хранящихся в объединении, чтобы правильно их интерпретировать при чтении.
Вот пример программы на языке C, которая использует объединения и перечисления для хранения различных объектов в одном массиве:

 */

// Определяем три структуры
typedef struct {
    char name[50];
    int age;
} Person;

typedef struct {
    char title[100];
    char author[50];
    int pages;
} Book;

typedef struct {
    char brand[50];
    char model[50];
    int year;
} Car;

// Определяем объединение, которое может содержать любую из трех структур
typedef union {
    Person person;
    Book book;
    Car car;
} Item;

// Определяем перечисление для отслеживания типа данных
typedef enum {
    PERSON,
    BOOK,
    CAR
} ItemType;

// Определяем структуру, содержащую объединение и его тип
typedef struct {
    ItemType type;
    Item item;
} ArrayItem;

int main() {

    const int counter = 9;
    // Создаем массив ArrayItem для хранения объектов
    ArrayItem array[counter];

    // Заполняем массив объектами Person
    array[0].type = PERSON;
    strcpy(array[0].item.person.name, "Alice");
    array[0].item.person.age = 30;

    array[1].type = PERSON;
    strcpy(array[1].item.person.name, "Bob");
    array[1].item.person.age = 25;

    array[2].type = PERSON;
    strcpy(array[2].item.person.name, "Charlie");
    array[2].item.person.age = 35;

    // Заполняем массив объектами Book
    array[3].type = BOOK;
    strcpy(array[3].item.book.title, "1984");
    strcpy(array[3].item.book.author, "George Orwell");
    array[3].item.book.pages = 328;

    array[4].type = BOOK;
    strcpy(array[4].item.book.title, "Brave New World");
    strcpy(array[4].item.book.author, "Aldous Huxley");
    array[4].item.book.pages = 288;

    array[5].type = BOOK;
    strcpy(array[5].item.book.title, "Fahrenheit 451");
    strcpy(array[5].item.book.author, "Ray Bradbury");
    array[5].item.book.pages = 256;

    // Заполняем массив объектами Car
    array[6].type = CAR;
    strcpy(array[6].item.car.brand, "Toyota");
    strcpy(array[6].item.car.model, "Camry");
    array[6].item.car.year = 2020;

    array[7].type = CAR;
    strcpy(array[7].item.car.brand, "Honda");
    strcpy(array[7].item.car.model, "Civic");
    array[7].item.car.year = 2018;

    array[8].type = CAR;
    strcpy(array[8].item.car.brand, "Ford");
    strcpy(array[8].item.car.model, "Mustang");
    array[8].item.car.year = 2021;

    // Читаем и выводим содержимое массива
    for (int i = 0; i < counter; i++) {
        if (array[i].type == PERSON) {
            printf("Person - Name: %s, Age: %d\n", array[i].item.person.name, array[i].item.person.age);
        } else if (array[i].type == BOOK) {
            printf("Book - Title: %s, Author: %s, Pages: %d\n", array[i].item.book.title, array[i].item.book.author, array[i].item.book.pages);
        } else if (array[i].type == CAR) {
            printf("Car - Brand: %s, Model: %s, Year: %d\n", array[i].item.car.brand, array[i].item.car.model, array[i].item.car.year);
        }
    }

    return 0;
}

/*
В этом примере:

Мы определяем три структуры: Person, Book и Car.
Создаем объединение Item, которое может содержать любую из трех структур.
Определяем перечисление ItemType для отслеживания типа данных, хранящихся в объединении.
Создаем структуру ArrayItem, содержащую объединение и его тип.
Заполняем массив array объектами различных типов.
Читаем и выводим содержимое массива, проверяя тип каждого элемента и правильно его интерпретируя.

*/
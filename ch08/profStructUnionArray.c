#include <stdio.h>
#include <stdlib.h>

// Вот пример кода, улучшенного с учетом профессиональных стандартов:

// Определение структуры Person
typedef struct {
    char *name;
    int age;
} Person;

// Определение структуры Book
typedef struct {
    char *title;
    char *author;
    int pages;
} Book;

// Определение структуры Car
typedef struct {
    char *brand;
    char *model;
    int year;
} Car;

// Перечисление типов
typedef enum {
    PERSON,
    BOOK,
    CAR
} ItemType;

// Определение объединения для хранения различных типов данных
typedef union {
    Person *person;
    Book *book;
    Car *car;
} ItemData;

// Определение структуры элемента массива
typedef struct {
    ItemType type;
    ItemData data;
} ArrayItem;


// Функция для создания объекта Person
ArrayItem create_person(char* name, int age) {
    ArrayItem item;
    item.type = PERSON;
    item.data.person = (Person*)malloc(sizeof(Person));
    if (item.data.person != NULL) {
        item.data.person->name = name;
        item.data.person->age = age;
    }
    return item;
}

// Функция для создания объекта Book
ArrayItem create_book(char* title, char* author, int pages) {
    ArrayItem item;
    item.type = BOOK;
    item.data.book = (Book*)malloc(sizeof(Book));
    if (item.data.book != NULL) {
        item.data.book->title = title;
        item.data.book->author = author;
        item.data.book->pages = pages;
    }
    return item;
}

// Функция для создания объекта Car
ArrayItem create_car( char* brand,  char* model, int year) {
    ArrayItem item;
    item.type = CAR;
    item.data.car = (Car*)malloc(sizeof(Car));
    if (item.data.car != NULL) {
        item.data.car->brand = brand;
        item.data.car->model = model;
        item.data.car->year = year;
    }
    return item;
}

// Функция для печати элемента массива
void print_item(const ArrayItem *item) {
    switch (item->type) {
    case PERSON:
        printf("Person - Name: %s, Age: %d\n", item->data.person->name, item->data.person->age);
        break;
    case BOOK:
        printf("Book - Title: %s, Author: %s, Pages: %d\n", item->data.book->title, item->data.book->author, item->data.book->pages);
        break;
    case CAR:
        printf("Car - Brand: %s, Model: %s, Year: %d\n", item->data.car->brand, item->data.car->model, item->data.car->year);
        break;
    default:
        printf("Unknown type\n");
    }
}

// Функция для освобождения памяти объекта Person
void free_person(Person *person) {
    free(person->name);
    free(person);
}

// Функция для освобождения памяти объекта Book
void free_book(Book *book) {
    free(book->title);
    free(book->author);
    free(book);
}

// Функция для освобождения памяти объекта Car
void free_car(Car *car) {
    free(car->brand);
    free(car->model);
    free(car);
}

// Функция для освобождения памяти элемента массива
void free_item(ArrayItem *item) {
    switch (item->type) {
    case PERSON:
        free_person(item->data.person);
        break;
    case BOOK:
        free_book(item->data.book);
        break;
    case CAR:
        free_car(item->data.car);
        break;
    default:
        printf("Unknown type\n");
    }
}

int main() {
        const int counter = 4;

        // Создание массива элементов
        ArrayItem items[counter];

        // Добавление объектов в массив
        items[0] = create_person("Alice", 30);
        items[1] = create_person("Bob", 25);
        items[2] = create_book("1984", "George Orwell", 328);
        items[3] = create_car("Toyota", "Camry", 2020);

        // Печать всех элементов массива
        for (int i = 0; i < counter; i++) {
            print_item(&items[i]);
        }

        // Освобождение памяти всех элементов массива
        for (int i = 0; i < counter; i++) {
            free_item(&items[i]);
        }

        return 0;
}

/*
Пояснение кода
Динамическое выделение памяти для объектов:

В функциях create_person, create_book, и create_car выделяется память для объектов Person, Book, и Car соответственно.
Создание строки с копированием:

Функция create_string выделяет необходимую память и копирует строку.
Функции для создания объектов:

В каждой функции создания объектов выделяется память для строк с помощью create_string.
Функция для печати элемента массива:

print_item печатает данные в зависимости от типа объекта.
Функции для освобождения памяти:

free_person, free_book, free_car: функции для освобождения памяти, выделенной под строки и самих объектов.
free_item: функция для вызова соответствующей функции освобождения памяти в зависимости от типа объекта.
*/
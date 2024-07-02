#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Определяем перечисление для отслеживания типа данных
typedef enum {
    PERSON,
    BOOK,
    CAR
} ItemType;

// Определяем структуру для обобщенного хранения объектов
typedef struct {
    ItemType type;
    void *data;
} ArrayItem;

int main() {
    // Создаем массив ArrayItem для хранения объектов
    ArrayItem array[9];

    // Создаем и заполняем объекты Person
    Person *person1 = (Person *)malloc(sizeof(Person));
    strcpy(person1->name, "Alice");
    person1->age = 30;
    array[0].type = PERSON;
    array[0].data = person1;

    Person *person2 = (Person *)malloc(sizeof(Person));
    strcpy(person2->name, "Bob");
    person2->age = 25;
    array[1].type = PERSON;
    array[1].data = person2;

    Person *person3 = (Person *)malloc(sizeof(Person));
    strcpy(person3->name, "Charlie");
    person3->age = 35;
    array[2].type = PERSON;
    array[2].data = person3;

    // Создаем и заполняем объекты Book
    Book *book1 = (Book *)malloc(sizeof(Book));
    strcpy(book1->title, "1984");
    strcpy(book1->author, "George Orwell");
    book1->pages = 328;
    array[3].type = BOOK;
    array[3].data = book1;

    Book *book2 = (Book *)malloc(sizeof(Book));
    strcpy(book2->title, "Brave New World");
    strcpy(book2->author, "Aldous Huxley");
    book2->pages = 288;
    array[4].type = BOOK;
    array[4].data = book2;

    Book *book3 = (Book *)malloc(sizeof(Book));
    strcpy(book3->title, "Fahrenheit 451");
    strcpy(book3->author, "Ray Bradbury");
    book3->pages = 256;
    array[5].type = BOOK;
    array[5].data = book3;

    // Создаем и заполняем объекты Car
    Car *car1 = (Car *)malloc(sizeof(Car));
    strcpy(car1->brand, "Toyota");
    strcpy(car1->model, "Camry");
    car1->year = 2020;
    array[6].type = CAR;
    array[6].data = car1;

    Car *car2 = (Car *)malloc(sizeof(Car));
    strcpy(car2->brand, "Honda");
    strcpy(car2->model, "Civic");
    car2->year = 2018;
    array[7].type = CAR;
    array[7].data = car2;

    Car *car3 = (Car *)malloc(sizeof(Car));
    strcpy(car3->brand, "Ford");
    strcpy(car3->model, "Mustang");
    car3->year = 2021;
    array[8].type = CAR;
    array[8].data = car3;

    // Читаем и выводим содержимое массива
    for (int i = 0; i < 9; i++) {
        if (array[i].type == PERSON) {
            Person *p = (Person *)array[i].data;
            printf("Person - Name: %s, Age: %d\n", p->name, p->age);
        } else if (array[i].type == BOOK) {
            Book *b = (Book *)array[i].data;
            printf("Book - Title: %s, Author: %s, Pages: %d\n", b->title, b->author, b->pages);
        } else if (array[i].type == CAR) {
            Car *c = (Car *)array[i].data;
            printf("Car - Brand: %s, Model: %s, Year: %d\n", c->brand, c->model, c->year);
        }

        /*
        Конструкция Person *p = (Person *)array[i].data; используется для приведения указателя общего типа void* к указателю конкретного типа Person*. Давайте разберем ее шаг за шагом.

        Пояснение Person *p = (Person *)array[i].data;
        array[i].data:

        array — это массив структур ArrayItem.
        i — это индекс элемента массива.
        data — это поле структуры ArrayItem, которое хранит указатель на данные типа void*. Поскольку void* может указывать на данные любого типа, это позволяет нам хранить указатели на объекты различных типов в одном массиве.
        (Person *)array[i].data:

        (Person *) — это операция приведения типа (каст), которая преобразует указатель общего типа void* к указателю конкретного типа Person*. Мы делаем это, чтобы сказать компилятору, что данные, на которые указывает data, на самом деле имеют тип Person.
        Person *p = (Person *)array[i].data;:

        Здесь мы создаем переменную p типа Person* и инициализируем ее значением (Person *)array[i].data. После приведения типа p становится указателем на структуру Person, и мы можем использовать его для доступа к полям структуры Person.
        
        Причина использования приведения типа:
        Поле data в структуре ArrayItem имеет тип void*, что позволяет ему хранить указатель на данные любого типа. Чтобы работать с этими данными как с конкретным типом (Person, Book, или Car), нам нужно явно привести void* к нужному типу указателя.

        Безопасность приведения:
        Приведение типа работает корректно только если вы уверены в том, что data действительно указывает на объект нужного типа. В нашем примере мы проверяем item->type, чтобы убедиться, что data содержит указатель на Person, перед приведением.

        Заключение
        Конструкция Person *p = (Person *)array[i].data; используется для приведения указателя типа void* к указателю типа Person*, что позволяет работать с данными как с объектами типа Person. Это приведение необходимо из-за использования обобщенной структуры ArrayItem, которая может хранить указатели на данные разных типов.

        */
    }

    // Освобождаем память
    for (int i = 0; i < 9; i++) {
        free(array[i].data);
    }

    return 0;
}

/*
Мы определяем три структуры: Person, Book и Car.
Определяем перечисление ItemType для отслеживания типа данных.
Создаем структуру ArrayItem, содержащую тип объекта и указатель на данные (void *data).
Заполняем массив array объектами различных типов, выделяя память под каждую структуру с помощью malloc.
Читаем и выводим содержимое массива, проверяя тип каждого элемента и правильно его интерпретируя.
Освобождаем память, выделенную под каждый объект, после использования.
*/
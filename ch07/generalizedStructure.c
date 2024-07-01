#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
можно создать массив структур, где каждая структура будет содержать информацию об объекте и тип этого объекта без использования объединений. 
Для этого можно использовать обобщенную структуру, которая будет содержать тип объекта и данные в виде указателя. 

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
    }

    // Освобождаем память
    for (int i = 0; i < 9; i++) {
        free(array[i].data);
    }

    return 0;
}

/*
Мы определяем три структуры: Person, Book и Car.
Создаем объединение Item, которое может содержать любую из трех структур.
Определяем перечисление ItemType для отслеживания типа данных, хранящихся в объединении.
Создаем структуру ArrayItem, содержащую объединение и его тип.
Заполняем массив array объектами различных типов.
Читаем и выводим содержимое массива, проверяя тип каждого элемента и правильно его интерпретируя.
*/
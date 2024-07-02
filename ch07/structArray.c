#include <stdio.h>
#include <string.h>


/*

Ниже представлен простой (но не типа безопасный) программы на языке C, которая включает:
три структуры, создает по три объекта от каждой структуры и помещает их в один массив указателей 
Для упрощения примера, будем использовать структуры для описания людей, книг и автомобилей.

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

int main() {
    // Создаем по три объекта от каждой структуры
    Person people[3] = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };

    Book books[3] = {
        {"1984", "George Orwell", 328},
        {"Brave New World", "Aldous Huxley", 288},
        {"Fahrenheit 451", "Ray Bradbury", 256}
    };

    Car cars[3] = {
        {"Toyota", "Camry", 2020},
        {"Honda", "Civic", 2018},
        {"Ford", "Mustang", 2021}
    };

    // Объединяем все объекты в один массив указателей
    void *array[9];
    
    for(int i = 0; i < 3; i++) {
        array[i] = &people[i];
    }

    for(int i = 0; i < 3; i++) {
        array[3 + i] = &books[i];
    }

    for(int i = 0; i < 3; i++) {
        array[6 + i] = &cars[i];
    }

    // Читаем и выводим содержимое массива
    for(int i = 0; i < 3; i++) {
        Person *p = (Person *)array[i];
        printf("Person - Name: %s, Age: %d\n", p->name, p->age);
    }

    for(int i = 3; i < 6; i++) {
        Book *b = (Book *)array[i];
        printf("Book - Title: %s, Author: %s, Pages: %d\n", b->title, b->author, b->pages);
    }

    for(int i = 6; i < 9; i++) {
        Car *c = (Car *)array[i];
        printf("Car - Brand: %s, Model: %s, Year: %d\n", c->brand, c->model, c->year);
    }

    return 0;
}

/*

В этом примере:

Мы определяем три структуры: Person, Book и Car.
Создаем по три объекта для каждой структуры.
Помещаем все объекты в один массив указателей array.
Читаем и выводим содержимое массива, делая приведение типов при доступе к каждому элементу.

 */
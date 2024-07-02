// Вот пример кода, улучшенного с учетом профессиональных стандартов:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Определение структур
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

// Перечисление для обозначения типа данных
typedef enum {
    PERSON,
    BOOK,
    CAR
} ItemType;

// Обобщенная структура для хранения указателя на объект и его типа
typedef struct {
    ItemType type;
    void *data;
} ArrayItem;

// Функции для создания объектов
Person* create_person(const char* name, int age) {
    Person *p = (Person*)malloc(sizeof(Person));
    if (p != NULL) {
        strncpy(p->name, name, sizeof(p->name) - 1);
        p->name[sizeof(p->name) - 1] = '\0';
        p->age = age;
    }
    return p;
}

Book* create_book(const char* title, const char* author, int pages) {
    Book *b = (Book*)malloc(sizeof(Book));
    if (b != NULL) {
        strncpy(b->title, title, sizeof(b->title) - 1);
        b->title[sizeof(b->title) - 1] = '\0';
        strncpy(b->author, author, sizeof(b->author) - 1);
        b->author[sizeof(b->author) - 1] = '\0';
        b->pages = pages;
    }
    return b;
}

Car* create_car(const char* brand, const char* model, int year) {
    Car *c = (Car*)malloc(sizeof(Car));
    if (c != NULL) {
        strncpy(c->brand, brand, sizeof(c->brand) - 1);
        c->brand[sizeof(c->brand) - 1] = '\0';
        strncpy(c->model, model, sizeof(c->model) - 1);
        c->model[sizeof(c->model) - 1] = '\0';
        c->year = year;
    }
    return c;
}

// Функция для вывода информации об объекте
void print_item(const ArrayItem *item) {
    if (item->type == PERSON) {
        Person *p = (Person *)item->data;
        printf("Person - Name: %s, Age: %d\n", p->name, p->age);
    } else if (item->type == BOOK) {
        Book *b = (Book *)item->data;
        printf("Book - Title: %s, Author: %s, Pages: %d\n", b->title, b->author, b->pages);
    } else if (item->type == CAR) {
        Car *c = (Car *)item->data;
        printf("Car - Brand: %s, Model: %s, Year: %d\n", c->brand, c->model, c->year);
    }
}

// Функция для освобождения памяти объекта
void free_item(ArrayItem *item) {
    free(item->data);
}

int main() {
    // Создаем массив ArrayItem для хранения указателей на объекты
    ArrayItem array[9];

    // Инициализируем массив
    array[0].type = PERSON;
    array[0].data = create_person("Alice", 30);

    array[1].type = PERSON;
    array[1].data = create_person("Bob", 25);

    array[2].type = PERSON;
    array[2].data = create_person("Charlie", 35);

    array[3].type = BOOK;
    array[3].data = create_book("1984", "George Orwell", 328);

    array[4].type = BOOK;
    array[4].data = create_book("Brave New World", "Aldous Huxley", 288);

    array[5].type = BOOK;
    array[5].data = create_book("Fahrenheit 451", "Ray Bradbury", 256);

    array[6].type = CAR;
    array[6].data = create_car("Toyota", "Camry", 2020);

    array[7].type = CAR;
    array[7].data = create_car("Honda", "Civic", 2018);

    array[8].type = CAR;
    array[8].data = create_car("Ford", "Mustang", 2021);

    // Читаем и выводим содержимое массива
    for (int i = 0; i < 9; i++) {
        if (array[i].data == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for item %d\n", i);
            continue;
        }
        print_item(&array[i]);
    }

    // Освобождаем память, выделенную под каждый объект
    for (int i = 0; i < 9; i++) {
        free_item(&array[i]);
    }

    return 0;
}

/*
Изменения и улучшения:
Проверка ошибок: Проверяется успешность выделения памяти.
Использование функций: Созданы функции для инициализации объектов, вывода информации и освобождения памяти.
Безопасность: Используются безопасные функции strncpy вместо strcpy.
Читаемость: Код разделен на логические части и снабжен комментариями для лучшего понимания.
*/
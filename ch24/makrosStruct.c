#include <stdio.h>
#include <string.h>

#define DEFINE_STRUCT(name, ...) \
    typedef struct { \
        __VA_ARGS__ \
    } name;

// Определение структуры Person с помощью макроса
DEFINE_STRUCT(Person,
              char name[50];
                      int age;
                      float height;
)

// Реализация функций для инициализации и печати структуры
void init_Person(Person* obj, const char* name, int age, float height) {
    snprintf(obj->name, sizeof(obj->name), "%s", name);
    obj->age = age;
    obj->height = height;
}

void print_Person(const Person* obj) {
    printf("Name: %s\n", obj->name);
    printf("Age: %d\n", obj->age);
    printf("Height: %.2f\n", obj->height);
}

int main() {
    Person p1;
    init_Person(&p1, "John Doe", 30, 175.5);
    print_Person(&p1);

    return 0;
}

/*

ъяснение кода
Макрос DEFINE_STRUCT_WITH_FUNCTIONS:

Определение структуры: typedef struct { __VA_ARGS__ } name; — определяет структуру с именем name и полями, заданными в __VA_ARGS__.
Функция инициализации: void init_##name(name* obj, __VA_ARGS__) { __VA_ARGS__ } — объявляет функцию инициализации для структуры с именем name. Она принимает указатель на структуру и параметры для инициализации.
Функция печати: void print_##name(const name* obj) { __VA_ARGS__ } — объявляет функцию для печати содержимого структуры. Она принимает указатель на структуру и выводит значения полей.
Определение структуры Person:

Макрос DEFINE_STRUCT_WITH_FUNCTIONS(Person, char name[50]; int age; float height;) определяет структуру Person и функции для её инициализации и печати.
Реализация функций:

Функции init_Person и print_Person реализованы отдельно от макроса, используя заданные поля.
Функция main:

Создаёт и инициализирует переменную структуры Person, а затем выводит её данные на экран.
*/
/*
 *
Таблицы функций (или таблицы виртуальных функций) — это мощный механизм, который позволяет организовывать вызов функций
 на основе определённого контекста или типа данных.
 В языке C они часто используются для реализации полиморфизма и подражания объектно-ориентированным принципам.
 Рассмотрим красивый пример использования таблиц функций для создания простой системы управления формами (например, прямоугольниками и кругами).

Пример: Система управления формами
 Представим, что у нас есть две формы: прямоугольник и круг.
 Мы хотим иметь возможность вычислять площадь и периметр каждой формы, но при этом использовать единый интерфейс для вызова соответствующих функций.
 * */

#include <stdio.h>
#include <math.h>

// Определяем структуру таблицы функций
typedef struct {
    double (*area)(void*);      // Функция для вычисления площади
    double (*perimeter)(void*); // Функция для вычисления периметра
} ShapeVTable;

// Прямоугольник
typedef struct {
    ShapeVTable* vtable; // Указатель на таблицу функций
    double width;
    double height;
} Rectangle;

// Круг
typedef struct {
    ShapeVTable* vtable; // Указатель на таблицу функций
    double radius;
} Circle;

// Функции для прямоугольника
double rectangleArea(void* self) {
    Rectangle* rect = (Rectangle*)self;
    return rect->width * rect->height;
}

double rectanglePerimeter(void* self) {
    Rectangle* rect = (Rectangle*)self;
    return 2 * (rect->width + rect->height);
}

// Функции для круга
double circleArea(void* self) {
    Circle* circle = (Circle*)self;
    return M_PI * circle->radius * circle->radius;
}

double circlePerimeter(void* self) {
    Circle* circle = (Circle*)self;
    return 2 * M_PI * circle->radius;
}

// Таблица функций для прямоугольника
ShapeVTable rectangleVTable = {
        .area = rectangleArea,
        .perimeter = rectanglePerimeter
};

// Таблица функций для круга
ShapeVTable circleVTable = {
        .area = circleArea,
        .perimeter = circlePerimeter
};

int main() {
    // Создаём прямоугольник
    Rectangle rect = { &rectangleVTable, 5.0, 3.0 };

    // Создаём круг
    Circle circle = { &circleVTable, 4.0 };

    // Вычисляем и выводим площадь и периметр прямоугольника
    printf("Прямоугольник:\n");
    printf("Площадь: %f\n", rect.vtable->area(&rect));
    printf("Периметр: %f\n", rect.vtable->perimeter(&rect));

    // Вычисляем и выводим площадь и периметр круга
    printf("Круг:\n");
    printf("Площадь: %f\n", circle.vtable->area(&circle));
    printf("Периметр: %f\n", circle.vtable->perimeter(&circle));

    return 0;
}

/*
 * Пояснение:
Таблица функций (ShapeVTable):

В этой структуре мы определяем два указателя на функции: area и perimeter. Эти функции принимают указатель на произвольную структуру (void*) и возвращают double.
Структуры для форм (Rectangle и Circle):

Каждая структура содержит указатель на соответствующую таблицу функций, а также специфичные для формы поля (ширина и высота для прямоугольника, радиус для круга).
Функции для вычисления площади и периметра:

Для каждой формы мы реализуем свои функции для вычисления площади и периметра.
Таблицы функций для каждой формы:

Мы создаем таблицы функций для прямоугольника и круга, где указываем на соответствующие функции для каждой формы.
Использование таблиц функций:

В main() мы создаем объекты для прямоугольника и круга, назначаем им таблицы функций и затем вызываем функции через эти таблицы.
 * */
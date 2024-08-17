/*
Указатели на функции:

    Самый распространённый способ реализовать что-то похожее на полиморфизм в C — использование указателей на функции,
    Этот подход гибкий и позволяет передавать разные функции для обработки в зависимости от контекста,
    что можно считать аналогом динамического диспетчинга в объектно-ориентированных языках.

1. Указатели на функции
    Предположим, у нас есть две фигуры: квадрат и круг.
    Мы хотим иметь возможность вычислять площадь этих фигур, используя указатели на функции.


Указатели на функции: обеспечивают максимальную гибкость и позволяют реализовать нечто,
похожее на виртуальные таблицы, эмулируя полиморфизм, характерный для объектно-ориентированных языков

*/

#include <stdio.h>

// Таблица функций (vtable) для фигур
typedef struct {
    double (*area)(void*);  // Указатель на функцию для вычисления площади
} ShapeVTable;

// Структура для квадрата
typedef struct {
    ShapeVTable* vtable;
    double side;
} Square;

// Структура для круга
typedef struct {
    ShapeVTable* vtable;
    double radius;
} Circle;

// Реализация функции для вычисления площади квадрата
double squareArea(void* self) {
    Square* square = (Square*)self;
    return square->side * square->side;
}

// Реализация функции для вычисления площади круга
double circleArea(void* self) {
    Circle* circle = (Circle*)self;
    return 3.14159 * circle->radius * circle->radius;
}

// Таблица функций для квадрата
ShapeVTable squareVTable = {
    .area = squareArea
};

// Таблица функций для круга
ShapeVTable circleVTable = {
    .area = circleArea
};

int main() {
    // Создаем квадрат и круг
    Square square = { &squareVTable, 5.0 };
    Circle circle = { &circleVTable, 3.0 };

    // Вычисляем и выводим площадь фигур
    printf("Площадь квадрата: %f\n", square.vtable->area(&square));
    printf("Площадь круга: %f\n", circle.vtable->area(&circle));

    return 0;
}

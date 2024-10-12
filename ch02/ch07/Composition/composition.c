/*
 Композиция
    Теперь создадим структуру, которая содержит общие данные для фигуры,
    и используем композицию для добавления специфических данных для квадрата и круга.

    Композиция: используется для организации кода и повторного использования общего кода, что является полезной техникой в C.
*/

#include <stdio.h>

// Общая структура для фигуры
typedef struct {
    double (*area)(void*);  // Указатель на функцию для вычисления площади
} Shape;

// Структура для квадрата
typedef struct {
    Shape shape;  // Композиция: общие данные фигуры
    double side;
} Square;

// Структура для круга
typedef struct {
    Shape shape;  // Композиция: общие данные фигуры
    double radius;
} Circle;

// Функция для вычисления площади квадрата
double squareArea(Square* square) {
    return square->side * square->side;
}

// Функция для вычисления площади круга
double circleArea(Circle* circle) {
    return 3.14159 * circle->radius * circle->radius;
}

int main() {
    // Создаем квадрат и круг с инициализацией функций
    Square square = { { (double(*)(void*))squareArea }, 5.0 };
    Circle circle = { { (double(*)(void*))circleArea }, 3.0 };

    // Вычисляем и выводим площадь фигур
    printf("Площадь квадрата: %f\n", square.shape.area(&square));
    printf("Площадь круга: %f\n", circle.shape.area(&circle));

    return 0;
}

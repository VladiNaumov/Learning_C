/*
2. Структуры и функции
    Здесь мы создадим общую структуру для хранения данных фигуры и определим отдельные функции для вычисления площади для каждой фигуры.
    Функции будут принимать указатель на структуру фигуры.


    Структуры и функции: это наиболее прямолинейный и типичный для C подход, где функции и данные разделены.
*/

#include <stdio.h>

// Структура для квадрата
typedef struct {
    double side;
} Square;

// Структура для круга
typedef struct {
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
    // Создаем квадрат и круг
    Square square = { 5.0 };
    Circle circle = { 3.0 };

    // Вычисляем и выводим площадь фигур
    printf("Площадь квадрата: %f\n", squareArea(&square));
    printf("Площадь круга: %f\n", circleArea(&circle));

    return 0;
}

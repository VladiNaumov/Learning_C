/*
 * Таблицы функций (или виртуальные таблицы) — это один из способов реализации полиморфизма в языках программирования,
 * особенно в тех, которые не поддерживают полиморфизм на уровне языка (например, C).

Полиморфизм и Таблицы Функций
 Полиморфизм — это способность функции обрабатывать объекты разных типов с единым интерфейсом.
 В объектно-ориентированных языках, таких как C++, полиморфизм достигается через наследование и виртуальные функции.
 В C, где нет встроенной поддержки классов и наследования, таблицы функций служат способом вручную реализовать полиморфизм.

Как работают Таблицы Функций?
 Таблица функций содержит указатели на функции, которые соответствуют операциям, выполняемым объектом.
 Когда вы вызываете метод на объекте, программа находит правильную функцию в таблице и вызывает её.
 Это позволяет разным объектам (например, кругу и прямоугольнику) предоставлять разные реализации одной и той же операции (например, вычисление площади) через один и тот же интерфейс.

Пример Реализации Полиморфизма через Таблицы Функций
 Рассмотрим ещё раз пример с фигурами. В этом случае таблицы функций позволяют нам использовать один и тот же интерфейс для работы с разными типами фигур (кругами и прямоугольниками), даже если каждая фигура реализует свои функции для вычисления площади и периметра.
 *
 * */

#include <stdio.h>
#include <math.h>

// Структура для таблицы функций
typedef struct {
    double (*area)(void*);      // Функция для вычисления площади
    double (*perimeter)(void*); // Функция для вычисления периметра
} ShapeVTable;

// Структура для прямоугольника
typedef struct {
    ShapeVTable* vtable; // Указатель на таблицу функций
    double width;
    double height;
} Rectangle;

// Структура для круга
typedef struct {
    ShapeVTable* vtable; // Указатель на таблицу функций
    double radius;
} Circle;

// Реализация функции площади для прямоугольника
double rectangleArea(void* self) {
    Rectangle* rect = (Rectangle*)self;
    return rect->width * rect->height;
}

// Реализация функции периметра для прямоугольника
double rectanglePerimeter(void* self) {
    Rectangle* rect = (Rectangle*)self;
    return 2 * (rect->width + rect->height);
}

// Реализация функции площади для круга
double circleArea(void* self) {
    Circle* circle = (Circle*)self;
    return M_PI * circle->radius * circle->radius;
}

// Реализация функции периметра для круга
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

/* Поле структуры хранит указатель на функцию  */

#include <stdio.h>

// Объявляем тип функции, которая принимает два целых числа и возвращает целое
typedef int (*operation_func)(int, int);

// Функции для выполнения операций
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

// Структура, содержащая указатель на функцию
struct Calculator {
    operation_func operation;
};

int main() {
    struct Calculator calc;

    // Назначаем функцию сложения
    calc.operation = add;
    printf("Add: %d\n", calc.operation(5, 3));

    // Назначаем функцию вычитания
    calc.operation = subtract;
    printf("Subtract: %d\n", calc.operation(5, 3));

    return 0;
}

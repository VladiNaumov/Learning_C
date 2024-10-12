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

// Структура, содержащая указатель на функцию и поле для хранения результата
struct Calculator {
    operation_func operation;  // Указатель на функцию
    int result;                // Поле для хранения результата вычисления
};

// Функция для выполнения операции и сохранения результата
void perform_operation(struct Calculator* calc, int a, int b) {
    calc->result = calc->operation(a, b);  // Вычисление и сохранение результата
}

int main() {
    struct Calculator calc;

    // Назначаем функцию сложения и выполняем операцию
    calc.operation = add;
    perform_operation(&calc, 5, 3);
    printf("Add result: %d\n", calc.result);

    // Назначаем функцию вычитания и выполняем операцию
    calc.operation = subtract;
    perform_operation(&calc, 5, 3);
    printf("Subtract result: %d\n", calc.result);

    return 0;
}

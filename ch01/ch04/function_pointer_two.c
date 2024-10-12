#include <stdio.h>

// Функция для сложения
int add(int a, int b) {
    return a + b;
}

// Функция для вычитания
int subtract(int a, int b) {
    return a - b;
}

// Функция для умножения
int multiply(int a, int b) {
    return a * b;
}

// Функция для деления
int divide(int a, int b) {
    if (b != 0)
        return a / b;
    else {
        printf("Error: Division by zero!\n");
        return 0;
    }
}

int main() {
    // Объявление указателя на функцию, которая принимает два целых числа и возвращает целое число
    int (*operation)(int, int);

    // Переменные для хранения операндов и результата
    int x = 10, y = 5, result;

    // Пример вызова функций через указатель
    operation = &add; // Можно также написать просто operation = add;
    result = operation(x, y);
    printf("Addition: %d + %d = %d\n", x, y, result);

    operation = &subtract; // Можно также написать просто operation = subtract;
    result = operation(x, y);
    printf("Subtraction: %d - %d = %d\n", x, y, result);

    operation = &multiply; // Можно также написать просто operation = multiply;
    result = operation(x, y);
    printf("Multiplication: %d * %d = %d\n", x, y, result);

    operation = &divide; // Можно также написать просто operation = divide;
    result = operation(x, y);
    printf("Division: %d / %d = %d\n", x, y, result);

    return 0;
}

/*
Объяснение кода:
Четыре функции для разных операций:

add, subtract, multiply, и divide — функции, которые выполняют основные арифметические операции.
Указатель на функцию:

int (*operation)(int, int); — объявляется указатель на функцию, которая принимает два целых числа и возвращает целое число. Этот указатель будет использоваться для вызова различных математических операций.
Присваивание указателя и вызов функции:

Сначала указателю operation присваивается адрес функции add, после чего она вызывается с параметрами x и y. Это же повторяется для всех других операций (subtract, multiply, divide).
Например, operation = &subtract; присваивает указателю адрес функции subtract, и затем вызов operation(x, y); эквивалентен вызову subtract(x, y);.
Вывод результата:

После каждого вызова функции результат операции выводится на экран.
*/
#include <stdio.h>

// Определение структуры с указателем на функцию, результатом и полем private
struct Operation {
    int (*operationFunc)(int, int);  // Указатель на функцию
    void *private;                   // Указатель на любые данные
    int result;                      // Поле для хранения результата выполнения функции
};

// Пример функции сложения
int add(int a, int b) {
    return a + b;
}

// Пример функции умножения
int multiply(int a, int b) {
    return a * b;
}

// Пример функции, которая работает с private данными
int add_with_private(int a, int b, int *extra) {
    return add(a, b) + *extra;
}

int main() {
    // Создаем переменную структуры Operation для сложения
    struct Operation opAdd;
    opAdd.operationFunc = add;
    opAdd.private = NULL;  // Не используем private в этом случае

    // Создаем переменную структуры Operation для умножения
    struct Operation opMultiply;
    opMultiply.operationFunc = multiply;
    opMultiply.private = NULL;

    // Вызов функций через указатели и сохранение результата
    opAdd.result = opAdd.operationFunc(3, 4);            // вызов функции add(3, 4) и сохранение результата
    opMultiply.result = opMultiply.operationFunc(3, 4);  // вызов функции multiply(3, 4) и сохранение результата

    // Использование private данных
    int extra_value = 10;
    struct Operation opAddWithPrivate;
    opAddWithPrivate.operationFunc = NULL;
    opAddWithPrivate.private = &extra_value;  // Сохраняем указатель на private данные (extra_value)

    // Сохранение результата с использованием private данных
    opAddWithPrivate.result = add_with_private(3, 4, &extra_value);

    // Вывод результатов
    printf("Addition result: %d\n", opAdd.result);                  // Результат сложения: 7
    printf("Multiplication result: %d\n", opMultiply.result);        // Результат умножения: 12
    printf("Addition with private result: %d\n", opAddWithPrivate.result); // Результат сложения с private: 17

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

// Макрос для обхода двумерного массива с проверкой границ
#define FOR_EACH_ELEMENT_2D(array, rows, cols, action) \
    for (size_t i = 0; i < (rows); ++i) { \
        for (size_t j = 0; j < (cols); ++j) { \
            action(array[i][j]); \
        } \
        printf("\n"); \
    }

// Функция для демонстрации работы макроса
void print_element(int element) {
    printf("%d ", element);
}

void print_array(int arr[][3], size_t rows, size_t cols) {
    FOR_EACH_ELEMENT_2D(arr, rows, cols, print_element);
}

int main() {
    // Пример двумерного массива
    int array[3][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
    };

    size_t rows = sizeof(array) / sizeof(array[0]);
    size_t cols = sizeof(array[0]) / sizeof(array[0][0]);

    // Вызов функции для печати массива
    print_array(array, rows, cols);

    return 0;
}
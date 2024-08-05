#include <stdio.h>
#include <stdlib.h>

// Макрос для вычисления размера массива
#define ARRAY2D_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

// Макрос для обхода двухмерного массива с проверкой границ
#define FOR_EACH_ELEMENT_2D(array, rows, cols, element) \
    for (size_t i = 0; i < (rows); ++i) \
        for (size_t j = 0; j < (cols); ++j) \
            if (i < (rows) && j < (cols)) \
                element = (array)[i][j]

// Функция для демонстрации работы макроса
void print_array(int arr[][3], size_t rows, size_t cols) {
    int element;

    FOR_EACH_ELEMENT_2D(arr, rows, cols, element) {
        printf("%d ", element);
        if (j == cols - 1) {
            printf("\n");
        }
    }
}

int main() {
    // Пример двухмерного массива
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

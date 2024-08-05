#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define for_each_element(element, array) \
    for (size_t i = 0; i < ARRAY_SIZE(array); ++i) \
        for (int once = 1; once--; element = array[i])

int main() {
    int numbers[] = {1, 2, 3, 4, 5};
    int element;

    for_each_element(element, numbers) {
        printf("%d\n", element);
    }

    return 0;
}

/*
Здесь макрос for_each_element позволяет обходить элементы массива numbers,
используя внутренний цикл for, который выполняется один раз за итерацию, и подставляет элемент массива в переменную element.

*/


#define PRINT_ARRAY(arr) \
    for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) \
        printf("%d\n", arr[i])

int main() {
    int numbers1[] = {1, 2, 3, 4, 5};
    int numbers2[] = {10, 20, 30, 40, 50};

    PRINT_ARRAY(numbers1);
    PRINT_ARRAY(numbers2);

    return 0;
}



/*
Здесь макрос SAFE_ACCESS обеспечивает проверку границ массива
 и предотвращает доступ к элементам за пределами допустимого диапазона.
 */
 
#define SAFE_ACCESS(arr, index, size) \
    ((index) < (size) ? (arr)[(index)] : (fprintf(stderr, "Index out of bounds: %zu\n", (index)), exit(EXIT_FAILURE)))

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    size_t size = sizeof(arr) / sizeof(arr[0]);

    for (size_t i = 0; i < size + 1; ++i) {
        printf("%d\n", SAFE_ACCESS(arr, i, size));
    }

    return 0;
}
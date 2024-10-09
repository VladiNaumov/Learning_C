#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Функция для проверки, является ли строка числом
int extract_number(const char *str, char *number) {
    int i = 0, j = 0;

    // Обрабатываем возможный минус в начале числа
    if (str[i] == '-') {
        number[j++] = str[i++];
    }

    // Извлекаем цифры из строки
    while (str[i]) {
        if (isdigit((unsigned char)str[i])) {
            number[j++] = str[i];
        } else {
            break;
        }
        i++;
    }
    number[j] = '\0';
    return j > 0; // Возвращаем 1, если было извлечено хотя бы одно число
}

// Функция для получения и очистки пользовательского ввода
void get_input(char *input, size_t size) {
    printf("Введите данные: ");
    if (fgets(input, size, stdin)) {
        // Удаляем символ новой строки, если он есть
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
    }
}

// Функция для обработки пользовательского ввода
void process_input() {
    char input[100];
    char number[100];

    while (1) {
        get_input(input, sizeof(input));
        if (extract_number(input, number)) {
            printf("Введено число: %s\n", number);
        } else {
            printf("Это не число. Введите данные еще раз: ");
        }
    }
}

int main() {
    process_input();
    return 0;
}

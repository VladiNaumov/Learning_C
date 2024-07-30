#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Функция для извлечения числа из строки
int extract_number(const char *str, char *number) {
    int j = 0;
    int has_digit = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit((unsigned char)str[i])) {
            number[j++] = str[i];
            has_digit = 1;
        }
    }

    number[j] = '\0';
    return has_digit;
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

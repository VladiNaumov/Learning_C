#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int is_number(const char *str) {
    if (*str == '-') {
        str++;
    }
    if (*str == '\0') {
        return 0;
    }
    while (*str) {
        if (!isdigit((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

int main() {
    char input[100];

    printf("Введите данные: ");
    while (scanf("%99s", input) == 1) {
        if (is_number(input)) {
            printf("Введено число: %s\n", input);
        } else {
            printf("Это не число. Введите любое число: ");
        }
    }

    return 0;
}

/*
Объяснение:
Функция is_number:

Проверяет, является ли строка числом.
Поддерживает отрицательные числа и цифры.
Основная логика:

Пользователь вводит данные.
Если данные соответствуют числу, выводится это число.
Если введены нечисловые данные, программа сообщает об этом и запрашивает ввод снова.
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Функция для проверки, является ли строка числом
int extract_number(const char *str, char *number);

// Функция для получения ввода от пользователя
void get_input(char *input, size_t size);

// Функция для обработки ввода
void process_input();

int main() {
    process_input();
    return 0;
}

int extract_number(const char *str, char *number) {
    int i = 0, j = 0;
    if (str[i] == '-') {
        number[j++] = str[i++];
    }
    while (str[i]) {
        if (isdigit((unsigned char)str[i])) {
            number[j++] = str[i];
        } else {
            break;
        }
        i++;
    }
    number[j] = '\0';
    return j > 0;
}

void get_input(char *input, size_t size) {
    printf("Введите данные: ");
    scanf("%99s", input);
}

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


/*
Объяснение:
Функция extract_number:

Извлекает число из строки.
Поддерживает отрицательные числа.
Возвращает 1, если число было извлечено, и 0, если нет.
Основная логика:

Пользователь вводит данные.
Если данные содержат число, программа выводит его.
Если число не найдено, программа сообщает об этом и запрашивает ввод снова.

*/
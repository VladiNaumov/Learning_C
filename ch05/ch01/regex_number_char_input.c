#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// Функция для извлечения числа из строки с использованием регулярного выражения
int extract_number(const char *str, char *number) {
    regex_t regex;
    regmatch_t match[1];
    const char *pattern = "[0-9]+";

    // Компилируем регулярное выражение
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Ошибка компиляции регулярного выражения.\n");
        return 0;
    }

    // Выполняем поиск в строке
    if (regexec(&regex, str, 1, match, 0) == 0) {
        size_t len = match[0].rm_eo - match[0].rm_so;
        strncpy(number, str + match[0].rm_so, len);
        number[len] = '\0';
        regfree(&regex);
        return 1;
    }

    regfree(&regex);
    return 0;
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


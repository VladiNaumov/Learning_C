/*
 * создадим пример, который демонстрирует хеширование команд и обработку команд с использованием таблицы указателей на функции. 
 Мы сделаем простой калькулятор, который будет выполнять арифметические операции по командам.

Пример реализации
Определение команд: Мы создадим несколько команд для выполнения простых арифметических операций: сложение, вычитание и умножение.
Хеширование команд: Мы будем использовать хеш-функцию для получения хеша команды.
Таблица указателей на функции: Мы создадим таблицу указателей на функции, чтобы легко вызывать нужные функции обработки команд.

 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CMD_ADD "ADD"
#define CMD_SUBTRACT "SUBTRACT"
#define CMD_MULTIPLY "MULTIPLY"

// Хеш-функция для строк
unsigned int hash_command(const char *command) {
    unsigned int hash = 0;
    while (*command) {
        hash += (unsigned char)(*command);
        command++;
    }
    return hash;
}

// Определение функций для обработки команд
void handle_add(const char *args) {
    int a, b;
    if (sscanf(args, "%d %d", &a, &b) != 2) {
        fprintf(stderr, "Invalid arguments for ADD. Use: ADD <number> <number>\n");
        return;
    }
    printf("Result of ADD: %d\n", a + b);
}

void handle_subtract(const char *args) {
    int a, b;
    if (sscanf(args, "%d %d", &a, &b) != 2) {
        fprintf(stderr, "Invalid arguments for SUBTRACT. Use: SUBTRACT <number> <number>\n");
        return;
    }
    printf("Result of SUBTRACT: %d\n", a - b);
}

void handle_multiply(const char *args) {
    int a, b;
    if (sscanf(args, "%d %d", &a, &b) != 2) {
        fprintf(stderr, "Invalid arguments for MULTIPLY. Use: MULTIPLY <number> <number>\n");
        return;
    }
    printf("Result of MULTIPLY: %d\n", a * b);
}

// Структура для хранения команды и соответствующей функции
typedef void (*CommandHandler)(const char *);
typedef struct {
    unsigned int hash;
    CommandHandler handler;
    const char *command_str;
} CommandEntry;

// Таблица команд
CommandEntry command_table[3]; // Задаём размер массива

void init_command_table() {
    command_table[0] = (CommandEntry){ hash_command(CMD_ADD), handle_add, CMD_ADD };
    command_table[1] = (CommandEntry){ hash_command(CMD_SUBTRACT), handle_subtract, CMD_SUBTRACT };
    command_table[2] = (CommandEntry){ hash_command(CMD_MULTIPLY), handle_multiply, CMD_MULTIPLY };
}

const size_t command_table_size = sizeof(command_table) / sizeof(command_table[0]);

// Обработка команд
void process_command(const char *command_str, const char *args) {
    unsigned int hash = hash_command(command_str);

    for (size_t i = 0; i < command_table_size; i++) {
        if (command_table[i].hash == hash && strcmp(command_str, command_table[i].command_str) == 0) {
            command_table[i].handler(args);
            return;
        }
    }

    // Обработка неизвестной команды
    fprintf(stderr, "Unknown command: %s\n", command_str);
}

int main() {
    // Инициализация таблицы команд
    init_command_table();

    // Пример использования
    char command[20];
    char args[100]; // Увеличили размер для аргументов

    // Ввод команды и аргументов
    printf("Enter command (ADD, SUBTRACT, MULTIPLY) followed by two numbers:\n");
    while (1) {
        printf("> ");

        // Считываем целую строку ввода
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; // Прерываем цикл при ошибке ввода
        }

        // Убираем символ новой строки
        command[strcspn(command, "\n")] = 0;

        // Разделяем строку на команду и аргументы
        char *args_start = strchr(command, ' ');
        if (args_start) {
            *args_start = '\0'; // Разделяем строку на команду и аргументы
            strcpy(args, args_start + 1); // Копируем аргументы
        } else {
            args[0] = '\0'; // Нет аргументов
        }

        // Обработка команды
        process_command(command, args);
    }

    return 0;
}


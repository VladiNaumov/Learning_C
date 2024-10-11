#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ctype.h>

#pragma comment(lib, "ws2_32.lib")  // Подключаем библиотеку для работы с сокетами

#define PORT 8080
#define MAX_INPUT_SIZE 256

typedef struct {
    char command[16];
    float numbers[2];  // Всего два числа
} BinaryMessage;

// Функция для сериализации сообщения
void serialize_message(BinaryMessage *msg, char *buffer) {
    memcpy(buffer, msg->command, sizeof(msg->command));
    memcpy(buffer + sizeof(msg->command), msg->numbers, sizeof(msg->numbers));
}

// Функция для инициализации сокета
SOCKET initialize_socket() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Error creating socket.\n");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed.\n");
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    return sock;
}

// Функция для проверки, является ли строка числом
int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str) && *str != '.' && *str != '-') return 0; // Проверка цифр, точки и знака
        str++;
    }
    return 1;
}

// Функция для ввода и обработки данных от пользователя
BinaryMessage get_user_input() {
    char input[MAX_INPUT_SIZE];
    BinaryMessage msg;

    printf("Enter the command and numbers in the format: PLUS, MINUS, MULTIPLICATION, DIVISION&2&3...\n");
    fgets(input, sizeof(input), stdin);

    // Разделение строки по символу '&'
    char *token = strtok(input, "&");
    if (token != NULL) {
        // Проверка на корректность команды
        if (strcmp(token, "PLUS") != 0 && strcmp(token, "MINUS") != 0 &&
            strcmp(token, "MULTIPLICATION") != 0 && strcmp(token, "DIVISION") != 0) {
            printf("Error: invalid command. Only PLUS, MINUS, MULTIPLICATION, and DIVISION are allowed.\n");
            exit(EXIT_FAILURE);
        }
        strncpy(msg.command, token, sizeof(msg.command));
        msg.command[sizeof(msg.command) - 1] = '\0';  // Обеспечение нуль-терминатора

        // Проверка первого числа
        token = strtok(NULL, "&");
        if (token == NULL || !is_number(token)) {
            printf("Error: invalid first number.\n");
            exit(EXIT_FAILURE);
        }
        msg.numbers[0] = atof(token);  // Первое число

        // Проверка второго числа
        token = strtok(NULL, "&");
        if (token == NULL || !is_number(token)) {
            printf("Error: invalid second number.\n");
            exit(EXIT_FAILURE);
        }
        msg.numbers[1] = atof(token);  // Второе число
    } else {
        printf("Error: invalid input format. Use COMMAND&NUM1&NUM2.\n");
        exit(EXIT_FAILURE);
    }

    return msg;
}

// Функция для отправки и получения данных
void send_and_receive(SOCKET sock, BinaryMessage *msg) {
    char buffer[sizeof(BinaryMessage)];
    serialize_message(msg, buffer);

    send(sock, buffer, sizeof(buffer), 0);

    // Получение результата
    float result;
    recv(sock, (char*)&result, sizeof(result), 0);
    printf("Result: %f\n", result);
}

// Главная функция
int main() {
    SOCKET sock = initialize_socket();
    BinaryMessage msg = get_user_input();
    send_and_receive(sock, &msg);

    closesocket(sock);
    WSACleanup();

    return 0;
}

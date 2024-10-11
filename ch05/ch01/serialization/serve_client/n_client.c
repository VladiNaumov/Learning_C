#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Подключаем библиотеку для работы с сокетами

#define PORT 8080
#define CMD_PLUS 1
#define CMD_MINUS 2
#define CMD_MULTIPLY 3
#define CMD_DIVIDE 4

#define MAX_BUFFER 1024

// Структура для бинарного сообщения, которую будем отправлять на сервер
typedef struct {
    int command_id;  // Идентификатор команды (1: PLUS, 2: MINUS, 3: MULTIPLY, 4: DIVIDE)
    float a;         // Первое число
    float b;         // Второе число
} BinaryMessage;

// Функция для сериализации сообщения (преобразование в бинарный формат для отправки)
void serialize_message(BinaryMessage *msg, char *buffer) {
    memcpy(buffer, &msg->command_id, sizeof(int));  // Копируем команду
    memcpy(buffer + sizeof(int), &msg->a, sizeof(float));  // Копируем первое число
    memcpy(buffer + sizeof(int) + sizeof(float), &msg->b, sizeof(float));  // Копируем второе число
}

// Функция для получения и валидации ввода от пользователя
int get_input(int *command_id, float *a, float *b) {
    char input[50];  // Буфер для ввода данных
    printf("Enter the command ID (1: PLUS, 2: MINUS, 3: MULTIPLY, 4: DIVIDE) and two numbers in format: 1&23&6\n");
    
    // Используем fgets для безопасного считывания
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error: input reading failed.\n");
        return 0;  // Возвращаем код ошибки
    }

    // Удаление символа новой строки, если он был введён
    input[strcspn(input, "\n")] = 0;

    // Разделяем строку по символу '&'
    char *token = strtok(input, "&");
    if (token != NULL) {
        *command_id = atoi(token);  // Преобразуем команду в число

        // Проверка допустимого диапазона команд
        if (*command_id < CMD_PLUS || *command_id > CMD_DIVIDE) {
            printf("Error: invalid command ID.\n");
            return 0;  // Возвращаем код ошибки
        }

        // Получение первого числа
        token = strtok(NULL, "&");
        if (token != NULL) {
            *a = atof(token);

            // Получение второго числа
            token = strtok(NULL, "&");
            if (token != NULL) {
                *b = atof(token);
                return 1;  // Успешный ввод данных
            }
        }
    }

    // Если ввели неправильный формат
    printf("Error: invalid input format. Please use the format: 1&23&6\n");
    return 0;  // Возвращаем код ошибки
}


// Функция для подключения к серверу и отправки данных
void connect_and_send_data(int command_id, float a, float b) {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;

    // Инициализация Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Создание сокета
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // Подключение к серверу
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
        printf("Error: unable to connect to server.\n");
        closesocket(sock);
        WSACleanup();
        return;
    }

    // Создание сообщения
    BinaryMessage msg;
    msg.command_id = command_id;
    msg.a = a;
    msg.b = b;

    // Сериализация сообщения
    char buffer[sizeof(BinaryMessage)];
    serialize_message(&msg, buffer);

    // Отправка данных на сервер
    send(sock, buffer, sizeof(buffer), 0);

    // Получение результата от сервера
    float result;
    recv(sock, (char*)&result, sizeof(result), 0);
    printf("Result: %.2f\n", result);

    // Закрытие сокета и очистка ресурсов
    closesocket(sock);
    WSACleanup();
}

int main() {
    int command_id;
    float a, b;

    // Ввод данных до тех пор, пока пользователь не введёт корректный ввод
    while (!get_input(&command_id, &a, &b)) {
        printf("Please try again.\n");
    }

    // После успешного ввода данных выполняем отправку данных на сервер
    connect_and_send_data(command_id, a, b);

    // Задержка, чтобы консоль не закрылась сразу
    printf("Press any key to exit...\n");
    getchar();
    return 0;
}

/* многопоточного сервера-калькулятора Serialization/Deserialization */

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")  // Подключаем библиотеку для работы с сокетами

#define PORT 8080
#define MAX_CLIENTS 10

// Структура для бинарного сообщения
typedef struct {
    char command[16];  // Команда (например, "PLUS", "MINUS" ....)
    float a;           // Первое число
    float b;           // Второе число
} BinaryMessage;

// Функция для десериализации бинарного формата обратно в структуру
void deserialize_message(char *buffer, BinaryMessage *msg) {
    memcpy(msg->command, buffer, sizeof(msg->command)); // Получаем команду
    memcpy(&msg->a, buffer + sizeof(msg->command), sizeof(float)); // Получаем первое число
    memcpy(&msg->b, buffer + sizeof(msg->command) + sizeof(float), sizeof(float)); // Получаем второе число
}

// Функции для математических операций
float add(float a, float b) {
    return a + b;
}

float subtract(float a, float b) {
    return a - b;
}

float multiply(float a, float b) {
    return a * b;
}

float divide(float a, float b) {
    if (b != 0) {
        return a / b;
    } else {
        return -1; // Возвращаем -1 для обозначения ошибки деления на ноль
    }
}

// Функция для вывода сообщения об ошибке
void print_error(const char *message) {
    printf("Error: %s\n", message);
}

// Функция для вывода результата
void print_result(float result) {
    printf("Result: %.2f\n", result);
}

// Функция для обработки клиента
void handle_client(void* client_socket_ptr) {
    SOCKET client_sock = (SOCKET)(intptr_t)client_socket_ptr; // Приведение void* к SOCKET
    BinaryMessage msg; // Создаем структуру для сообщения
    char buffer[sizeof(BinaryMessage)]; // Буфер для получения данных

    // Получение данных от клиента
    recv(client_sock, buffer, sizeof(buffer), 0);
    deserialize_message(buffer, &msg); // Десериализуем данные

    float result = 0; // Переменная для хранения результата
    int error_occurred = 0; // Флаг для отслеживания ошибок

    // Выполняем операции в зависимости от команды
    if (strcmp(msg.command, "PLUS") == 0) {
        result = add(msg.a, msg.b); // Вызов функции сложения
    } else if (strcmp(msg.command, "MINUS") == 0) {
        result = subtract(msg.a, msg.b); // Вызов функции вычитания
    } else if (strcmp(msg.command, "MULTIPLICATION") == 0) {
        result = multiply(msg.a, msg.b); // Вызов функции умножения
    } else if (strcmp(msg.command, "DIVISION") == 0) {
        result = divide(msg.a, msg.b); // Вызов функции деления
        if (result == -1) {
            print_error("Division by zero."); // Обработка ошибки деления на ноль
            error_occurred = 1; // Устанавливаем флаг ошибки
        }
    } else {
        print_error("Invalid command."); // Обработка неверной команды
        error_occurred = 1; // Устанавливаем флаг ошибки
    }

    // Если ошибок не произошло, отправляем результат
    if (!error_occurred) {
        print_result(result); // Выводим результат на сервер
        send(client_sock, (char*)&result, sizeof(result), 0); // Отправка результата обратно клиенту
    }

    // Закрываем сокет клиента
    closesocket(client_sock); 
}

// Главная функция для запуска сервера
int main() {
    WSADATA wsaData; // Данные для инициализации Winsock
    SOCKET server_socket, client_socket; // Сокеты сервера и клиента
    struct sockaddr_in server_addr, client_addr; // Структуры для адреса сервера и клиента
    int addr_len = sizeof(client_addr); // Длина адреса клиента

    // Инициализация Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Создание сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Все интерфейсы
    server_addr.sin_port = htons(PORT); // Порт

    // Привязка сокета к адресу
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS); // Прослушивание входящих подключений

    printf("The server is running and waiting for connections...\n");

    while(1) {
        // Принятие входящего подключения
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        printf("Client connected!\n");

        // Создание потока для обработки клиента
        _beginthread(handle_client, 0, (void*)(intptr_t)client_socket);
    }

    // Закрытие серверного сокета
    closesocket(server_socket);
    WSACleanup();
    return 0;
}

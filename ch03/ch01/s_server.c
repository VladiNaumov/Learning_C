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
    char command[16];  // Команда (например, "PLUS", "MINUS")
    float numbers[10]; // Массив чисел (количество может быть переменным)
    int count;         // Количество чисел
} BinaryMessage;

// Функция для сериализация структуры в бинарный формат
void serialize_message(BinaryMessage *msg, char *buffer) {
    memcpy(buffer, msg->command, sizeof(msg->command)); // Копируем команду
    memcpy(buffer + sizeof(msg->command), &msg->count, sizeof(int)); // Копируем количество чисел
    memcpy(buffer + sizeof(msg->command) + sizeof(int), msg->numbers, msg->count * sizeof(float)); // Копируем числа
}

// Функция для десериализации бинарного формата обратно в структуру
void deserialize_message(char *buffer, BinaryMessage *msg) {
    memcpy(msg->command, buffer, sizeof(msg->command)); // Получаем команду
    memcpy(&msg->count, buffer + sizeof(msg->command), sizeof(int)); // Получаем количество чисел
    memcpy(msg->numbers, buffer + sizeof(msg->command) + sizeof(int), msg->count * sizeof(float)); // Получаем числа
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

    // Выполняем операции в зависимости от команды
    if (strcmp(msg.command, "PLUS") == 0) {
        if (msg.count == 2) { // Проверяем, что передано ровно два числа
            result = msg.numbers[0] + msg.numbers[1]; // Сложение двух чисел
        } else {
            printf("Error: PLUS requires exactly two numbers.\n");
            result = 0; // Или любая другая логика обработки ошибки
        }
    } else if (strcmp(msg.command, "MINUS") == 0) {
        if (msg.count == 2) { // Проверяем, что передано ровно два числа
            result = msg.numbers[0] - msg.numbers[1]; // Вычитание двух чисел
        } else {
            printf("Error: MINUS requires exactly two numbers.\n");
            result = 0; // Или любая другая логика обработки ошибки
        }
    } else if (strcmp(msg.command, "MULTIPLICATION") == 0) {
        if (msg.count == 2) { // Проверяем, что передано ровно два числа
            result = msg.numbers[0] * msg.numbers[1]; // Умножение двух чисел
        } else {
            printf("Error: MULTIPLICATION requires exactly two numbers.\n");
            result = 0; // Или любая другая логика обработки ошибки
        }
    } else if (strcmp(msg.command, "DIVISION") == 0) {
        if (msg.count == 2) { // Проверяем, что передано ровно два числа
            if (msg.numbers[1] != 0) { // Проверяем деление на ноль
                result = msg.numbers[0] / msg.numbers[1]; // Деление двух чисел
            } else {
                printf("Error: DIVISION by zero.\n");
                result = 0; // Или любая другая логика обработки ошибки
            }
        } else {
            printf("Error: DIVISION requires exactly two numbers.\n");
            result = 0; // Или любая другая логика обработки ошибки
        }
    } else {
        printf("Error: Invalid command.\n");
    }

    // Отправка результата обратно клиенту
    send(client_sock, (char*)&result, sizeof(result), 0);
    closesocket(client_sock); // Закрываем сокет клиента
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


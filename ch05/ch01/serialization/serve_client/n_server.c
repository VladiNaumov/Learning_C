/* Использование числовое значение в качестве команды вместо строк. */

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")  // Подключаем библиотеку для работы с сокетами

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_THREADS 4 // Максимальное количество потоков

#define CMD_PLUS 1
#define CMD_MINUS 2
#define CMD_MULTIPLY 3
#define CMD_DIVIDE 4

HANDLE semaphore; // Семафор для контроля потоков

// Структура для бинарного сообщения
typedef struct {
    int command_id;    // Числовая команда (например, CMD_PLUS = 1, CMD_MINUS = 2, ...)
    float a;        // Первое число
    float b;        // Второе число
} BinaryMessage;

// Функция для десериализации бинарного формата обратно в структуру
void deserialize_message(char *buffer, BinaryMessage *msg) {
    memcpy(&msg->command_id, buffer, sizeof(int)); // Получаем числовую команду
    memcpy(&msg->a, buffer + sizeof(int), sizeof(float)); // Получаем первое число
    memcpy(&msg->b, buffer + sizeof(int) + sizeof(float), sizeof(float)); // Получаем второе число
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
    return (b != 0) ? a / b : -1; // Возвращаем -1 для обозначения ошибки деления на ноль
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
    // Захватываем слот в семафоре
    WaitForSingleObject(semaphore, INFINITE);

    SOCKET client_sock = (SOCKET)(intptr_t)client_socket_ptr; // Приведение void* к SOCKET
    BinaryMessage msg; // Создаем структуру для сообщения
    char buffer[sizeof(BinaryMessage)]; // Буфер для получения данных

    // Получение данных от клиента
    int recv_len = recv(client_sock, buffer, sizeof(buffer), 0);

    if (recv_len <= 0 || recv_len != sizeof(BinaryMessage)) {
        printf("Error receiving data from client. Length: %d\n", recv_len);
        closesocket(client_sock);
        ReleaseSemaphore(semaphore, 1, NULL); // Освобождаем слот в семафоре
        return;
    }

    // Десериализация полученного сообщения
    memcpy(&msg, buffer, sizeof(msg));

    // Проверка валидности команды
    if (msg.command_id < CMD_PLUS || msg.command_id > CMD_DIVIDE) {
        printf("Error: invalid command_id ID received: %d\n", msg.command_id);
        float error_result = -1;  // Можно отправить специальный код ошибки
        send(client_sock, (char*)&error_result, sizeof(error_result), 0);
        closesocket(client_sock);
        ReleaseSemaphore(semaphore, 1, NULL); // Освобождаем слот в семафоре
        return;
    }

    float result = 0; // Переменная для хранения результата
    int error_occurred = 0; // Флаг для отслеживания ошибок

    // Выполняем операции в зависимости от команды
    switch (msg.command_id) {
        case CMD_PLUS:
            result = add(msg.a, msg.b);
            break;
        case CMD_MINUS:
            result = subtract(msg.a, msg.b);
            break;
        case CMD_MULTIPLY:
            result = multiply(msg.a, msg.b);
            break;
        case CMD_DIVIDE:
            if (msg.b == 0) {
                print_error("Division by zero.");
                error_occurred = 1; // Устанавливаем флаг ошибки
                result = -1;  // Возвращаем -1 в случае деления на ноль
            } else {
                result = divide(msg.a, msg.b);
            }
            break;
    }

    // Если ошибок не произошло, отправляем результат
    if (!error_occurred) {
        print_result(result);
        send(client_sock, (char*)&result, sizeof(result), 0); // Отправка результата обратно клиенту
    } else {
        send(client_sock, (char*)&result, sizeof(result), 0); // Отправка кода ошибки обратно клиенту
    }

    // Закрываем сокет клиента
    closesocket(client_sock);
    ReleaseSemaphore(semaphore, 1, NULL); // Освобождаем слот в семафоре
}


// Функция для инициализации сокета сервера
SOCKET initialize_server_socket() {
    SOCKET server_socket;
    struct sockaddr_in server_addr;

    // Создание сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Error creating socket.\n");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязка сокета
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(server_socket);
        exit(EXIT_FAILURE);
    }

    // Прослушивание подключений
    if (listen(server_socket, MAX_CLIENTS) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(server_socket);
        exit(EXIT_FAILURE);
    }

    return server_socket;
}

// Функция для инициализации семафора
void initialize_semaphore() {
    semaphore = CreateSemaphore(NULL, MAX_THREADS, MAX_THREADS, NULL);
    if (semaphore == NULL) {
        printf("Semaphore initialization failed.\n");
        exit(EXIT_FAILURE);
    }
}

// Функция для ожидания и обработки подключений клиентов
void wait_for_clients(SOCKET server_socket) {
    SOCKET client_socket;
    struct sockaddr_in client_addr;
    int addr_len = sizeof(client_addr);

    printf("The server is running and waiting for connections...\n");

    while (1) {
        // Приём подключения клиента
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket == INVALID_SOCKET) {
            printf("Error accepting connection.\n");
            continue;
        }

        printf("Client connected!\n");

        // Создание потока для обработки клиента
        _beginthread(handle_client, 0, (void*)(intptr_t)client_socket);
    }
}

// Главная функция
int main() {
    WSADATA wsaData;
    SOCKET server_socket;

    // Инициализация Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Инициализация семафора
    initialize_semaphore();

    // Инициализация сокета сервера
    server_socket = initialize_server_socket();

    // Ожидание и обработка подключений клиентов
    wait_for_clients(server_socket);

    // Завершение работы сокета
    closesocket(server_socket);
    WSACleanup();

    return 0;
}




#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <ctype.h>

#pragma comment(lib, "ws2_32.lib")  // Подключение библиотеки для работы с сокетами

// Объявляем операции с хешами для математических операций
typedef enum {
    OP_PLUS = 5381,
    OP_MINUS = 5863,
    OP_MULTIPLICATION = 5981,
    OP_DIVISION = 5807,
    OP_INVALID = 0
} Operation;

// Реализация операций сложения, вычитания, умножения и деления
double op_plus(double num1, double num2, int *error) {
    return num1 + num2;
}

double op_minus(double num1, double num2, int *error) {
    return num1 - num2;
}

double op_multiplication(double num1, double num2, int *error) {
    return num1 * num2;
}

double op_division(double num1, double num2, int *error) {
    if (num2 != 0) {
        return num1 / num2;
    } else {
        *error = 1;  // Ошибка при делении на ноль
        return 0;
    }
}

// Массив указателей на функции операций
typedef double (*OperationFunc)(double, double, int *);

OperationFunc operations[] = {
        op_plus,
        op_minus,
        op_multiplication,
        op_division
};

// Функция для хеширования строки (названия операции)
unsigned long hash_operation(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;  // Алгоритм DJB2
    }
    return hash;
}

// Функция для выполнения вычисления
double calculate(Operation op, double num1, double num2, int *error) {
    if (op != OP_INVALID) {
        return operations[op % 4](num1, num2, error);  // Выбор операции по хешу
    } else {
        *error = 1;  // Ошибка, если операция недействительна
        return 0;
    }
}

// Парсинг ввода от клиента, извлечение операции и чисел
int parse_input(const char *buffer, Operation *op, double *num1, double *num2) {
    char operation[20];
    if (buffer == NULL || *buffer == '\0') {
        return 0;  // Проверка на пустой ввод
    }
    if (sscanf_s(buffer, "%19[^&]&%lf&%lf", operation, num1, num2) != 3) {
        return 0;  // Проверка корректности формата
    }
    *op = (Operation)hash_operation(operation);  // Хешируем операцию
    if (*op == OP_INVALID) return 0;  // Проверка на валидность операции
    return 1;
}

// Функция потока для обработки клиента
DWORD WINAPI handle_client(LPVOID client_socket) {
    SOCKET client_sock = *(SOCKET*)client_socket;
    char buffer[1024];
    int recv_size = recv(client_sock, buffer, sizeof(buffer) - 1, 0);  // Получаем данные
    if (recv_size == SOCKET_ERROR) {
        closesocket(client_sock);
        return 1;
    }
    buffer[recv_size] = '\0';  // Добавляем терминальный ноль

    Operation op;
    double num1, num2;
    int error = 0;

    // Парсим ввод
    if (!parse_input(buffer, &op, &num1, &num2)) {
        const char *error_msg = "Invalid input\n";
        send(client_sock, error_msg, strlen(error_msg), 0);  // Отправляем сообщение об ошибке
        closesocket(client_sock);
        return 1;
    }

    // Вычисление результата
    double result = calculate(op, num1, num2, &error);
    if (error) {
        const char *error_msg = "Error in calculation\n";
        send(client_sock, error_msg, strlen(error_msg), 0);  // Отправляем ошибку расчета
    } else {
        char result_buffer[50];
        sprintf(result_buffer, "%.2lf", result);  // Формируем строку с результатом
        send(client_sock, result_buffer, strlen(result_buffer), 0);  // Отправляем результат
    }

    closesocket(client_sock);  // Закрываем сокет клиента
    return 0;
}

// Функция для инициализации сервера
SOCKET initialize_server(int port) {
    WSADATA wsa;
    SOCKET server_socket;
    struct sockaddr_in server;

    // Инициализация Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return INVALID_SOCKET;
    }

    // Создаем сокет
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Устанавливаем параметры адреса
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // Привязываем сокет к адресу и порту
    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(server_socket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    listen(server_socket, 10);  // Начинаем прослушивание
    return server_socket;
}

// Функция для ожидания клиента
SOCKET accept_client(SOCKET server_socket) {
    struct sockaddr_in client;
    int client_len = sizeof(struct sockaddr_in);
    return accept(server_socket, (struct sockaddr*)&client, &client_len);  // Принимаем подключение клиента
}

// Функция для обработки входящих подключений
void handle_connections(SOCKET server_socket) {
    while(1) {
        SOCKET client_socket = accept_client(server_socket);  // Принимаем клиента
        if (client_socket == INVALID_SOCKET) {
            continue;
        }

        // Создаем поток для обработки клиента
        HANDLE thread = CreateThread(NULL, 0, handle_client, &client_socket, 0, NULL);
        if (thread == NULL) {
            closesocket(client_socket);  // Закрываем сокет при ошибке создания потока
        } else {
            CloseHandle(thread);  // Закрываем дескриптор потока
        }
    }
}

// Функция очистки ресурсов сервера
void cleanup_server(SOCKET server_socket) {
    closesocket(server_socket);  // Закрытие сокета сервера
    WSACleanup();  // Очистка Winsock
}

// Функция запуска сервера
int run_server(int port) {
    SOCKET server_socket = initialize_server(port);  // Инициализируем сервер
    if (server_socket == INVALID_SOCKET) {
        printf("Failed to initialize server\n");
        return 1;
    }

    handle_connections(server_socket);  // Начинаем обработку подключений

    cleanup_server(server_socket);  // Очистка ресурсов
    return 0;
}

// Основная функция программы
int main() {
    return run_server(8888);  // Запускаем сервер на порту 8888
}

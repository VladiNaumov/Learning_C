/* многопоточного сервера-калькулятора с enum */

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")  // Подключаем библиотеку для работы с сокетами

// Определение операций с помощью enum
typedef enum {
    OP_PLUS,
    OP_MINUS,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_INVALID
} Operation;

// Функция для выполнения математической операции
double calculate(Operation op, double num1, double num2, int *error) {
    switch (op) {
        case OP_PLUS:
            return num1 + num2;
        case OP_MINUS:
            return num1 - num2;
        case OP_MULTIPLICATION:
            return num1 * num2;
        case OP_DIVISION:
            if (num2 != 0) {
                return num1 / num2;
            } else {
                *error = 1;  // Установка флага ошибки
                return 0;    // Возвращаем 0 для деления на ноль
            }
        default:
            *error = 1;  // Установка флага ошибки для некорректной операции
            return 0;    // Возвращаем 0 для некорректной операции
    }
}

// Функция для парсинга входных данных
int parse_input(const char *buffer, Operation *op, double *num1, double *num2) {
    char operation[20];

    // Используем sscanf_s для безопасного парсинга
    if (sscanf_s(buffer, "%s %lf %lf", operation, (unsigned)_countof(operation), num1, num2) != 3) {
        return 0;  // Некорректный ввод
    }

    // Определяем операцию
    if (strcmp(operation, "PLUS") == 0) {
        *op = OP_PLUS;
    } else if (strcmp(operation, "MINUS") == 0) {
        *op = OP_MINUS;
    } else if (strcmp(operation, "MULTIPLICATION") == 0) {
        *op = OP_MULTIPLICATION;
    } else if (strcmp(operation, "DIVISION") == 0) {
        *op = OP_DIVISION;
    } else {
        *op = OP_INVALID;
    }

    return 1;  // Успешный ввод
}

// Функция для обработки запросов от клиента
DWORD WINAPI handle_client(LPVOID client_socket) {
    SOCKET client_sock = *(SOCKET*)client_socket;
    char buffer[1024];
    int recv_size;

    // Получаем данные от клиента
    recv_size = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
    if (recv_size == SOCKET_ERROR) {
        printf("Recv failed\n");
        closesocket(client_sock);
        return 1;
    }

    buffer[recv_size] = '\0';  // Завершаем строку
    printf("Received: %s\n", buffer);

    Operation op;
    double num1, num2;
    int error = 0;  // Флаг ошибки

    // Парсим входные данные
    if (!parse_input(buffer, &op, &num1, &num2)) {
        printf("Invalid input from client: %s\n", buffer);
        const char *error_msg = "Invalid input\n";
        send(client_sock, error_msg, strlen(error_msg), 0);
        closesocket(client_sock);
        return 1;
    }

    // Вычисляем результат
    double result = calculate(op, num1, num2, &error);

    // Проверяем на ошибки при вычислении
    if (error) {
        const char *error_msg = "Error in calculation\n";
        send(client_sock, error_msg, strlen(error_msg), 0);
    } else {
        // Отправляем результат клиенту
        char result_buffer[50];
        sprintf(result_buffer, "%.2lf", result);
        send(client_sock, result_buffer, strlen(result_buffer), 0);
        printf("Result sent: %s\n", result_buffer);
    }

    // Закрываем подключение клиента
    closesocket(client_sock);
    return 0;
}

// Функция для инициализации сервера
SOCKET initialize_server(int port) {
    WSADATA wsa;
    SOCKET server_socket;
    struct sockaddr_in server;

    printf("Initializing Winsock...\n");
    // Инициализируем Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return INVALID_SOCKET;
    }

    // Создаем серверный сокет
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Задаем параметры сервера: IP-адрес и порт
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // Привязываем сокет к адресу
    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Начинаем прослушивание
    listen(server_socket, 10);  // Максимум 10 подключений в очереди

    return server_socket;
}

// Функция accept_connections обрабатывает ожидание подключения от клиента.
SOCKET accept_connections(SOCKET server_socket) {
    struct sockaddr_in client;
    int client_len = sizeof(struct sockaddr_in);
    SOCKET client_socket;

    // Ожидание подключения
    client_socket = accept(server_socket, (struct sockaddr*)&client, &client_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        return INVALID_SOCKET; // Возвращаем недействительный сокет при ошибке
    }

    printf("Connection accepted\n");

    return client_socket; // Возвращаем сокет клиента
}

// Функция для обработки завершения программы
void cleanup(SOCKET server_socket) {
    closesocket(server_socket);
    WSACleanup();
}

int main() {
    SOCKET server_socket = initialize_server(8888);
    if (server_socket == INVALID_SOCKET) {
        return 1;  // Ошибка инициализации сервера
    }

    printf("Waiting for incoming connections...\n");

    // Ожидание подключений от клиентов
    while (1) {
        SOCKET client_socket = accept_connections(server_socket);
        if (client_socket == INVALID_SOCKET) {
            continue; // Продолжаем цикл, если произошла ошибка
        }

        // Создаем новый поток для обработки каждого клиента
        HANDLE thread = CreateThread(NULL, 0, handle_client, &client_socket, 0, NULL);
        if (thread == NULL) {
            printf("Could not create thread\n");
            closesocket(client_socket);
        } else {
            CloseHandle(thread);  // Закрываем handle потока, так как он не нужен
        }
    }

    // Закрываем серверный сокет
    cleanup(server_socket);
    return 0;
}


/* TODO: Возможные улучшения:
Обработка ошибок:

Улучшить обработку ошибок, чтобы она была более информативной. Например, вместо того чтобы просто выводить сообщение об ошибке и завершать программу, можно обработать ошибку более гибко (например, игнорировать ее и продолжать работу сервера).
Улучшение ввода от клиента:

Добавить более строгую проверку входных данных от клиента. Сейчас мы просто парсим строку, но если клиент отправит некорректный ввод, это может привести к ошибкам. Можно добавить валидацию входных данных.
Управление потоками:

Рассмотреть возможность использования пула потоков. Создание нового потока для каждого клиента может быть ресурсоемким, особенно при большом количестве подключений. Пул потоков позволит переиспользовать уже созданные потоки и оптимизировать использование ресурсов.
Логирование:

Реализовать механизм логирования для отслеживания событий на сервере (подключения, ошибки, вычисления и т. д.). Это может помочь в отладке и мониторинге сервера.
Конфигурируемые параметры:

Сделать порт и другие параметры конфигурации настраиваемыми (например, через файл конфигурации или параметры командной строки).
Более сложные операции:

Рассмотреть возможность поддержки более сложных математических операций (например, степень, корень и т. д.) с использованием дополнительного перечисления или другой структуры данных.
Оптимизация работы с памятью:

Использовать free для освобождения выделенной памяти, если используешь динамическое выделение.
Безопасность:

Обратить внимание на безопасность данных, передаваемых между клиентом и сервером. Можно добавить шифрование для повышения уровня безопасности.

 */
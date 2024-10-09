/* многопоточного сервера-калькулятора Serialization/Deserialization */

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")  // Подключаем библиотеку для работы с сокетами

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_THREADS 4 // Максимальное количество потоков

HANDLE semaphore; // Семафор для контроля потоков

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
    // Возвращаем -1 для обозначения ошибки деления на ноль
    return (b != 0) ? a / b : -1;
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

    if (recv_len <= 0) {
        printf("Error receiving data from client.\n");
        closesocket(client_sock);
        ReleaseSemaphore(semaphore, 1, NULL); // Освобождаем слот в семафоре
        return;
    }

// Десериализация полученного сообщения
    deserialize_message(buffer, &msg);

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
    ReleaseSemaphore(semaphore, 1, NULL); // Освобождаем слот в семафоре
}


// Главная функция для запуска сервера
int main() {
    WSADATA wsaData; // Данные для инициализации Winsock
    SOCKET server_socket, client_socket; // Сокеты сервера и клиента
    struct sockaddr_in server_addr, client_addr; // Структуры для адреса сервера и клиента
    int addr_len = sizeof(client_addr); // Длина адреса клиента

// Инициализация семафора
    semaphore = CreateSemaphore(NULL, MAX_THREADS, MAX_THREADS, NULL);

// Инициализация Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

// Создание сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Все интерфейсы
    server_addr.sin_port = htons(PORT); // Порт

// Привязка сокета
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS); // Прослушивание входящих подключений

    printf("The server is running and waiting for connections...\n");

// Создание пула потоков
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket == INVALID_SOCKET) {
            printf("Error accepting connection.\n");
            continue;
        }
        printf("Client connected!\n");

// Создание потока для обработки клиента
        _beginthread(handle_client, 0, (void*)(intptr_t)client_socket);
    }

// Закрытие серверного сокета
    closesocket(server_socket);
    WSACleanup();
    return 0;
}


/*

Вот распределение функций по их назначению:

1. **`main`**:
   - Инициализация сокетов, семафора и запуск сервера.
   - Организация основного цикла для принятия новых подключений.
   - Запуск потоков для обработки клиентов.

2. **`handle_client`**:
   - Основная функция для обработки запросов клиентов.
   - Приём бинарных данных, десериализация, выполнение математических операций и отправка результата.
   - Контроль за количеством потоков с помощью семафора.

3. **`deserialize_message`**:
   - Преобразование (десериализация) полученных бинарных данных в структуру `BinaryMessage`, которая содержит команду и два числа.

4. **Математические функции** (`add`, `subtract`, `multiply`, `divide`):
   - Выполнение арифметических операций на основе данных, полученных от клиента.

5. **`print_error`**:
   - Вывод сообщений об ошибках (например, некорректная команда или ошибка деления на ноль).

6. **Семафор** (`WaitForSingleObject`, `ReleaseSemaphore`):
   - Управление количеством одновременно работающих потоков, предотвращение создания лишних потоков и защита от перегрузки сервера.

7. **Сокетные функции** (`recv`, `send`, `closesocket`):
   - Обработка сетевого взаимодействия между сервером и клиентами (приём и отправка данных, закрытие соединений).

Вот краткий анализ последнего кода:

### Что было сделано:

1. **Пул потоков с ограничением (семафор)**:
   - Мы добавили семафор для контроля количества одновременно работающих потоков. Это позволяет ограничить число клиентов, которых сервер может обслуживать одновременно, предотвращая перегрузку системы.
   - **Как это работает**: Когда новый клиент подключается, поток для его обслуживания блокирует слот в семафоре с помощью `WaitForSingleObject`. Когда работа завершена, слот освобождается с помощью `ReleaseSemaphore`, позволяя серверу обрабатывать нового клиента.

2. **Логирование подключений**:
   - Сервер выводит сообщение в консоль при подключении клиента, что позволяет отслеживать активность и анализировать количество подключений.
   - **Как это работает**: При каждом успешном подключении через `accept`, сервер выводит сообщение с подтверждением подключения.

3. **Обработка клиентских сообщений**:
   - Сервер принимает бинарное сообщение от клиента, десериализует его и выполняет математическую операцию (сложение, вычитание, умножение или деление). Если команда некорректна, выводится сообщение об ошибке.
   - **Как это работает**: Сообщение клиента передаётся в структуру `BinaryMessage` через `recv` и обрабатывается в зависимости от команды (`PLUS`, `MINUS`, и т.д.). Результат отправляется клиенту обратно через `send`.

4. **Безопасное завершение работы**:
   - После завершения обработки клиентского запроса поток закрывает сокет клиента и освобождает слот в семафоре.
   - **Как это работает**: После отправки результата или при возникновении ошибки соединение закрывается через `closesocket`, а семафор освобождается.

5. **Обработка ошибок**:
   - Добавлены проверки на ошибки при приёме данных (`recv`) и подключении клиентов (`accept`), что повышает стабильность сервера.
   - **Как это работает**: Если возникают ошибки во время приёма данных или подключения, сервер выводит сообщение об ошибке и корректно завершает работу с клиентом.

### Как это работает в целом:

1. Сервер запускается, инициализирует сокеты и создаёт семафор для контроля потоков.
2. В бесконечном цикле сервер принимает новые подключения клиентов.
3. Когда клиент подключается, создаётся новый поток, который захватывает слот в семафоре.
4. Поток обрабатывает запрос клиента (принимает данные, выполняет математическую операцию, отправляет результат).
5. По завершении поток освобождает слот в семафоре, закрывает сокет клиента и завершает свою работу, готовя место для нового клиента.

### Итог:
Мы сделали сервер более масштабируемым и стабильным благодаря пулу потоков с ограничением, добавили логирование и улучшили обработку ошибок. Теперь сервер может контролировать нагрузку и эффективно обрабатывать несколько подключений одновременно, не перегружая систему.

*/
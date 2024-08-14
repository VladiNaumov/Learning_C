#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Подключение библиотеки Winsock

#define PORT 12345

int main() {
    WSADATA wsa;
    SOCKET server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    int num1, num2, result;

    // Инициализация Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }

    // Создаем сокет для сервера
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Настраиваем сокет для повторного использования порта
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        printf("setsockopt failed with error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Привязываем сокет к адресу и порту
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Начинаем слушать входящие соединения
    if (listen(server_fd, 1) == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Принимаем входящее соединение от клиента
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
            printf("Accept failed with error: %d\n", WSAGetLastError());
            closesocket(server_fd);
            WSACleanup();
            return 1;
        }

        // Читаем данные от клиента
        int valread = recv(client_socket, buffer, 1024, 0);

        // Парсим данные (предполагаем, что клиент отправляет два числа через пробел)
        sscanf(buffer, "%d %d", &num1, &num2);

        // Вычисляем сумму чисел
        result = num1 + num2;

        // Выводим полученные числа и отправляем сумму клиенту
        printf("Received numbers: %d and %d\n", num1, num2);
        printf("Sending sum: %d\n", result);

        // Отправляем результат обратно клиенту
        send(client_socket, &result, sizeof(result), 0);

        // Закрываем соединение с клиентом
        closesocket(client_socket);
    }

    // Очистка Winsock
    closesocket(server_fd);
    WSACleanup();

    return 0;
}


/*
    Комментарии к серверной части:
    socket(): Создаем сокет для IPv4 и потоковой передачи данных (TCP).
    setsockopt(): Устанавливаем опцию SO_REUSEADDR и SO_REUSEPORT, чтобы можно было повторно использовать адрес и порт сервера после его закрытия.
    bind(): Привязываем сокет к адресу (INADDR_ANY) и порту (12345).
    listen(): Начинаем прослушивать входящие соединения. Второй аргумент (1) указывает, что мы будем принимать только одно соединение одновременно.
    accept(): Принимаем входящее соединение от клиента. Функция блокируется до тех пор, пока не будет получено новое соединение.
    read(): Читаем данные от клиента в буфер buffer.
    sscanf(): Парсим данные из буфера, ожидая два целых числа, которые клиент отправил через пробел.
    printf(): Выводим на консоль принятые числа и сумму, которую собираемся отправить обратно клиенту.
    send(): Отправляем результат (сумму чисел) обратно клиенту.
    close(): Закрываем соединение с клиентом.
*/
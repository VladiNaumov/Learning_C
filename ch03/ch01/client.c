#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Подключаем библиотеку для работы с сокетами

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server;
    char message[1024], server_reply[50];
    int recv_size;

    printf("Initializing Winsock...\n");
    // Инициализируем Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Создаем сокет для клиента
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Указываем IP-адрес и порт сервера (localhost и порт 8888)
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    // Подключаемся к серверу
    if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection failed\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server\n");

    // Вводим сообщение для отправки (операцию и два числа)
    printf("Enter operation and numbers (e.g., PLUS, MINUS, MULTIPLICATION, DIVISION 2 3): ");
    fgets(message, 1024, stdin);
    message[strcspn(message, "\n")] = '\0';  // Удаляем символ новой строки

    // Отправляем сообщение серверу
    if (send(client_socket, message, strlen(message), 0) < 0) {
        printf("Send failed\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Получаем ответ от сервера
    if ((recv_size = recv(client_socket, server_reply, 50, 0)) == SOCKET_ERROR) {
        printf("Receive failed\n");
    }

    // Завершаем строку ответа и выводим результат
    server_reply[recv_size] = '\0';
    printf("Server reply: %s\n", server_reply);

    // Закрываем сокет и освобождаем ресурсы
    closesocket(client_socket);
    WSACleanup();
    return 0;
}

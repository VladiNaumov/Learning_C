// Клиент отправляет изображение на сервер:

// Клиентская часть
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void send_file(const char *filename, SOCKET sock) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    char buffer[1024];
    size_t bytes_read;

    // Чтение файла и отправка его частями
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(sock, buffer, bytes_read, 0);
    }

    fclose(file);
    printf("Файл отправлен!\n");
}

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;

    // Инициализация Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Создаем сокет
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP-адрес сервера
    server.sin_family = AF_INET;
    server.sin_port = htons(8888); // Порт сервера

    // Подключаемся к серверу
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    // Отправляем изображение
    send_file("image.jpg", sock);

    closesocket(sock);
    WSACleanup();

    return 0;
}


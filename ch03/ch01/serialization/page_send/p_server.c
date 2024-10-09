// Серверная часть
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void receive_file(SOCKET client_sock, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    char buffer[1024];
    int bytes_received;

    // Чтение данных от клиента и запись в файл
    while ((bytes_received = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
    }

    fclose(file);
    printf("Файл получен и сохранен!\n");
}

int main() {
    WSADATA wsa;
    SOCKET server_sock, client_sock;
    struct sockaddr_in server, client;
    int client_len = sizeof(client);

    // Инициализация Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Создаем сокет
    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Привязываем сокет
    bind(server_sock, (struct sockaddr *)&server, sizeof(server));

    // Слушаем входящие соединения
    listen(server_sock, 3);

    printf("Ожидание подключений...\n");

    // Принимаем клиента
    client_sock = accept(server_sock, (struct sockaddr *)&client, &client_len);
    printf("Клиент подключен!\n");

    // Получаем и сохраняем файл
    receive_file(client_sock, "received_image.jpg");

    closesocket(client_sock);
    closesocket(server_sock);
    WSACleanup();

    return 0;
}

/*
 * Как это работает:
На клиенте:
Открывается файл изображения (image.jpg), читается по частям (например, блоками по 1024 байта), и каждая часть отправляется на сервер через сокет.
На сервере:
Сервер принимает эти части через сокет и записывает их в файл (received_image.jpg) до тех пор, пока все данные не будут получены.
Особенности:
Нет явной сериализации: Поскольку изображение уже представляет собой данные в бинарном формате, сериализация здесь не требуется. Мы просто отправляем и получаем "сырые" байты.
Обработка больших файлов: Используем буфер (1024 байта в примере), чтобы отправлять и принимать файл частями, что удобно для передачи больших файлов, таких как изображение 10 МБ.

 */
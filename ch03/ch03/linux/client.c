#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char server_reply[2000];
    int recv_size;

    // Создание сокета
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Настройка структуры sockaddr_in
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Подключение к удаленному серверу
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Получение ответа от сервера
    if ((recv_size = recv(client_socket, server_reply, 2000, 0)) < 0) {
        perror("Recv failed");
    }

    server_reply[recv_size] = '\0';
    printf("Server reply: %s\n", server_reply);

    // Закрытие сокета
    close(client_socket);

    return 0;
}

/*
Описание работы программ
Сервер:

Создает сокет для прослушивания входящих соединений.
Привязывает сокет к локальному адресу и порту.
Переводит сокет в режим прослушивания и ждет входящих соединений.
Принимает входящее соединение от клиента и отправляет ему сообщение "Hello from server!".
Закрывает сокеты и освобождает ресурсы.
Клиент:

Создает сокет и подключается к серверу по IP-адресу и порту.
Получает сообщение от сервера и выводит его на экран.
Закрывает сокет и освобождает ресурсы.

*/
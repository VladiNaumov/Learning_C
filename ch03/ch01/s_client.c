#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Подключаем библиотеку для работы с сокетами

#define PORT 8080
#define MAX_INPUT_SIZE 256

typedef struct {
    char command[16];
    float numbers[10];
    int count;
} BinaryMessage;

void serialize_message(BinaryMessage *msg, char *buffer) {
    memcpy(buffer, msg->command, sizeof(msg->command));
    memcpy(buffer + sizeof(msg->command), &msg->count, sizeof(int));
    memcpy(buffer + sizeof(msg->command) + sizeof(int), msg->numbers, msg->count * sizeof(float));
}

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Ввод данных от клиента в одной строке
    char input[MAX_INPUT_SIZE];
    printf("Enter the command and numbers in the format: PLUS, MINUS, MULTIPLICATION, DIVISION&2&3...\n");
    fgets(input, sizeof(input), stdin);

    // Разделение строки по символу '&'
    char *token = strtok(input, "&");
    if (token != NULL) {
        BinaryMessage msg;
        strncpy(msg.command, token, sizeof(msg.command));
        msg.command[sizeof(msg.command) - 1] = '\0';  // Обеспечение нуль-терминатора

        msg.count = 0;
        while ((token = strtok(NULL, "&")) != NULL) {
            if (msg.count < 10) {  // Ограничение на количество чисел
                msg.numbers[msg.count++] = atof(token);
            }
        }

        // Сериализация и отправка сообщения
        char buffer[sizeof(BinaryMessage)];
        serialize_message(&msg, buffer);
        send(sock, buffer, sizeof(buffer), 0);

        // Получение результата
        float result;
        recv(sock, (char*)&result, sizeof(result), 0);
        printf("Result: %f\n", result);
    } else {
        printf("Error: invalid input.\n");
    }

    closesocket(sock);
    WSACleanup();

    while (1){

    }
    return 0;
}

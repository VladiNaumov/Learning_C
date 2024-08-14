#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Подключение библиотеки Winsock

#define PORT 12345

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in serv_addr;
    int num1, num2;
    int result;
    char buffer[1024] = {0};

    // Инициализация Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }

    // Создаем сокет для клиента
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        return 1;
    }

    // Настраиваем адрес и порт сервера
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Подключаемся к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return 1;
    }

    // Вводим два числа с клавиатуры
    printf("Enter first number: ");
    scanf("%d", &num1);
    printf("Enter second number: ");
    scanf("%d", &num2);

    // Формируем строку для отправки серверу (два числа через пробел)
    sprintf(buffer, "%d %d", num1, num2);

    // Отправляем данные серверу
    send(sock, buffer, strlen(buffer), 0);
    printf("Numbers sent to server\n");

    // Получаем ответ от сервера
    recv(sock, (char *)&result, sizeof(result), 0);
    printf("Sum received from server: %d\n", result);

    // Закрываем сокет
    closesocket(sock);

    // Очистка Winsock
    WSACleanup();

    return 0;
}


/*
    Комментарии к клиентской части:
    socket(): Создаем сокет для IPv4 и потоковой передачи данных (TCP).
    connect(): Устанавливаем соединение с сервером, используя адрес 127.0.0.1 (localhost) и порт 12345.
    sprintf(): Формируем строку buffer, содержащую два целых числа, введенные пользователем через пробел.
    send(): Отправляем данные серверу.
    read(): Читаем ответ от сервера (сумму чисел).
    printf(): Выводим на консоль подтверждение отправки чисел и полученную сумму от сервера.
*/
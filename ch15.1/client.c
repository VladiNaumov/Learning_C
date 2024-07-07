#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    int num1, num2;
    int result;

    char buffer[1024] = {0};

    // Создаем сокет для клиента
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Настраиваем адрес и порт сервера
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Преобразуем IP-адрес в формат, который можно использовать для сетевых операций
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Подключаемся к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
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
    valread = read(sock, &result, sizeof(result));
    printf("Sum received from server: %d\n", result);

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
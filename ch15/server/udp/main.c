##include <stdio.h>
 #include <string.h>
 #include <errno.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <pthread.h>

 #include <sys/socket.h>
 #include <netinet/in.h>

 #include <datagram_server_core.h>

 // Функция для выделения памяти под структуру sockaddr_in
 struct sockaddr* sockaddr_new() {
   return malloc(sizeof(struct sockaddr_in));
 }

 // Функция для возврата размера структуры sockaddr_in
 socklen_t sockaddr_sizeof() {
   return sizeof(struct sockaddr_in);
 }

 // Основная функция программы
 int main(int argc, char** argv) {

   // ----------- 1. Create socket object ------------------
   // Создание сокета с использованием IPv4 и протокола UDP (SOCK_DGRAM)
   int server_sd = socket(AF_INET, SOCK_DGRAM, 0);
   if (server_sd == -1) {
     fprintf(stderr, "Could not create socket: %s\n", strerror(errno));
     exit(1);
   }

   // ----------- 2. Bind the socket file ------------------

   // Подготовка адреса сервера
   struct sockaddr_in addr;
   memset(&addr, 0, sizeof(addr)); // Заполнение структуры нулями
   addr.sin_family = AF_INET; // Установка семейства адресов IPv4
   addr.sin_addr.s_addr = INADDR_ANY; // Принимать сообщения с любых сетевых интерфейсов
   addr.sin_port = htons(9999); // Установка порта (в сетевом порядке байт)

   // Привязка адреса к сокету
   int result = bind(server_sd, (struct sockaddr*)&addr, sizeof(addr));
   if (result == -1) {
     close(server_sd);
     fprintf(stderr, "Could not bind the address: %s\n", strerror(errno));
     exit(1);
   }

   // ----------- 3. Start serving requests ---------
   // Начало обработки запросов
   serve_forever(server_sd);

   return 0;
 }

/*
    Описание функций и основных шагов программы:
    struct sockaddr* sockaddr_new()
    Эта функция выделяет память под структуру sockaddr_in, используемую для хранения сетевого адреса.

    socklen_t sockaddr_sizeof()
    Эта функция возвращает размер структуры sockaddr_in.

    int main(int argc, char** argv)
    Это основная функция программы:

    Создание сокета:

    Создается сокет с использованием IPv4 и протокола UDP (SOCK_DGRAM).
    Привязка адреса к сокету:

    Задается адрес семейства IPv4 (AF_INET), который принимает сообщения с любых сетевых интерфейсов (INADDR_ANY) и порт 9999.
    Начало обработки запросов:

    Вызывается функция serve_forever, которая, как предполагается, обрабатывает входящие сообщения в бесконечном цикле.
    Этот код реализует базовый UDP сервер, который принимает сообщения на порт 9999 и использует функцию serve_forever для обработки входящих пакетов.

*/

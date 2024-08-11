#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <stream_server_core.h>

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
  // Создание сокета
  int server_sd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sd == -1) {
    fprintf(stderr, "Could not create socket: %s\n", strerror(errno));
    exit(1);
  }

  // ----------- 2. Bind the socket file ------------------

  // Подготовка адреса
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr)); // Заполнение структуры нулями
  addr.sin_family = AF_INET; // Установка семейства адресов IPv4
  addr.sin_addr.s_addr = INADDR_ANY; // Принимать соединения с любых сетевых интерфейсов
  addr.sin_port = htons(6666); // Установка порта (в сетевом порядке байт)

  // Привязка адреса к сокету
  int result = bind(server_sd, (struct sockaddr*)&addr, sizeof(addr));
  if (result == -1) {
    close(server_sd);
    fprintf(stderr, "Could not bind the address: %s\n", strerror(errno));
    exit(1);
  }

  // ----------- 3. Prepare backlog ------------------
  // Установка очереди ожидающих соединений (backlog)
  result = listen(server_sd, 10);
  if (result == -1) {
    close(server_sd);
    fprintf(stderr, "Could not set the backlog: %s\n", strerror(errno));
    exit(1);
  }

  // ----------- 4. Start accepting clients ---------
  // Начало приема клиентов
  accept_forever(server_sd);

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

    Создается сокет с использованием IPv4 и протокола TCP (SOCK_STREAM).
    Привязка адреса к сокету:

    Задается адрес семейства IPv4 (AF_INET), который принимает подключения с любых сетевых интерфейсов (INADDR_ANY) и порт 6666.
    Установка очереди ожидающих соединений (backlog):

    Устанавливается максимальная длина очереди ожидающих соединений на серверном сокете до того,
    как клиенты будут приняты или отклонены.
    Начало приема клиентов:

    Вызывается функция accept_forever, которая обрабатывает подключения клиентов в бесконечном цикле.
    Этот код реализует базовый сервер TCP, который принимает подключения на порт 6666 и использует функцию accept_forever,
    которая, как предполагается, обрабатывает подключения в потоке.

*/
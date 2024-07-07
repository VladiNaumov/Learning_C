#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <datagram_server_core.h>

// Функция для выделения памяти под структуру sockaddr_un
struct sockaddr* sockaddr_new() {
  return malloc(sizeof(struct sockaddr_un));
}

// Функция для возврата размера структуры sockaddr_un
socklen_t sockaddr_sizeof() {
  return sizeof(struct sockaddr_un);
}

// Основная функция программы
int main(int argc, char** argv) {
  char sock_file[] = "/tmp/calc_svc.sock"; // Имя файла сокета

  // ----------- 1. Create socket object ------------------
  // Создание сокета с использованием UNIX domain и протокола UDP (SOCK_DGRAM)
  int server_sd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (server_sd == -1) {
    fprintf(stderr, "Could not create socket: %s\n", strerror(errno));
    exit(1);
  }

  // ----------- 2. Bind the socket file ------------------

  // Удаление ранее созданного файла сокета, если он существует.
  unlink(sock_file);

  // Подготовка адреса
  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX; // Установка семейства UNIX domain
  strncpy(addr.sun_path, sock_file, sizeof(addr.sun_path) - 1); // Копирование имени файла сокета

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
Эта функция выделяет память под структуру sockaddr_un, используемую для хранения адреса UNIX domain.

socklen_t sockaddr_sizeof()
Эта функция возвращает размер структуры sockaddr_un.

int main(int argc, char** argv)
Это основная функция программы:

Создание сокета:

Создается сокет с использованием UNIX domain и протокола UDP (SOCK_DGRAM).
Привязка адреса к сокету:

Удаляется ранее созданный файл сокета (если он существует) с помощью unlink.
Задается адрес типа sockaddr_un, который представляет собой семейство UNIX domain и путь к файлу /tmp/calc_svc.sock.
Начало обработки запросов:

Вызывается функция serve_forever, которая, как предполагается, обрабатывает входящие пакеты в бесконечном цикле.
Этот код реализует базовый UDP сервер с использованием UNIX domain сокета, который принимает сообщения на указанный файл сокета /tmp/calc_svc.sock и использует функцию serve_forever для обработки входящих пакетов.

*/

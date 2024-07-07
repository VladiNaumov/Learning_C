#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <stream_server_core.h>

int main(int argc, char** argv) {
  char sock_file[] = "/tmp/calc_svc.sock"; // Путь к файлу сокета

  // ----------- 1. Create socket object ------------------
  // Создание сокета с использованием UNIX domain и протокола TCP (SOCK_STREAM)
  int server_sd = socket(AF_UNIX, SOCK_STREAM, 0);
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

  // ----------- 3. Prepare backlog ------------------
  // Установка размера очереди ожидающих соединений (backlog)
  result = listen(server_sd, 10);
  if (result == -1) {
    close(server_sd);
    fprintf(stderr, "Could not set the backlog: %s\n", strerror(errno));
    exit(1);
  }

  // ----------- 4. Start accepting clients ---------
  // Начало принятия клиентских соединений в бесконечном цикле
  accept_forever(server_sd);

  return 0;
}

/*
Описание функций и основных шагов программы:
int main(int argc, char** argv)
Это основная функция программы:

Создание сокета:

Создается сокет с использованием UNIX domain и протокола TCP (SOCK_STREAM).
Привязка адреса к сокету:

Удаляется ранее созданный файл сокета (если он существует) с помощью unlink.
Задается адрес типа sockaddr_un, который представляет собой семейство UNIX domain и путь к файлу /tmp/calc_svc.sock.
Подготовка backlog:

Устанавливается размер очереди ожидающих соединений (backlog) на 10. Это означает, что сервер может одновременно обрабатывать до 10 клиентских соединений.
Начало принятия клиентских соединений:

Вызывается функция accept_forever, которая ожидает входящие соединения от клиентов в бесконечном цикле.
Этот код реализует простой TCP сервер с использованием UNIX domain сокета, который принимает соединения на указанный файл сокета /tmp/calc_svc.sock и обрабатывает их с помощью функции accept_forever
*/
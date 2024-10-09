#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <calc_proto_ser.h>
#include <calc_service.h>

#include "common_server_core.h"
#include "datagram_server_core.h"

// Структура для хранения адреса клиента
struct client_addr_t {
  int server_sd;             // Дескриптор серверного сокета
  struct sockaddr* sockaddr; // Адрес клиента
  socklen_t socklen;         // Длина адреса клиента
};

// Функция для отправки ответа клиенту через датаграмму
void datagram_write_resp(struct client_context_t* context,
        struct calc_proto_resp_t* resp) {
  // Сериализация ответа в буфер
  struct buffer_t buf = calc_proto_ser_server_serialize(context->ser, resp);
  if (buf.len == 0) {
    close(context->addr->server_sd);
    fprintf(stderr, "Internal error while serializing object.\n");
    exit(1);
  }
  // Отправка сериализованных данных клиенту
  int ret = sendto(context->addr->server_sd, buf.data, buf.len,
      0, context->addr->sockaddr, context->addr->socklen);
  free(buf.data);
  if (ret == -1) {
    fprintf(stderr, "Could not write to client: %s\n", strerror(errno));
    close(context->addr->server_sd);
    exit(1);
  } else if (ret < buf.len) {
    fprintf(stderr, "WARN: Less bytes were written!\n");
    close(context->addr->server_sd);
    exit(1);
  }
}

// Функция для запуска сервера и обработки запросов клиентов
void serve_forever(int server_sd) {
  char buffer[64];
  while (1) {
    struct sockaddr* sockaddr = sockaddr_new(); // Создание нового адреса клиента
    socklen_t socklen = sockaddr_sizeof();      // Получение размера адреса клиента
    // Чтение данных из датаграммного сокета
    int read_nr_bytes = recvfrom(server_sd, buffer, sizeof(buffer), 0, sockaddr, &socklen);
    if (read_nr_bytes == -1) {
      close(server_sd);
      fprintf(stderr, "Could not read from datagram socket: %s\n", strerror(errno));
      exit(1);
    }
    struct client_context_t context;

    // Инициализация контекста клиента
    context.addr = (struct client_addr_t*)malloc(sizeof(struct client_addr_t));
    context.addr->server_sd = server_sd;
    context.addr->sockaddr = sockaddr;
    context.addr->socklen = socklen;

    context.ser = calc_proto_ser_new(); // Создание нового сериализатора
    calc_proto_ser_ctor(context.ser, &context, 256);
    calc_proto_ser_set_req_callback(context.ser, request_callback); // Установка обработчика запросов
    calc_proto_ser_set_error_callback(context.ser, error_callback); // Установка обработчика ошибок

    context.svc = calc_service_new(); // Создание нового сервиса калькулятора
    calc_service_ctor(context.svc);

    context.write_resp = &datagram_write_resp; // Установка функции отправки ответа

    // Десериализация запроса
    bool_t req_found = FALSE;
    struct buffer_t buf;
    buf.data = buffer;
    buf.len = read_nr_bytes;
    calc_proto_ser_server_deserialize(context.ser, buf, &req_found);

    // Обработка недействительного запроса
    if (!req_found) {
      struct calc_proto_resp_t resp;
      resp.req_id = -1;
      resp.status = ERROR_INVALID_RESPONSE;
      resp.result = 0.0;
      context.write_resp(&context, &resp);
    }

    // Деструкторы и освобождение памяти
    calc_service_dtor(context.svc);
    calc_service_delete(context.svc);

    calc_proto_ser_dtor(context.ser);
    calc_proto_ser_delete(context.ser);

    free(context.addr->sockaddr);
    free(context.addr);
  }
}

/*

    Описание функций и структур:
    struct client_addr_t
    Эта структура хранит информацию о клиенте:

    int server_sd: Дескриптор серверного сокета.
    struct sockaddr* sockaddr: Адрес клиента.
    socklen_t socklen: Длина адреса клиента.
    void datagram_write_resp(struct client_context_t* context, struct calc_proto_resp_t* resp)
    Эта функция отправляет ответ клиенту:

    Сериализует ответ в буфер.
    Отправляет сериализованные данные клиенту через датаграммный сокет.
    Обрабатывает ошибки при отправке данных.
    void serve_forever(int server_sd)
    Эта функция запускает сервер и обрабатывает запросы клиентов:

    Создает новый адрес клиента и получает его размер.
    Читает данные из датаграммного сокета.
    Инициализирует контекст клиента, создавая и настраивая необходимые структуры.
    Десериализует запрос и вызывает соответствующие обработчики.
    Обрабатывает недействительные запросы, отправляя ответ с ошибкой.
    Освобождает память и уничтожает созданные структуры после обработки запроса.

*/
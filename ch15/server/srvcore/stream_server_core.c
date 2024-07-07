#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/socket.h>

#include <calc_proto_ser.h>
#include <calc_service.h>

#include "common_server_core.h"
#include "stream_server_core.h"

// Структура для хранения адреса клиента
struct client_addr_t {
  int sd; // Дескриптор сокета
};

// Функция для отправки ответа клиенту через поток
void stream_write_resp(
        struct client_context_t* context,
        struct calc_proto_resp_t* resp) {
  // Сериализация ответа в буфер
  struct buffer_t buf = calc_proto_ser_server_serialize(context->ser, resp);
  if (buf.len == 0) {
    close(context->addr->sd);
    fprintf(stderr, "Internal error while serializing response\n");
    exit(1);
  }
  // Отправка сериализованных данных клиенту через поток
  int ret = write(context->addr->sd, buf.data, buf.len);
  free(buf.data);
  if (ret == -1) {
    fprintf(stderr, "Could not write to client: %s\n", strerror(errno));
    close(context->addr->sd);
    exit(1);
  } else if (ret < buf.len) {
    fprintf(stderr, "WARN: Less bytes were written!\n");
    exit(1);
  }
}

// Функция для обработки клиента в отдельном потоке
void* client_handler(void *arg) {
  struct client_context_t context;

  // Инициализация контекста клиента
  context.addr = (struct client_addr_t*)malloc(sizeof(struct client_addr_t));
  context.addr->sd = *((int*)arg);
  free((int*)arg);

  context.ser = calc_proto_ser_new(); // Создание нового сериализатора
  calc_proto_ser_ctor(context.ser, &context, 256);
  calc_proto_ser_set_req_callback(context.ser, request_callback); // Установка обработчика запросов
  calc_proto_ser_set_error_callback(context.ser, error_callback); // Установка обработчика ошибок

  context.svc = calc_service_new(); // Создание нового сервиса калькулятора
  calc_service_ctor(context.svc);

  context.write_resp = &stream_write_resp; // Установка функции отправки ответа

  int ret;
  char buffer[128];
  while (1) {
    // Чтение данных из сокета клиента
    int ret = read(context.addr->sd, buffer, 128);
    if (ret == 0 || ret == -1) {
      break; // Выход из цикла при ошибке или закрытии соединения
    }
    struct buffer_t buf;
    buf.data = buffer; buf.len = ret;
    // Десериализация запроса и вызов соответствующих обработчиков
    calc_proto_ser_server_deserialize(context.ser, buf, NULL);
  }

  // Деструкторы и освобождение памяти
  calc_service_dtor(context.svc);
  calc_service_delete(context.svc);

  calc_proto_ser_dtor(context.ser);
  calc_proto_ser_delete(context.ser);

  free(context.addr);

  return NULL;
}

// Функция для принятия подключений клиентов
void accept_forever(int server_sd) {
  while (1) {
    // Принятие нового подключения клиента
    int client_sd = accept(server_sd, NULL, NULL);
    if (client_sd == -1) {
      close(server_sd);
      fprintf(stderr, "Could not accept the client: %s\n", strerror(errno));
      exit(1);
    }
    pthread_t client_handler_thread;
    int* arg = (int *)malloc(sizeof(int));
    *arg = client_sd;
    // Создание нового потока для обработки клиента
    int result = pthread_create(&client_handler_thread, NULL, &client_handler, arg);
    if (result) {
      close(client_sd);
      close(server_sd);
      free(arg);
      fprintf(stderr, "Could not start the client handler thread.\n");
      exit(1);
    }
  }
}


/*
    Описание функций и структур:
    struct client_addr_t
    Эта структура хранит информацию о клиенте:

    int sd: Дескриптор сокета клиента.
    void stream_write_resp(struct client_context_t* context, struct calc_proto_resp_t* resp)
    Эта функция отправляет ответ клиенту через поток:

    Сериализует ответ в буфер.
    Отправляет сериализованные данные клиенту через сокет.
    Обрабатывает ошибки при отправке данных.
    void* client_handler(void *arg)
    Эта функция обрабатывает запросы клиента в отдельном потоке:

    Инициализирует контекст клиента, создавая и настраивая необходимые структуры.
    Читает данные из сокета клиента.
    Десериализует запрос и вызывает соответствующие обработчики.
    Обрабатывает ошибки и закрытие соединения.
    Освобождает память и уничтожает созданные структуры после обработки запросов.
    void accept_forever(int server_sd)
    Эта функция принимает подключения клиентов в бесконечном цикле:

    Принимает новое подключение клиента.
    Создает новый поток для обработки каждого клиента.
    Обрабатывает ошибки при принятии подключения и создании потока.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#include <sys/socket.h>

#include "common_client_core.h"

/* Этот код реализует клиент для взаимодействия с сервером по протоколу TCP. Клиент отправляет запросы, получает ответы и обрабатывает их в отдельном потоке.
Вся работа с соединением осуществляется через потоки ввода-вывода сокета. Вот подробный разбор кода. */

/* Поток чтения ответов от сервера */

/* Эта функция выполняется в отдельном потоке и непрерывно читает данные из сокета.
Она использует read для получения данных и десериализует их с помощью calc_proto_ser_client_deserialize.
Если возникает ошибка при чтении, функция выводит сообщение об ошибке и завершает работу. */
void* stream_response_reader(void* obj) {
  struct context_t* context = (struct context_t*)obj;
  char buf[64];
  while (1) {
    int ret = read(context->sd, buf, sizeof(buf));
    if (ret < 0) {
      fprintf(stderr, "Read error! %s\n", strerror(errno));
      break;
    }
    if (ret == 0) {
      break;
    }
    struct buffer_t b; b.data = buf, b.len = ret;
    calc_proto_ser_client_deserialize(context->ser, b, NULL);
  }
  return NULL;
}

/* Основной цикл клиента */

/* Инициализация контекста и сериализатора */
void stream_client_loop(int conn_sd) {
  struct context_t context;

  context.sd = conn_sd;
  context.ser = calc_proto_ser_new();
  calc_proto_ser_ctor(context.ser, &context, 128);
  calc_proto_ser_set_resp_callback(context.ser, on_response);
  calc_proto_ser_set_error_callback(context.ser, on_error);

/* Создание потока для чтения ответов */
/* Создается отдельный поток для чтения ответов от сервера, который выполняет функцию stream_response_reader.*/
  pthread_t reader_thread;
  pthread_create(&reader_thread, NULL,
          stream_response_reader, &context);

/* Основной цикл обработки ввода пользователя */

/* Читает ввод пользователя.
   Разбирает ввод и создает структуру запроса.
   Сериализует запрос и отправляет его на сервер с помощью write.
   Обрабатывает ошибки записи и выводит сообщения о статусе отправки запроса.
*/

  char buf[128];
  printf("? (type quit to exit) ");
  while (1) {
    scanf("%s", buf);
    int brk = 0, cnt = 0;
    struct calc_proto_req_t req;
    parse_client_input(buf, &req, &brk, &cnt);
    if (brk) {
      break;
    }
    if (cnt) {
      continue;
    }
    struct buffer_t ser_req =
        calc_proto_ser_client_serialize(context.ser, &req);
    int ret = write(context.sd, ser_req.data, ser_req.len);
    if (ret == -1) {
      fprintf(stderr, "Error while writing! %s\n",
              strerror(errno));
      break;
    }
    if (ret < ser_req.len) {
      fprintf(stderr, "Wrote less than anticipated!\n");
      break;
    }
    printf("The req(%d) is sent.\n", req.id);
  }

  /* Завершение работы */
  shutdown(conn_sd, SHUT_RD);
  calc_proto_ser_dtor(context.ser);
  calc_proto_ser_delete(context.ser);
  pthread_join(reader_thread, NULL);
  printf("Bye.\n");
}

/* Этот код создает клиент для TCP-сервера, который обрабатывает арифметические операции.
Клиент отправляет запросы, получает и обрабатывает ответы в отдельном потоке, обеспечивая асинхронную обработку данных. */
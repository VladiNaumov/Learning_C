#ifndef COMMON_SERVER_CORE_H
#define COMMON_SERVER_CORE_H

#include <sys/socket.h>

struct client_addr_t;
struct client_context_t;

// Тип для указателя на функцию, которая пишет ответ
typedef void (*write_resp_func_t)(struct client_context_t*, struct calc_proto_resp_t*);


// Структура, представляющая контекст клиента
struct client_context_t {
  struct client_addr_t* addr;       // Информация об адресе клиента
  struct calc_proto_ser_t* ser;     // Сериализатор протокола для клиента
  struct calc_service_t* svc;       // Сервис калькулятора для обработки запросов
  write_resp_func_t write_resp;     // Указатель на функцию для записи ответа
};


typedef void (*write_resp_func_t)(struct client_context_t*, struct calc_proto_resp_t*);

// Прототипы функций
void error_callback(void* obj, int ref_id, int error_code);
void request_callback(void* obj, struct calc_proto_req_t req);

extern struct sockaddr* sockaddr_new();
extern socklen_t sockaddr_sizeof();

#endif

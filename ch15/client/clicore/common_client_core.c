#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#include "common_client_core.h"

/*Функции преобразования и обработки ошибок*/

/*Эта функция пытается преобразовать строку в число с плавающей запятой (double). Она использует sscanf для чтения строки в переменную типа double.
Если преобразование успешно, функция возвращает 1; в противном случае возвращает 0.

*/
int _to_double(const char* str, double* num) {
  return sscanf(str, "%lf", num) == 1 ? 1 : 0;
}

/* Эта функция преобразует коды ошибок в читаемые строки. Коды ошибок, вероятно, определены в common_client_core.h. */
const char* _error_to_str(int error_code) {
  switch (error_code) {
    case ERROR_INVALID_RESPONSE: return "INVALID_RESPONSE";
    case ERROR_INVALID_RESPONSE_REQ_ID: return "INVALID_REQUEST_ID";
    case ERROR_INVALID_RESPONSE_STATUS: return "INVALID_STATUS";
    case ERROR_INVALID_RESPONSE_RESULT: return "INVALID_RESPONSE";
    default: return "(UNKOWN)";
  }
}

/* Эта функция преобразует статусные коды в читаемые строки. Статусные коды, вероятно, определены в common_client_core.h. */
const char* _status_to_str(status_t status) {
  switch (status) {
    case STATUS_OK: return "OK";
    case STATUS_INVALID_REQUEST: return "INVALID_REQUEST";
    case STATUS_INVALID_METHOD: return "INVALID_METHOD";
    case STATUS_INVALID_OPERAND: return "INVALID_OPERAND";
    case STATUS_DIV_BY_ZERO: return "DIV_BY_ZERO";
    case STATUS_INTERNAL_ERROR: return "INTERNAL_ERROR";
    default: return "(UNKNOWN)";
  }
}
/* Обработчики ошибок и ответов */

/* Эта функция обрабатывает ошибки сериализации. Она выводит сообщение об ошибке в стандартный поток ошибок (stderr).
Если req_id больше 0, сообщение включает идентификатор запроса. */
void on_error(void* obj, int req_id, int error_code) {
  if (req_id > 0) {
    fprintf(stderr, "Serializer error on req(%d): %s\n", req_id, _error_to_str(error_code));
  } else {
    fprintf(stderr, "Serializer error: %s\n", _error_to_str(error_code));
  }
  printf("? (type quit to exit) "); fflush(stdout);
}

/* Эта функция обрабатывает ответы сервера.
Она выводит идентификатор запроса, статус и результат операции в стандартный поток вывода (stdout).
*/
void on_response(void* obj, struct calc_proto_resp_t resp) {
  printf("req(%d) > status: %s, result: %lf\n", resp.req_id, _status_to_str(resp.status), resp.result);
  printf("? (type quit to exit) "); fflush(stdout);
}

/* Разбор ввода пользователя
Эта функция разбирает ввод пользователя и заполняет структуру запроса для протокола калькулятора.
Если пользователь вводит quit, функция устанавливает флаг brk, чтобы завершить работу.
Функция поддерживает команды для различных операций калькулятора и проверяет корректность введенных операндов.

*/

void parse_client_input(char* buf, struct calc_proto_req_t* req, int *brk, int*cnt) {
  static int req_id = 0;

  double op1 = 0.0, op2 = 0.0;
  method_t method = NONE;
  char* sign, *op1_str, *op2_str;
  if (!strcmp(buf, "quit")) {
    *brk = 1;
    return;
  } else if (!strcmp(buf, "mem")) {
    method = GETMEM;
  } else if (!strcmp(buf, "reset")) {
    method = RESMEM;
  } else if ((sign = strstr(buf, "++")) > 0) {
    *sign = '\0';
    op1_str = buf;
    op2_str = sign + 2;
    method = ADDM;
  } else if ((sign = strstr(buf, "+")) > 0) {
    *sign = '\0';
    op1_str = buf;
    op2_str = sign + 1;
    method = ADD;
  } else if ((sign = strstr(buf, "--")) > 0) {
    *sign = '\0';
    op1_str = buf;
    op2_str = sign + 2;
    method = SUBM;
  } else if ((sign = strstr(buf, "-")) > 0) {
    *sign = '\0';
    op1_str = buf;
    op2_str = sign + 1;
    method = SUB;
  } else if ((sign = strstr(buf, "**")) > 0) {
    *sign = '\0';
    op1_str = buf;
    op2_str = sign + 2;
    method = MULM;
  } else if ((sign = strstr(buf, "*")) > 0) {
    *sign = '\0';
    op1_str = buf;
    op2_str = sign + 1;
    method = MUL;
  } else if ((sign = strstr(buf, "/")) > 0) {
    *sign = '\0';
    op1_str = buf;
    op2_str = sign + 1;
    method = DIV;
  } else {
    fprintf(stderr, "Invalid input!\n");
    printf("? (type quit to exit) "); fflush(stdout);
    *cnt = 1;
    return;
  }
  if (method == NONE) {
    fprintf(stderr, "No method could be deduced!\n");
    printf("? (type quit to exit) "); fflush(stdout);
    *cnt = 1;
    return;
  }
  if (method != GETMEM && method != RESMEM) {
    if (!_to_double(op1_str, &op1) || !_to_double(op2_str, &op2)) {
      fprintf(stderr, "Invalid operands!\n");
      printf("? (type quit to exit) "); fflush(stdout);
      *cnt = 1;
      return;
    }
  }
  req->id = req_id++;
  req->method = method;
  req->operand1 = op1;
  req->operand2 = op2;
}

/* Этот код создает основу для клиента протокола калькулятора, который может взаимодействовать с сервером,
выполняющим арифметические операции и возвращающим результаты или ошибки. */
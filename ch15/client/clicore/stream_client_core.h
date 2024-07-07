#ifndef STREAM_CLIENT_CORE_H
#define STREAM_CLIENT_CORE_H

/* Этот файл объявляет одну функцию stream_client_loop(int conn_sd);,
которая используется для запуска клиентского цикла, работающего с потоками (TCP). */
void stream_client_loop(int conn_sd);

#endif

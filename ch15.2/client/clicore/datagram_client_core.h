#ifndef DATAGRAM_CLIENT_CORE_H
#define DATAGRAM_CLIENT_CORE_H

/* Этот файл объявляет только одну функцию datagram_client_loop(int conn_sd);,
которая используется для запуска клиентского цикла, работающего с датаграммами (UDP). */
void datagram_client_loop(int conn_sd);

#endif

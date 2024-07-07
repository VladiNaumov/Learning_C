#ifndef STREAM_SERVER_CORE_H
#define STREAM_SERVER_CORE_H


/* Это объявление функции accept_forever, которая принимает один параметр server_sd.

   server_sd: Параметр, вероятно, представляет собой дескриптор сокета сервера, который будет использоваться в функции для принятия входящих соединений.  */
void accept_forever(int server_sd);

#endif

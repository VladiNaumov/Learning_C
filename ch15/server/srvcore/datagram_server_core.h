#ifndef DATAGRAM_SERVER_CORE_H
#define DATAGRAM_SERVER_CORE_H

/* Эта строка объявляет функцию serve_forever, которая принимает параметр server_sd.

   server_sd: Этот параметр, вероятно, представляет собой дескриптор сокета сервера, используемый для связи в сетевом программировании. */

void serve_forever(int server_sd);

#endif

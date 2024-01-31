
## Lerning in C

- ch01 Bit operations
- ch02 Pointers
- ch03 Array + pointer
- ch04 Struct + pointer


### Compiling

- gcc app.c
- gcc app.c -o app.exe


- gcc -std=c99 -Wall -pedantic app.c -o app.exe
- gcc -std=c11 -Wall -pedantic app.c -o app.exe
- gcc -std=c17 -Wall -pedantic app.c -o app.exe
- gcc -std=c23 -Wall -pedantic app.c -o app.exe


- Опция -S позволяет создать компьютерный файл содержащий ассемблерный кодом: gcc app.c -S
- Опция -c позволяет создать компьютерный файл содержащий объектный код, то есть машинный код app.o: gcc -c app.c

- gcc --version
- gcc --help
- gcc target-help


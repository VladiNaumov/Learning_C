
## Lerning in C

- ch01 Bit operations
- ch02 Pointers
- ch03 Array + pointer
- ch04 Struct + pointer
- ch05 Structure as a "class"
- ch06 Structure and "private attribute", в заголовочном файлe List.h объявляем структуры и функции, но не реализовываем их! (т.e как interface).
- ch07 Отношение "композиция"
- ch08 Отношение "aгрегация"
- ch09 Наследование	"восходящее приведение"
- ch10 Первый подход к наследованию (одиночного наследования) 
- ch11 Второй подход к наследованию (множественного наследования)
- ch12 Полиморфизм


### Compiling

- gcc app.c
- gcc app.c -o app.exe
- gcc -std=c99 -Wall -pedantic app.c -o app.exe
- gcc -std=c11 -Wall -pedantic app.c -o app.exe
- gcc -std=c17 -Wall -pedantic app.c -o app.exe
- gcc -std=c23 -Wall -pedantic app.c -o app.exe
- gcc -E app.c (компиляции в единицу трансляции)
- gcc -S app.c (компиляция в асcамблер код)
- gcc -c app.c (компиляция в машинный код "объектный файл")
- gcc --version
- gcc --help
- gcc target-help


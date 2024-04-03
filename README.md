
## Lerning in C

- ch01 Bit operations
- ch02 Pointers
- ch03 Array + pointer, dynamic array
- ch04 Struct + pointer
- ch05 Structure as a "class"
- ch06 Structure and "private attribute", заголовочный файл List.h как interface.
- ch07 Отношение "композиция"
- ch08 Отношение "aгрегация"
- ch09 Наследование	"восходящее приведение"
- ch10 Первый подход к наследованию (одиночного наследования) 
- ch11 Второй подход к наследованию (множественного наследования)
- ch12 Полиморфизм
- ch13 Singly linked list
- ch14 Doubly linked list
- ch15 Stack
- ch16 Stack (Dynamic, stack array, stack linked list)
- ch17 Queue
- ch18 Binary search tree


### Compiling

- gcc app.c
- gcc app.c -o app.exe
- gcc -std=c99 -Wall -pedantic app.c -o app.exe
- gcc -std=c11 -Wall -pedantic app.c -o app.exe
- gcc -std=c17 -Wall -pedantic app.c -o app.exe
- gcc -std=c23 -Wall -pedantic app.c -o app.exe
- gcc -E app.c (Компиляция в единицу трансляции)
- gcc -S -masm=intel app.c (Компиляция с созданием файла ассемблерного кода синтаксис -masm)
- as -alhnd app.s > output.lst (Эта команда преобразует файл ассемблерного кода app.s в листинг программы output.lst)
- gcc -c app.c (компиляция в машинный код "объектный файл")
- gcc --version
- gcc --help
- gcc target-help

### Рекомендация по правильности оформления программного кода на Cи, он предусматривает разбиение программы на три файла: 
- Interface - заголовочные файлы, для определения структуры данных и объявление функции используемое для управления этой структурой.
- Реализация функций объявленных в interface.
- Клиентская программа, которая использует функции, объявление в интерфейсе, для работы на более высоком уровне. 

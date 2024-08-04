
## Lerning in C

- ch01 Bit operations
- ch02 Pointers
- ch03 Array + pointer, dynamic array
- ch04 Struct + pointer
- ch05 Structure as a "class"
- ch06 создание массива объектов Person
- ch07 Structure and "private attribute", заголовочный файл List.h как interface.
- 
- ch08 Several structures into one array (создание обобщений с помощью Enum, Union)
- ch09 Отношение "композиция"
- ch10 Отношение "aгрегация"
- ch11 Наследование	"восходящее приведение"
- ch12 Первый подход к наследованию (одиночного наследования) 
- ch13 Второй подход к наследованию (множественного наследования)
- ch14 Полиморфизм
- 
- ch15.1 Socket-programming "Calculator"
- ch15.2 Socket-programming (Project "Calculator + unit test")

- ch16 Singly linked list
- ch17 Doubly linked list
- ch18 Stack
- ch19 Stack (Dynamic, stack array, stack linked list)
- ch20 Queue
- ch21 String vai number
- ch22 Named Pipes, Shared Memory

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


## Lerning in C

- ch01 Bit operations
- ch02 Pointers
- ch03 Array + pointer
- ch04 Struct + pointer
- ch05 Structure as a "class"
- ch06 Structure and "private attribute", в заголовочном файлe List.h как interface.
- ch07 Отношение "композиция"
- ch08 Отношение "aгрегация"
- ch09 Наследование	"восходящее приведение"
- ch10 Первый подход к наследованию (одиночного наследования) 
- ch11 Второй подход к наследованию (множественного наследования)
- ch12 Полиморфизм
- ch13 Односвязный список
- ch14 Стек


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

### Рекомендация по правильности оформления программного кода на Cи, он предусматривает разбиене программы на три файла: 
- Interface - заголовочные файлы, для определения структуры данных и объявление функции используемое для управления этой структурой.
- Реализация функций объявленных в interface.
- Клиентская программа, которая использует функции, объявление в интерфейсе, для работы на более высоком уровне. 
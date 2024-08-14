
## Lerning in C

- ch01
  - ch01 Bit operations
  - ch02 Pointers
  - ch03 Array + pointer, dynamic array
  - ch04 Struct + pointer
  - ch05 Structure as a "class"
  - ch06 создание массива объектов Person
  - ch07 Structure and "private attribute", заголовочный файл List.h как interface.
-
- ch02 
  - ch01 Several structures into one array (создание обобщений с помощью Enum, Union)
  - ch02 Отношение "композиция"
  - ch03 Отношение "aгрегация"
  - ch04 Наследование	"восходящее приведение"
  - ch05 Первый подход к наследованию (одиночного наследования) 
  - ch06 Второй подход к наследованию (множественного наследования)
  - ch07 Полиморфизм
-
- ch03
  - ch01 Socket-programming "Calculator" 
  - ch02 Socket-programming (Project "Calculator + unit test")
-
- ch04 
  - ch01 Linked list
  - ch02 Stack (Dynamic, stack array, stack linked list)
  - ch03 Queue
- 
- ch21 String vai number
- ch22 Named Pipes, Shared Memory
- ch23 Menu
- ch24 Macros (Макрос — это фрагмент кода, которому дано имя)
- ch25 Design patterns
- ch26 question/answer
- ch27 finite state machine

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

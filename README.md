
## Lerning in C

- ch01
  - ch01 Bit operations
  - ch02 Pointers
  - ch03 Array + pointer, dynamic array
  - ch04 Struct + pointer
  - ch05 Structure as a "class"
  - ch06 создание массива объектов Person
  - ch07 Structure and "private attribute", заголовочный файл List.h как interface.
  - ch08 функции возвращающая значение структуры
  - ch09 function pointer (пример указателя на функцию) 
  - ch10 Функция, которая принимает другую функцию в качестве аргументов
  - ch11 Difference between struct and union
  - ch12 Типы квалификаторов в языке C
-
- ch02 
  - ch01 Several structures into one array (создание обобщений с помощью Enum, Union)
  - ch02 Отношение "композиция"
  - ch03 Отношение "aгрегация"
  - ch04 Наследование	"восходящее приведение"
  - ch05 Первый подход к наследованию (одиночного наследования) 
  - ch06 Второй подход к наследованию (множественного наследования)
  - ch07 Полиморфизм основные подходы в C: 
    - Указатели на функции.
    - Структуры и функции.
    - Композиция.
    - Аналогично виртуальным таблицам (vtable)  из C++. . 
  - ch08 Callback

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
- ch05
  - ch01 String vai number
  - ch02 Named Pipes, Shared Memory
  - ch03 Menu
  - ch04 Macros (Макрос - это фрагмент кода, которому дано имя)
  - ch05 Design patterns
  - ch06 question/answer
  - ch07 finite state machines (Конечный автомат - Математическая абстракция)


### Основные рекомендации по именованию файлов

1. **Расширение файлов:**
- Для исходных файлов на языке C используется расширение `.c`.
- Для заголовочных файлов используется расширение `.h`.
- Файлы, содержащие C++ код, используют расширение `.cpp` или `.cc`.

Примеры:
- `main.c` — исходный файл программы.
- `utils.h` — заголовочный файл с определениями функций.

2. **Имя файла должно отражать его содержимое:**
- Имя файла должно быть информативным и кратким, чтобы по нему можно было понять, какие функции или данные он содержит.
- Например, если файл содержит функции для работы с математическими операциями, его можно назвать `math_utils.c`.

3. **Использование нижнего регистра и символов подчеркивания:**
- В имени файла рекомендуется использовать только символы нижнего регистра и символ подчеркивания (`_`) для разделения слов.
- Например: `file_io.c`, `data_processing.h`.

4. **Уникальность имени:**
- Убедитесь, что имена файлов в одном проекте уникальны, чтобы избежать конфликтов при сборке проекта.

5. **Избегайте использования специальных символов и пробелов:**
- Не рекомендуется использовать символы, отличные от букв, цифр и символа подчеркивания. Это связано с кросс-платформенной совместимостью.
- Пробелы также не следует использовать в именах файлов. Вместо этого лучше использовать символ подчеркивания.

6. **Кросс-платформенная совместимость:**
- Следует учитывать ограничения файловых систем разных операционных систем. Например, Windows не позволяет использовать такие символы, как `\ / : * ? " < > |` в именах файлов.

### Примеры имен файлов:
- **Правильно:** `file_io.c`, `data_parser.h`, `config_reader.c`.
- **Неправильно:** `File IO.c`, `dataParser.h`, `config-reader.c`.

Следование этим рекомендациям позволит сделать ваш код более понятным, удобным в сопровождении и кросс-платформенно совместимым.
### Compiling

- gcc app.c
- gcc app.c -o app.exe
- gcc -c app.c (компиляция в машинный код "объектный файл")
- gcc -E  app.c -o app.txt (Компиляция в единицу трансляции)
- gcc -S -masm=intel app.c (Компиляция с созданием файла ассемблерного кода синтаксис -masm)
- as -alhnd app.s > output.lst (Эта команда преобразует файл ассемблерного кода app.s в листинг программы output.lst)
-
- gcc -std=c99 -Wall -pedantic app.c -o app.exe
- gcc -std=c11 -Wall -pedantic app.c -o app.exe
- gcc -std=c17 -Wall -pedantic app.c -o app.exe
- gcc -std=c23 -Wall -pedantic app.c -o app.exe
- 
- gcc --version
- gcc --help
- gcc target-help

### Рекомендация по правильности оформления программного кода на Cи, он предусматривает разбиение программы на три файла: 
- Interface `app.h` - заголовочные файлы, для определения структуры данных и объявление функции используемое для управления этой структурой.
- Реализация `app.c` функций объявленных в interface.
- Клиентская программа `int main(void)`, которая использует функции, объявление в интерфейсе, для работы на более высоком уровне. 

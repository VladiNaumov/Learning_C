/*
1. Объявление только указателя на структуру
В заголовочном файле (.h) объявляйте тип структуры как typedef для указателя на структуру.
Это скрывает детали внутренней реализации структуры, оставляя только интерфейс для работы с объектами этого типа.

2. Определение структуры в исходном файле
В исходном файле (.c) определяйте структуру и функции, которые с ней работают.
Это позволяет вам скрыть детали реализации от пользователя заголовочного файла.

3. Использование в основном файле
В основном файле (.c) или любом другом месте, где вам нужно использовать структуру, вы включаете заголовочный файл и используете предоставленные функции.
*/


#include <stdio.h>
#include "mymodule.h"

int main() {
    MyStruct* ms = create_mystruct(10);
    if (ms != NULL) {
        printf("Value: %d\n", get_value(ms));
        set_value(ms, 20);
        printf("Updated Value: %d\n", get_value(ms));
        destroy_mystruct(ms);
    }
    return 0;
}


/*
Пояснение
typedef struct MyStruct MyStruct;: Это создаёт тип MyStruct, который является указателем на структуру.
Это скрывает реализацию структуры от пользователя.

Функции: Объявленные в заголовочном файле функции являются интерфейсом для работы с объектами типа MyStruct.
Пользователь может создавать, модифицировать и уничтожать объекты структуры, но не видит её внутреннюю реализацию.

Определение структуры в .c: В исходном файле вы определяете структуру и функции, которые с ней работают.
Это позволяет вам изменять реализацию, не затрагивая код, который использует ваш модуль.

Этот подход обеспечивает гибкость и удобство в разработке и поддержке программного обеспечения, улучшает модульность и уменьшает связность между различными частями программы.

*/
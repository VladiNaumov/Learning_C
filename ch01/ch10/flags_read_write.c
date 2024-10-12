/* Пример битовой маски:
Предположим, у нас есть флаги для управления доступом к файлам: чтение, запись и выполнение. Каждый флаг занимает свою битовую позицию:
*/

#include <stdio.h>

// Определяем флаги для разрешений
#define FLAG_READ   0x01  // 0001 (1)
#define FLAG_WRITE  0x02  // 0010 (2)
#define FLAG_EXEC   0x04  // 0100 (4)
#define FLAG_DELETE 0x08  // 1000 (8)

void check_permissions(int flags) {
    if (flags & FLAG_READ) {
        printf("Permission to READ\n");
    }
    if (flags & FLAG_WRITE) {
        printf("Permission to WRITE\n");
    }
    if (flags & FLAG_EXEC) {
        printf("Permission to EXECUTE\n");
    }
    if (flags & FLAG_DELETE) {
        printf("Permission to DELETE\n");
    }
}

int main() {
    int permissions = FLAG_READ | FLAG_WRITE;  // Разрешаем чтение и запись
    check_permissions(permissions);  // Проверим разрешения

    return 0;
}


/*

Флаги FLAG_READ, FLAG_WRITE, FLAG_EXEC и FLAG_DELETE имеют значения 1, 2, 4 и 8, что позволяет их комбинировать в одной переменной.
Мы можем комбинировать флаги с помощью побитового ИЛИ (|). Например, FLAG_READ | FLAG_WRITE означает, что включены оба флага — и чтение, и запись.
Функция check_permissions() проверяет каждый флаг с помощью побитового И (&). Например, выражение flags & FLAG_READ проверяет, установлен ли бит, соответствующий чтению.

*/
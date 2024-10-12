/*
Допустим, вы хотите управлять не только состоянием лампы, но и её яркостью (например, низкая или высокая яркость)
*/

#include <stdio.h>

// Определяем флаги
#define FLAG_ON  1   // Лампа включена
#define FLAG_OFF 0   // Лампа выключена
#define FLAG_BRIGHT 2   // Лампа в ярком режиме

void control_lamp(int flag) {
    if (flag & FLAG_ON) {
        printf("Lamp is ON\n");

        if (flag & FLAG_BRIGHT) {
            printf("Lamp is in BRIGHT mode\n");
        } else {
            printf("Lamp is in DIM mode\n");
        }
    } else {
        printf("Lamp is OFF\n");
    }
}

int main() {
    int lamp_state;

    // Включаем лампу в ярком режиме
    lamp_state = FLAG_ON | FLAG_BRIGHT;
    control_lamp(lamp_state);

    // Включаем лампу в обычном режиме
    lamp_state = FLAG_ON;
    control_lamp(lamp_state);

    // Выключаем лампу
    lamp_state = FLAG_OFF;
    control_lamp(lamp_state);

    return 0;
}

/*
Объяснение:
Мы добавили ещё один флаг: FLAG_BRIGHT, который равен 2. Этот флаг указывает, что лампа должна работать в ярком режиме.
Теперь мы можем комбинировать флаги с помощью оператора побитового ИЛИ (|). Например, FLAG_ON | FLAG_BRIGHT означает, что лампа включена и работает в ярком режиме.
Внутри функции control_lamp() используется побитовое И (&) для проверки, какие флаги установлены.
*/
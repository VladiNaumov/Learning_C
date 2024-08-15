#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <locale.h>

// Функция для очистки экрана консоли
void clear_screen() {
    system("cls"); // Очистка экрана консоли
}

// Функция для отображения меню с выделением текущего выбранного пункта
void display_menu(const char *menu[], int menuSize, int selected) {
    clear_screen();
    printf("Use the arrow keys to navigate. Press Enter to select.\n");

    for (int i = 0; i < menuSize; i++) {
        if (i == selected) {
            printf("-> %s\n", menu[i]); // Выделение выбранного элемента
        } else {
            printf("   %s\n", menu[i]); // Отображение остальных элементов
        }
    }
}

// Функция для обработки навигации по меню и возврата индекса выбранного пункта
int navigate_menu(const char *menu[], int menuSize) {
    int selected = 0;
    while (1) {
        display_menu(menu, menuSize, selected);
        int ch = _getch(); // Получение нажатой клавиши

        if (ch == 224) { // Специальные клавиши (стрелки)
            ch = _getch(); // Получение кода стрелки
            switch (ch) {
                case 72: // Стрелка вверх
                    selected = (selected - 1 + menuSize) % menuSize; // Переход к предыдущему элементу
                    break;
                case 80: // Стрелка вниз
                    selected = (selected + 1) % menuSize; // Переход к следующему элементу
                    break;
            }
        } else if (ch == 13) { // Клавиша Enter
            break; // Завершение цикла и возврат выбранного индекса
        }
    }
    return selected;
}

// Функция для выполнения арифметической операции и отображения результата
void perform_operation(int choice, int num1, int num2) {
    clear_screen();
    printf("You selected: %s\n", (const char*[]){"Add", "Subtract", "Multiply", "Divide"}[choice]);

    switch (choice) {
        case 0: { // Сложение
            int result = num1 + num2;
            printf("Result: %d + %d = %d\n", num1, num2, result);
            break;
        }
        case 1: { // Вычитание
            int result = num1 - num2;
            printf("Result: %d - %d = %d\n", num1, num2, result);
            break;
        }
        case 2: { // Умножение
            int result = num1 * num2;
            printf("Result: %d * %d = %d\n", num1, num2, result);
            break;
        }
        case 3: { // Деление
            if (num2 != 0) {
                float result = (float)num1 / num2;
                printf("Result: %d / %d = %.2f\n", num1, num2, result);
            } else {
                printf("Error: Division by zero!\n"); // Обработка ошибки деления на ноль
            }
            break;
        }
        default:
            printf("Error: Invalid operation selected!\n"); // Обработка неверного выбора
            break;
    }
}

// Основная функция для запуска программы меню
void run_menu_program() {
    const char *menu[] = { 
        "Add", 
        "Subtract", 
        "Multiply", 
        "Divide"
    };
    int menuSize = sizeof(menu) / sizeof(menu[0]);
    int num1 = 2, num2 = 2; // Числа для операций

    int choice = navigate_menu(menu, menuSize); // Получение выбора пользователя
    perform_operation(choice, num1, num2); // Выполнение выбранной операции

    printf("Press any key to exit...\n");
    _getch(); // Ожидание нажатия клавиши перед выходом
}

// Точка входа в программу
int main() {
    setlocale(LC_ALL, ""); // Установка локали для поддержки международных символов
    run_menu_program(); // Запуск программы меню
    return 0;
}

/*

Объяснение функций на русском:
Функция clear_screen:

Очищает экран консоли для обновления интерфейса.
Функция display_menu:

Отображает меню и выделяет текущий выбранный пункт, что помогает пользователю понять, какой пункт выбран.
Функция navigate_menu:

Обрабатывает ввод пользователя для навигации по меню с использованием стрелок вверх и вниз, возвращает индекс выбранного пункта.
Функция perform_operation:

Выполняет арифметическую операцию (сложение, вычитание, умножение или деление) в зависимости от выбора пользователя и отображает результат. Обрабатывает деление на ноль.
Функция run_menu_program:

Инициализирует меню, обрабатывает выбор пользователя, выполняет выбранную операцию и ожидает нажатия клавиши перед завершением.
Функция main:

Устанавливает локаль для поддержки международных символов и запускает основную функцию программы.


*/
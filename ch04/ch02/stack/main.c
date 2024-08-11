#include <stdio.h>
#include "stack.h"

// Выводит инструкции по использованию программы
void instructions(void) {
    puts("Enter choice:\n"
         "1 to push a value onto the stack\n"
         "2 to pop a value off the stack\n"
         "3 to end the program");
}

// Обрабатывает выбор пользователя
void handleChoice(unsigned int choice, StackNodePtr *stackPtr) {
    int value;

    switch (choice) {
        case 1:
            printf("Enter an integer: ");
            if (scanf("%d", &value) != 1) {
                fprintf(stderr, "Error: Invalid input. Please enter a valid integer.\n");
                while (getchar() != '\n'); // Очистка ввода
                break;
            }
            push(stackPtr, value);
            printStack(*stackPtr);
            break;

        case 2:
            if (!isEmpty(*stackPtr)) {
                printf("The popped value is %d.\n", pop(stackPtr));
            } else {
                puts("The stack is empty. Nothing to pop.");
            }
            printStack(*stackPtr);
            break;

        case 3:
            puts("End of program.");
            break;

        default:
            puts("Invalid choice. Please enter 1, 2, or 3.");
            instructions();
            break;
    }
}

// Основное меню программы
void menu(void) {
    StackNodePtr stackPtr = NULL;
    unsigned int choice;

    instructions();

    while (1) {
        printf("? ");
        if (scanf("%u", &choice) != 1) {
            fprintf(stderr, "Error: Invalid input. Please enter a valid choice.\n");
            while (getchar() != '\n'); // Очистка ввода
            continue;
        }

        if (choice == 3) {
            handleChoice(choice, &stackPtr);
            break;
        }

        handleChoice(choice, &stackPtr);
    }
}

// Точка входа в программу
int main(void) {
    menu();
    return 0;
}

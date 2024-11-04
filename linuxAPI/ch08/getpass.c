/* Этот код демонстрирует, как реализовать функцию для безопасного ввода пароля без отображения символов на экране. */

#include "../../lib/error_functions.h"
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

char *my_getpass(const char *prompt)
{
    // Отключаем флаг ECHO для подавления отображения символов
    struct termios buf;
    if (0 != tcgetattr(0, &buf)) {
        errExit("tcgetattr");
    }
    buf.c_lflag &= ~ECHO;

    if (0 != tcsetattr(0, TCSANOW, &buf)) {
        errExit("tcsetattr");
    }

    // Игнорируем сигнал SIGINT
    signal(SIGINT, SIG_IGN);

    printf("%s", prompt);
    fflush(stdout);

    static char pw[BUFSIZ];
    if (fgets(pw, BUFSIZ, stdin) == NULL) {
        // Если возникла ошибка чтения
        printf("\nError reading password.\n");
        pw[0] = '\0';
    } else {
        unsigned long len = strlen(pw);
        if (len > 0 && pw[len - 1] == '\n') {
            pw[len - 1] = '\0';
        }
    }

    // Восстанавливаем исходные настройки
    if (0 != tcgetattr(0, &buf)) {
        errExit("tcgetattr");
    }
    buf.c_lflag |= ECHO;

    if (0 != tcsetattr(0, TCSANOW, &buf)) {
        errExit("tcsetattr");
    }
    signal(SIGINT, SIG_DFL);

    return pw;
}

/*
Этот код демонстрирует, как реализовать функцию для безопасного ввода пароля без отображения символов на экране. Основные шаги:

1. **Отключение флага ECHO**: Этот флаг в структуре `termios` управляет отображением вводимых символов. Отключив его, мы предотвращаем отображение символов пароля на экране.
2. **Игнорирование сигнала `SIGINT`**: Временное игнорирование сигнала `SIGINT` (Ctrl+C) для предотвращения прерывания ввода пароля.
3. **Считывание пароля**: Используется `fgets` для чтения пароля с ограничением по размеру буфера.
4. **Восстановление флага ECHO и сигнала `SIGINT`**: После завершения ввода пароля настройки терминала и поведения при прерывании восстанавливаются.

*/
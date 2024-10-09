#include <stdio.h>
#include <regex.h>

int main() {
    regex_t regex;
    int reti;
    const char *pattern = "[0-9]+";
    const char *test_str = "abc123def456";

    // Компиляция регулярного выражения
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Не удалось скомпилировать регулярное выражение\n");
        return 1;
    }

    // Выполнение поиска
    reti = regexec(&regex, test_str, 0, NULL, 0);
    if (!reti) {
        printf("Найдено совпадение: %s\n", test_str);
    } else if (reti == REG_NOMATCH) {
        printf("Совпадений не найдено.\n");
    } else {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Регулярное выражение соответствует ошибке: %s\n", msgbuf);
        return 1;
    }

    // Освобождение памяти
    regfree(&regex);

    return 0;
}

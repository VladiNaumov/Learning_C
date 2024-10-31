/* Этот код выполняет проверку подлинности пользователя путем запроса имени пользователя и пароля,
 * сравнивая зашифрованный ввод пароля с хранимым в системе значением.
 * Программа запрашивает имя пользователя, получает информацию о нём и сравнивает введённый пользователем пароль
 * с зашифрованным паролем в `/etc/shadow`. */

/*
 * компиляция: $ gcc check_password.c getpass.c ../../lib/error_functions.c -lcrypt -o check_password
 */

#include "../../lib/tlpi_hdr.h"
#include "getpass.h"
#include <pwd.h>
#include <shadow.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    // Получаем максимальную длину имени пользователя
    long lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    if (lnmax == -1) {
        lnmax = 256;  // Устанавливаем значение по умолчанию
    }

    char username[lnmax];
    printf("Username: ");
    fflush(stdout);
    if (fgets(username, lnmax, stdin) == NULL) {
        exit(EXIT_FAILURE);  // Ошибка при чтении имени пользователя
    }

    // Удаляем завершающий символ '\n'
    unsigned long len = strlen(username);
    username[len - 1] = '\0';

    // Получаем запись пароля пользователя
    struct passwd *pwd = getpwnam(username);
    if (pwd == NULL) {
        fatal("couldn't get password record");  // Ошибка при получении записи
    }

    // Получаем запись пароля из файла shadow
    struct spwd *spwd = getspnam(username);
    if (spwd == NULL && errno == EACCES) {
        fatal("no permission to read shadow file");  // Ошибка доступа к файлу shadow
    }

    // Если в файле shadow есть запись, используем её для пароля
    if (spwd != NULL) {
        pwd->pw_passwd = spwd->sp_pwdp;
    }

    // Запрашиваем ввод пароля
    char *password = my_getpass("Password: ");
    char *encrypted = crypt(password, pwd->pw_passwd);  // Шифруем ввод и сравниваем с сохранённым

    // Затираем пароль из памяти для безопасности
    for (char *p = password; *p != '\0';) {
        *p++ = '\0';
    }

    if (encrypted == NULL) {
        errExit("crypt");  // Ошибка при шифровании
    }

    // Сравниваем зашифрованный пароль с паролем пользователя
    if (strcmp(encrypted, pwd->pw_passwd) != 0) {
        printf("Incorrect password\n");  // Неверный пароль
        exit(EXIT_FAILURE);
    }

    printf("Successfully authenticated: UID=%ld\n", (long)pwd->pw_uid);  // Успешная аутентификация
    exit(EXIT_SUCCESS);
}

/*
### Описание программы

1. **Запрос имени пользователя**: программа запрашивает имя пользователя и удаляет символ новой строки (`\n`).
2. **Поиск информации о пользователе**: получает информацию о пользователе с помощью `getpwnam` и, если доступен, `getspnam` для доступа к зашифрованному паролю в файле `/etc/shadow`.
3. **Ввод пароля**: с использованием функции `my_getpass`, запрашивается пароль, который не отображается на экране.
4. **Шифрование и проверка**: пароль шифруется функцией `crypt`, используя тот же метод, что и для пароля пользователя. Результат сравнивается с хранимым значением.
5. **Результат аутентификации**: если пароли совпадают, выводится сообщение об успешной аутентификации и UID пользователя. В противном случае — сообщение об ошибке.
*/
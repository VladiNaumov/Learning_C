/* 8.Users and Groups */

#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <unistd.h>
#include <string.h>

int main() {
    /* 1. Пример использования getpwnam() и getpwuid() -  */
    struct passwd *pw = getpwnam("root");   // Поиск информации по имени пользователя
    if (pw) {
        printf("User %s has UID: %d\n", pw->pw_name, pw->pw_uid);
    }

    pw = getpwuid(0);                       // Поиск по UID
    if (pw) {
        printf("UID 0 belongs to user: %s\n", pw->pw_name);
    }

    /* 2. Пример использования getgrnam() и getgrgid() */
    struct group *gr = getgrnam("wheel");   // Поиск информации по имени группы
    if (gr) {
        printf("Group %s has GID: %d\n", gr->gr_name, gr->gr_gid);
    }

    gr = getgrgid(0);                       // Поиск по GID
    if (gr) {
        printf("GID 0 belongs to group: %s\n", gr->gr_name);
    }

    /* 3. Пример использования setpwent(), getpwent() и endpwent() */
    setpwent();                             // Открываем файл паролей
    while ((pw = getpwent()) != NULL) {     // Считываем запись за записью
        printf("User: %s (UID: %d)\n", pw->pw_name, pw->pw_uid);
    }
    endpwent();                             // Закрываем файл паролей

    /* 4. Пример использования getspnam(), getspent(), setspent() и endspent() */
    struct spwd *spw;
    setspent();                             // Открываем файл теневых паролей
    while ((spw = getspent()) != NULL) {    // Считываем запись за записью
        printf("Shadow entry for user: %s\n", spw->sp_namp);
    }
    endspent();                             // Закрываем файл теневых паролей

    spw = getspnam("root");                 // Поиск записи по имени
    if (spw) {
        printf("Shadow password for root: %s\n", spw->sp_pwdp);
    }

    /* 5. Пример использования crypt() для проверки пароля */
    char *encrypted_password = crypt("password", spw->sp_pwdp);  // Шифрование для сравнения
    if (encrypted_password && strcmp(encrypted_password, spw->sp_pwdp) == 0) {
        printf("Password is correct.\n");
    } else {
        printf("Password is incorrect.\n");
    }

    /* 6. Пример использования getpass() для чтения пароля пользователя */
    char *user_password = getpass("Enter password: ");  // Скрытый ввод пароля
    if (user_password) {
        printf("You entered: %s\n", user_password);     // Пароль доступен для сравнения
    }

    return 0;
}


/*

getpwnam(), getpwuid(), getgrnam(), getgrgid() - Эти функции предназначены для получения информации о пользователях и группах:

getpwnam(), getpwuid() позволяют получить информацию о пользователе по его имени или UID.
getgrnam(), getgrgid() позволяют получить информацию о группе по её имени или GID.

Применимость: Эти функции полезны для получения данных, но не для создания или изменения пользователей/групп. 
Например, с их помощью можно найти UID или GID пользователя, чтобы затем использовать их в других системных вызовах.

*/
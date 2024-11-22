/* check_password_caps.c

   Программа демонстрирует использование привилегий (capabilities) для создания
   приложения, которое выполняет задачи, требующие повышенных прав, без полного
   доступа root. Она считывает имя пользователя и пароль, а затем проверяет
   их достоверность, сравнивая с данными из (shadow) файла паролей.

   Выполняемый файл программы должен быть установлен с привилегией CAP_DAC_READ_SEARCH:
   
        $ sudo setcap "cap_dac_read_search=p" check_password_caps

   Программа специфична для Linux.

   Смотрите также: check_password.c.
*/

#define _BSD_SOURCE             /* Для объявления getpass() из <unistd.h> */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE           /* Для объявления crypt() из <unistd.h> */
#endif
#include <sys/capability.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <crypt.h>
#include <shadow.h>
#include "tlpi_hdr.h"

/**
 * @brief Изменяет настройки указанной привилегии.
 *
 * @param capability Привилегия, которую нужно изменить.
 * @param setting Новое значение: CAP_SET для включения или CAP_CLEAR для отключения.
 * @return 0 при успешном завершении, -1 при ошибке.
 */
static int modifyCap(cap_value_t capability, int setting) {
    cap_t caps;
    cap_value_t capList[1];

    // Получение текущих привилегий процесса
    caps = cap_get_proc();
    if (caps == NULL)
        return -1;

    // Изменение значения указанной привилегии
    capList[0] = capability;
    if (cap_set_flag(caps, CAP_EFFECTIVE, 1, capList, setting) == -1) {
        cap_free(caps);
        return -1;
    }

    // Применение изменений к процессу
    if (cap_set_proc(caps) == -1) {
        cap_free(caps);
        return -1;
    }

    // Освобождение памяти
    if (cap_free(caps) == -1)
        return -1;

    return 0;
}

/**
 * @brief Устанавливает указанную привилегию в эффективный набор.
 *
 * @param capability Привилегия, которую нужно установить.
 * @return 0 при успешном завершении, -1 при ошибке.
 */
static int raiseCap(cap_value_t capability) {
    return modifyCap(capability, CAP_SET);
}

/**
 * @brief Убирает все привилегии из всех наборов процесса.
 *
 * @return 0 при успешном завершении, -1 при ошибке.
 */
static int dropAllCaps(void) {
    cap_t empty;
    int s;

    empty = cap_init();
    if (empty == NULL)
        return -1;

    s = cap_set_proc(empty);
    if (cap_free(empty) == -1)
        return -1;

    return s;
}

int main(int argc, char *argv[]) {
    char *username, *password, *encrypted, *p;
    struct passwd *pwd;
    struct spwd *spwd;
    Boolean authOk;
    size_t len;
    long lnmax;

    // Получение размера буфера для имени пользователя
    lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    if (lnmax == -1)  // Если предел неизвестен
        lnmax = 256;  // Предполагаем значение

    username = malloc(lnmax);
    if (username == NULL)
        errExit("malloc");

    // Запрос имени пользователя
    printf("Username: ");
    fflush(stdout);
    if (fgets(username, lnmax, stdin) == NULL)
        exit(EXIT_FAILURE);  // Выход по EOF

    len = strlen(username);
    if (username[len - 1] == '\n')
        username[len - 1] = '\0';  // Удаление завершающего '\n'

    // Получение записи пароля для имени пользователя
    pwd = getpwnam(username);
    if (pwd == NULL)
        fatal("couldn't get password record");

    // Временное поднятие привилегии CAP_DAC_READ_SEARCH
    if (raiseCap(CAP_DAC_READ_SEARCH) == -1)
        fatal("raiseCap() failed");

    // Получение записи shadow-пароля
    spwd = getspnam(username);
    if (spwd == NULL && errno == EACCES)
        fatal("no permission to read shadow password file");

    // Сброс всех привилегий
    if (dropAllCaps() == -1)
        fatal("dropAllCaps() failed");

    if (spwd != NULL)  // Если есть запись в shadow
        pwd->pw_passwd = spwd->sp_pwdp;  // Используем shadow-пароль

    // Запрос пароля
    password = getpass("Password: ");

    // Шифрование пароля и удаление его открытой версии
    encrypted = crypt(password, pwd->pw_passwd);
    for (p = password; *p != '\0'; )
        *p++ = '\0';  // Удаление пароля из памяти

    if (encrypted == NULL)
        errExit("crypt");

    // Сравнение зашифрованного пароля
    authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
    if (!authOk) {
        printf("Incorrect password\n");
        exit(EXIT_FAILURE);
    }

    // Успешная аутентификация
    printf("Successfully authenticated: UID=%ld\n", (long) pwd->pw_uid);

    // Здесь можно выполнять действия от имени аутентифицированного пользователя

    exit(EXIT_SUCCESS);
}

/*
### Резюме:
- **Назначение**: Программа использует Linux capabilities для временного доступа к привилегированным ресурсам.
- **Ключевые моменты**:
  - **Capabilities**:
    - Поднимаются для чтения shadow-файла (`CAP_DAC_READ_SEARCH`).
    - Сбрасываются сразу после выполнения задачи.
  - **Функции**:
    - `raiseCap` поднимает привилегии.
    - `dropAllCaps` убирает все привилегии.
  - Проверяется введенный пользователем пароль с записью в shadow-файле.
- **Применение**: Этот подход минимизирует риски, связанные с выполнением задач с повышенными правами.
*/
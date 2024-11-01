
/*
 * Реализовать `getpwnam()` с использованием функций `setpwent()`, `getpwent()` и `endpwent()` можно следующим образом.
 * Функция `getpwnam()` ищет пользователя в файле `/etc/passwd` по имени пользователя и возвращает структуру `passwd`,
 * содержащую информацию о нем. Мы можем использовать `setpwent()` для инициализации или сброса файла,
 * `getpwent()` для последовательного чтения записей, и `endpwent()` для закрытия файла после поиска.

*/

#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <stdlib.h>

struct passwd *my_getpwnam(const char *name) {
    struct passwd *entry;

    // Инициализируем чтение файла паролей
    setpwent();

    // Читаем записи, пока не найдем совпадение
    while ((entry = getpwent()) != NULL) {
        if (strcmp(entry->pw_name, name) == 0) {
            // Копируем структуру passwd, чтобы избежать перезаписи
            struct passwd *result = malloc(sizeof(struct passwd));
            if (result != NULL) {
                *result = *entry;
            }
            // Закрываем доступ к файлу
            endpwent();
            return result;
        }
    }

    // Закрываем доступ к файлу, если ничего не найдено
    endpwent();
    return NULL; // Если пользователь не найден
}

int main() {
    const char *username = "tsr";  // Укажите имя пользователя
    struct passwd *user = my_getpwnam(username);

    if (user != NULL) {
        printf("User found: %s, UID: %d\n", user->pw_name, user->pw_uid);
        free(user);  // Освобождаем память после использования
    } else {
        printf("User %s not found\n", username);
    }

    return 0;
}

/*

### Объяснение:

1. **setpwent()** — открывает файл `/etc/passwd` и устанавливает указатель в начало файла, чтобы мы могли начать чтение с первой записи.
2. **getpwent()** — читает следующую запись в файле `/etc/passwd`, возвращая указатель на структуру `passwd`, или `NULL`, если достигнут конец файла.
3. **endpwent()** — закрывает файл, освобождая ресурсы.

### Примечания:

- Мы копируем найденную структуру `passwd` в динамически выделенную память с помощью `malloc()`, чтобы избежать перезаписи данных при следующем вызове `getpwent()`.
- Не забывайте освобождать выделенную память, используя `free()`, после завершения работы с результатом.
 */
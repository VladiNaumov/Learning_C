/*
 Реализуйте функцию initgroups(), используя setgroups() и библиотечные функции.

Функция `initgroups()` устанавливает все вспомогательные группы для пользователя,
 основываясь на данных о пользователе и его основной группе.
 Она также добавляет пользователя ко всем группам, в которых он состоит.
 Для реализации такой функции с использованием `setgroups()` и стандартных библиотек нам нужно:

1. Найти основную группу пользователя по имени.
2. Собрать список всех групп, в которые входит пользователь.
3. Вызвать `setgroups()` с этим списком.

Функция `setgroups()` требует прав привилегированного процесса, так как она изменяет доступ к группам.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <errno.h>

int my_initgroups(const char *user, gid_t group) {
    struct group *grp;
    gid_t *groups;
    int ngroups = 0;
    int max_groups = 16;  // Начальный размер массива
    int i;

    // Инициализируем массив для хранения идентификаторов групп
    groups = malloc(max_groups * sizeof(gid_t));
    if (groups == NULL) {
        perror("malloc");
        return -1;
    }

    // Добавляем основную группу пользователя
    groups[ngroups++] = group;

    // Проходим по всем группам и добавляем те, в которых состоит пользователь
    setgrent(); // Начинаем чтение файла групп
    while ((grp = getgrent()) != NULL) {
        if (grp->gr_gid == group) {
            continue; // Пропускаем основную группу, она уже добавлена
        }

        // Проверяем, состоит ли пользователь в текущей группе
        for (i = 0; grp->gr_mem[i] != NULL; i++) {
            if (strcmp(grp->gr_mem[i], user) == 0) {
                // Если группа найдена, добавляем её в список
                if (ngroups >= max_groups) {
                    max_groups *= 2;
                    gid_t *new_groups = realloc(groups, max_groups * sizeof(gid_t));
                    if (new_groups == NULL) {
                        free(groups);
                        endgrent();
                        perror("realloc");
                        return -1;
                    }
                    groups = new_groups;
                }
                groups[ngroups++] = grp->gr_gid;
                break;
            }
        }
    }
    endgrent(); // Закрываем файл групп

    // Устанавливаем группы для процесса
    if (setgroups(ngroups, groups) == -1) {
        perror("setgroups");
        free(groups);
        return -1;
    }

    free(groups); // Освобождаем память
    return 0;
}

int main() {
    struct passwd *pwd = getpwnam("username"); // Замените "username" на нужное имя пользователя
    if (pwd == NULL) {
        perror("getpwnam");
        return 1;
    }

    if (my_initgroups(pwd->pw_name, pwd->pw_gid) == -1) {
        fprintf(stderr, "Failed to initialize groups\n");
        return 1;
    }

    printf("Groups initialized successfully for user: %s\n", pwd->pw_name);
    return 0;
}

/*
### Объяснение:

1. **Основная группа** — сразу добавляется в массив `groups`.
2. **Перебор всех групп**:
- `setgrent()` открывает файл групп.
- `getgrent()` последовательно возвращает записи о группах, в которых перечислены все пользователи группы.
- Для каждой записи проверяем, состоит ли пользователь в группе, и, если да, добавляем ее в массив `groups`.
3. **`setgroups()`** — используется для установки групп для текущего процесса.
4. **Очистка памяти** — освобождаем выделенную память после завершения.

### Примечания

- Не забудьте, что `setgroups()` требует прав root, поэтому для работы программы процесс должен быть привилегированным.
*/
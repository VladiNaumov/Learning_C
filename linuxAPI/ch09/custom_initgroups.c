/*
 Реализуйте функцию initgroups(), используя setgroups() и библиотечные функции.

 Функция initgroups() в Linux API используется для инициализации группы доступа пользователя.
 Она позволяет установить все группы, к которым принадлежит данный пользователь,
 что особенно полезно при изменении идентификатора пользователя (UID) процесса с помощью функции setuid().
 Для реализации такой функции с использованием `setgroups()` и стандартных библиотек нам нужно:

1. Найти основную группу пользователя по имени.
2. Собрать список всех групп, в которые входит пользователь.
3. Вызвать `setgroups()` с этим списком.

Функция `setgroups()` требует прав привилегированного процесса, так как она изменяет доступ к группам.

*/

#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int my_initgroups(const char *username, gid_t gid) {
    struct group *grp;
    struct passwd *pwd;
    gid_t groups[NGROUPS_MAX];
    int ngroups = 0;

    // Получаем информацию о пользователе
    pwd = getpwnam(username);
    if (pwd == NULL) {
        perror("getpwnam");
        return -1;
    }

    // Добавляем основную группу пользователя
    groups[ngroups++] = gid;

    // Проходим по всем группам и ищем, в каких группах состоит пользователь
    setgrent();
    while ((grp = getgrent()) != NULL) {
        if (grp->gr_gid == gid) {
            continue; // Пропускаем основную группу
        }

        for (char **member = grp->gr_mem; *member != NULL; member++) {
            if (strcmp(*member, username) == 0) {
                // Если пользователь найден в группе, добавляем ее
                if (ngroups < NGROUPS_MAX) {
                    groups[ngroups++] = grp->gr_gid;
                } else {
                    fprintf(stderr, "Too many groups\n");
                    endgrent();
                    return -1;
                }
            }
        }
    }
    endgrent();

    // Устанавливаем группы для пользователя
    if (setgroups(ngroups, groups) == -1) {
        perror("setgroups");
        return -1;
    }

    return 0;
}


/*
Объяснение:
Получение основной группы пользователя: Функция getpwnam() ищет структуру passwd для заданного username,
чтобы получить основную группу пользователя.
Поиск дополнительных групп: С помощью getgrent() итерируем по всем группам системы,
 проверяя, состоит ли пользователь в каждой из них. Если пользователь найден, добавляем идентификатор группы (gid) в массив.

Установка групп: Вызов setgroups() устанавливает все найденные группы для пользователя.
*/
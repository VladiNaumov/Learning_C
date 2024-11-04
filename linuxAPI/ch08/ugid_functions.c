/*
 Этот код демонстрирует, как получить имя пользователя или группы по их идентификатору (UID или GID) и наоборот,
 найти UID или GID по имени. Код включает четыре функции:

userNameFromId(uid_t uid): принимает UID и возвращает имя пользователя, которому он соответствует.
Если пользователь с таким UID не найден, возвращает NULL.

userIdFromName(const char *name): принимает имя пользователя и возвращает его UID.
Если имя не найдено или введено некорректно, возвращает -1.
Дополнительно проверяет, является ли строка числом, что позволяет сразу получить UID, если он указан в числовой форме.

groupNameFromId(gid_t gid): принимает GID и возвращает имя группы, которой он соответствует.
Возвращает NULL, если группа не найдена.

groupIdFromName(const char *name): принимает имя группы и возвращает её GID.
Если имя группы не найдено или введено некорректно, возвращает -1. Аналогично, если имя является числом, возвращает соответствующее значение сразу.

Таким образом, код демонстрирует работу с системными функциями для поиска информации о пользователях и группах.
 */

#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

/*
 * Возвращает имя, соответствующее `uid`, или NULL в случае ошибки
 */
char *userNameFromId(uid_t uid)
{
    struct passwd *pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}

/*
 * Возвращает UID, соответствующий `name`, или -1 в случае ошибки
 *
 * Используем `int64_t` в качестве возвращаемого типа, чтобы можно было использовать
 * значение `-1` для обозначения ошибки
 */
int64_t userIdFromName(const char *name)
{
    // Пустая строка
    if (name == NULL || *name == '\0') {
        return -1;
    }

    // Имя — это строка, представляющая число
    char *endptr;
    errno = 0;  // Сбрасываем errno перед вызовом strtol
    long val = strtol(name, &endptr, 10);

    // Проверка на ошибки: выход за пределы диапазона или отсутствие преобразования
    if (errno == ERANGE || val < 0 || val > UINT_MAX || *endptr != '\0') {
        struct passwd *pwd = getpwnam(name);
        return (pwd == NULL) ? -1 : (int64_t)pwd->pw_uid;
    }

    return (uid_t)val;
}

/*
 * Возвращает имя, соответствующее `gid`, или NULL в случае ошибки
 */
char *groupNameFromId(gid_t gid)
{
    struct group *grp = getgrgid(gid);
    return (grp == NULL) ? NULL : grp->gr_name;
}

/*
 * Возвращает GID, соответствующий `name`, или -1 в случае ошибки
 */
int64_t groupIdFromName(const char *name)
{
    // Пустая строка
    if (name == NULL || *name == '\0') {
        return -1;
    }

    // Имя — это строка, представляющая число
    char *endptr;
    errno = 0;  // Сбрасываем errno перед вызовом strtol
    long val = strtol(name, &endptr, 10);

    // Проверка на ошибки: выход за пределы диапазона или отсутствие преобразования
    if (errno == ERANGE || val < 0 || val > UINT_MAX || *endptr != '\0') {
        struct group *grp = getgrnam(name);
        return (grp == NULL) ? -1 : (int64_t)grp->gr_gid;
    }

    return (gid_t)val;
}

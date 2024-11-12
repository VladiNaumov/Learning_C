/* file_perms.c

   Возвращает строковое представление маски прав доступа к файлу.
*/

#include <sys/stat.h>
#include <stdio.h>
#include "file_perms.h"                 /* Интерфейс для данной реализации */

#define STR_SIZE sizeof("rwxrwxrwx")     /* Размер строки прав доступа */

/*
 * Функция filePermStr - Возвращает строку в стиле команды ls(1) для маски прав доступа к файлу.
 * Параметры:
 *   perm - маска прав доступа к файлу.
 *   flags - указывает, включать ли специальные биты (set-user-ID, set-group-ID, sticky).
 * Возвращает строку прав доступа.
 */
char *filePermStr(mode_t perm, int flags)
{
    static char str[STR_SIZE];

    /* Если задан флаг FP_SPECIAL, то выводим специальные биты (set-user-ID,
       set-group-ID, sticky) в полях выполнения для пользователя/группы/других.
       Сложность заключается в том, что символ отображается разным в зависимости
       от того, установлен ли соответствующий бит выполнения. */

    snprintf(str, STR_SIZE, "%c%c%c%c%c%c%c%c%c",
        (perm & S_IRUSR) ? 'r' : '-',   // Чтение для владельца
        (perm & S_IWUSR) ? 'w' : '-',   // Запись для владельца
        (perm & S_IXUSR) ?
            (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
            (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 'S' : '-'), // Выполнение для владельца

        (perm & S_IRGRP) ? 'r' : '-',   // Чтение для группы
        (perm & S_IWGRP) ? 'w' : '-',   // Запись для группы
        (perm & S_IXGRP) ?
            (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
            (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 'S' : '-'), // Выполнение для группы

        (perm & S_IROTH) ? 'r' : '-',   // Чтение для остальных
        (perm & S_IWOTH) ? 'w' : '-',   // Запись для остальных
        (perm & S_IXOTH) ?
            (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 't' : 'x') :
            (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 'T' : '-')); // Выполнение для остальных

    return str;
}

/*
Резюме:
Функция filePermStr преобразует маску прав доступа файла в строку, которая отражает
права в стиле команды ls. Она учитывает специальные биты (set-user-ID, set-group-ID, 
sticky), отображая их в зависимости от флагов и значений битов выполнения. Результирующая 
строка подходит для наглядного отображения прав доступа к файлу.
*/


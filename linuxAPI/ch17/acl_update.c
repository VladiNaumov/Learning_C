/* acl_update.c

   Выполняет обновления в списках управления доступом (ACL) файлов, указанных в командной строке.
   Эта программа предоставляет подмножество функциональности команды setfacl(1).
   Для использования см. функцию usageError() ниже.

   Эта программа специфична для Linux. Поддержка ACL появилась с версии Linux 2.6.
   Для сборки этой программы необходимо установить библиотеку ACL (libacl) на вашу систему.
*/
#include <sys/acl.h>
#include <stdbool.h>
#include "ugid_functions.h"
#include "tlpi_hdr.h"

#define MAX_ENTRIES 10000   /* Максимальное количество записей, которое мы можем обработать в ACL */

/* Структура для представления одной записи в ACL */
struct AccessControlEntry {
    acl_tag_t   tag;            /* Тип тега */
    id_t        qual;           /* Необязательный квалификатор тега (UID или GID) */
    int         perms;          /* Маска прав доступа */
};

/* Перечисление для операций */
enum Operation {
    unspecifiedOp, modifyEntries, removeEntries, removeDefaultACL,
    checkValidACL
};

/* Функция вывода ошибки использования программы */
static void
usageError(char *progName, char *msg, bool shortUsage)
{
    if (msg != NULL)
        fprintf(stderr, "%s\n", msg);

    if (shortUsage) {
        fprintf(stderr, "Тип '%s --help' для получения информации о использовании\n", progName);
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Использование: %s -m acl [-d] [-n] file...\n", progName);
    fprintf(stderr, "   или: %s -x acl [-d] [-n] file...\n", progName);
    fprintf(stderr, "   или: %s -k file...\n\n", progName);
    fprintf(stderr, "   или: %s -V acl\n\n", progName);
#define fpe(msg) fprintf(stderr, "      " msg);
    fpe("-m  Изменить/создать записи ACL\n");
    fpe("-x  Удалить записи ACL\n");
    fpe("-k  Удалить default ACL\n\n");
    fpe("-V  Проверить допустимость 'acl'\n\n");
    fpe("'acl' состоит из одной или нескольких записей, разделенных запятыми, в формате:\n");
    fpe("\n            tag:[qualifier][:[perms]]\n\n");
    fpe("      'perms' указываются только для -m\n\n");
    fpe("-d  Применить операцию к default ACL\n");
    fpe("-n  Не пересчитывать маску записи\n");
    fpe("      ПРИМЕЧАНИЕ: если указать эту опцию при -m, могут возникнуть ошибки, если\n");
    fpe("      файл не имеет записи маски.\n");
    exit(EXIT_FAILURE);
}

/* Разбор строки записи ACL и сохранение информации в структуре 'ace'.
   Если 'permsReqd' установлено, то строки ACL должны содержать права (rwx), 
   иначе не должны. */

static bool
parseEntrySpec(char *entryStr, struct AccessControlEntry *ace,
               bool permsReqd)
{
    char *colon1 = strchr(entryStr, ':');
    if (colon1 == NULL) {
        fprintf(stderr, "Отсутствует начальная двоеточие в записи ACL: %s\n", entryStr);
        return false;
    }

    bool hasQual = *(colon1 + 1) != '\0' && *(colon1 + 1) != ':';

    *colon1 = '\0';     /* Добавляем терминатор для типа тега */

    char *colon2 = strchr(colon1 + 1, ':');
    bool hasPerms = colon2 != NULL && *(colon2 + 1) != '\0';

    if (hasPerms && !permsReqd) {
        fprintf(stderr, "Невозможно указать права в этой записи\n");
        return false;
    }

    if (!hasPerms && permsReqd) {
        fprintf(stderr, "Необходимо указать права\n");
        return false;
    }

    /* Определение типа тега, в зависимости от строки тега и наличия квалификатора */

    if (strcmp(entryStr, "u") == 0 || strcmp(entryStr, "user") == 0)
        ace->tag = hasQual ? ACL_USER : ACL_USER_OBJ;
    else if (strcmp(entryStr, "g") == 0 || strcmp(entryStr, "group") == 0)
        ace->tag = hasQual ? ACL_GROUP : ACL_GROUP_OBJ;
    else if (strcmp(entryStr, "o") == 0 || strcmp(entryStr, "other") == 0)
        ace->tag = ACL_OTHER;
    else if (strcmp(entryStr, "m") == 0 || strcmp(entryStr, "mask") == 0)
        ace->tag = ACL_MASK;
    else {
        fprintf(stderr, "Неверный тег: %s\n", entryStr);
        return false;
    }

    /* Для тегов ACL_USER и ACL_GROUP извлекаем UID / GID из квалификатора */

    if (colon2 != NULL)
        *colon2 = '\0';         /* Добавляем терминатор для квалификатора */

    ace->qual = 0;

    if (ace->tag == ACL_USER) {
        ace->qual = userIdFromName(colon1 + 1);
        if (ace->qual == -1) {
            fprintf(stderr, "Неверный идентификатор пользователя: %s\n", colon1 + 1);
            return false;
        }
    } else if (ace->tag == ACL_GROUP) {
        ace->qual = groupIdFromName(colon1 + 1);
        if (ace->qual == -1) {
            fprintf(stderr, "Неверный идентификатор группы: %s\n", colon1 + 1);
            return false;
        }
    }

    /* Если присутствуют права, конвертируем их в маску прав */

    if (hasPerms) {
        ace->perms = 0;

        /* Здесь мы не слишком строгие: не проверяем повторение прав или длину строки прав... */

        for (char *p = colon2 + 1; *p != '\0'; p++) {
            if (*p == 'r')
                ace->perms |= ACL_READ;
            else if (*p == 'w')
                ace->perms |= ACL_WRITE;
            else if (*p == 'x')
                ace->perms |= ACL_EXECUTE;
            else if (*p != '-') {
                fprintf(stderr, "Неверный символ в строке прав: %c\n", *p);
                return false;
            }
        }
    }

    return true;
}

/* Разбор строки ACL, возвращая информацию о записях в 'aclist'. 
   На успех возвращает количество записей, на ошибку -1. */

static int
parseACL(char *aclStr, struct AccessControlEntry aclist[],
        bool permsReqd)
{
    char *comma;
    int n = 0;

    for (char *nextEntry = aclStr; ; nextEntry = comma + 1) {

        if (n >= MAX_ENTRIES) {
            fprintf(stderr, "Слишком много записей в ACL\n");
            return -1;
        }

        comma = strchr(nextEntry, ',');
        if (comma != NULL)
            *comma = '\0';

        if (!parseEntrySpec(nextEntry, &aclist[n], permsReqd))
            return -1;

        if (comma == NULL)              /* Это была последняя запись */
            break;

        n++;
    }

    return n + 1;
}

/* Функция для нахождения записи ACL по типу тега и квалификатору */

static acl_entry_t
findEntry(acl_t acl, acl_tag_t tag, id_t qual)
{
    acl_entry_t entry;
    acl_tag_t entryTag;

    for (int ent = ACL_FIRST_ENTRY; ; ent = ACL_NEXT_ENTRY) {
        int s = acl_get_entry(acl, ent, &entry);
        if (s == -1)
            errExit("acl_get_entry");

        if (s == 0)
            return NULL;

        if (acl_get_tag_type(entry, &entryTag) == -1)
            errExit("acl_get_tag_type");

        if (tag == entryTag) {
            if (tag == ACL_USER) {
                uid_t *uidp = acl_get_qualifier(entry);
                if (uidp == NULL)
                    errExit("acl_get_qualifier");

                if (qual == *uidp) {
                    if (acl_free(uidp) == -1)
                        errExit("acl_free");
                    return entry;
                } else {
                    if (acl_free(uidp) == -1)
                        errExit("acl_free");
                }

            } else if (tag == ACL_GROUP) {
                gid_t *gidp = acl_get_qualifier(entry);
                if (gidp == NULL)
                    errExit("acl_get_qualifier");

                if (qual == *gidp) {
                    if (acl_free(gidp) == -1)
                        errExit("acl_free");
                    return entry;
                } else {
                    if (acl_free(gidp) == -1)
                        errExit("acl_free");
                }
            }
        }
    }

    return NULL;
}

/*
Этот код представляет собой программу для работы с ACL (списками управления доступом) файлов в Linux. Основные моменты, которые он охватывает:

1. **Основная цель программы**: Она позволяет изменять, удалять и проверять записи ACL для файлов. Программа предоставляет несколько операций:
   - `-m` — для изменения/добавления записей ACL.
   - `-x` — для удаления записей ACL.
   - `-k` — для удаления default ACL.
   - `-V` — для проверки допустимости указанной строки ACL.

2. **Структуры данных**:
   - `struct AccessControlEntry` — используется для хранения информации о записи в ACL, включая тег (пользователь, группа и т. д.), квалификатор (UID или GID) и маску прав.
   
3. **Разбор строки ACL**: 
   - Функция `parseEntrySpec` разбирает строку записи в ACL и сохраняет информацию в структуру `AccessControlEntry`.
   - Она также проверяет корректность прав и тегов, например, 'user', 'group', 'other' и другие.
   
4. **Обработка ACL**:
   - Функция `parseACL` анализирует строку, представляющую собой несколько записей ACL, и записывает их в массив `aclist`.
   - Функция `findEntry` находит запись в существующем ACL по типу тега и квалификатору, чтобы ее изменить или удалить.
   
5. **Ошибки и вывод сообщений**:
   - В случае неправильного формата строки ACL или других ошибок, программа выводит соответствующие сообщения об ошибках и завершает выполнение с кодом ошибки.

6. **Дополнительные возможности**:
   - Программа использует функции для получения UID и GID на основе имен (с помощью `userIdFromName` и `groupIdFromName`).
   - Обработка прав доступа с использованием масок прав (`rwx`).

Таким образом, этот код является частью утилиты для работы с ACL в Linux, реализующей операции добавления, удаления и проверки прав доступа для файлов.
*/
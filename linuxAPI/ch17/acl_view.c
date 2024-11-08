/* 
пример программы на C, которая отображает список контроля доступа (ACL) для файла или каталога, 
в зависимости от указанных аргументов командной строки. 
*/


/* acl_view.c

   Отображает список контроля доступа (ACL) на файл.

   Использование: acl_view [-d] file

   Если указана опция '-d', то отображается стандартный ACL (и 'file' должен быть каталогом),
   иначе отображается доступный ACL.

   Эта программа специфична для Linux. ACL поддерживаются с версии Linux 2.6.
   Для компиляции программы необходимо установить библиотеку ACL (libacl).
*/

#include <acl/libacl.h>         // Библиотека для работы с ACL
#include <sys/acl.h>            // Заголовок для работы с ACL
#include "ugid_functions.h"     // Функции для получения имени пользователя/группы по ID
#include "tlpi_hdr.h"           // Заголовок для стандартных функций ошибок

// Функция для отображения правильного использования программы
static void usageError(char *progName)
{
    fprintf(stderr, "Usage: %s [-d] filename\n", progName);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    acl_t acl;                   // Переменная для хранения ACL
    acl_type_t type;             // Тип ACL (доступ или стандартный)
    acl_entry_t entry;           // Переменная для хранения текущей записи ACL
    acl_tag_t tag;               // Тег для текущей записи ACL
    uid_t *uidp;                 // Указатель на UID (для тега пользователя)
    gid_t *gidp;                 // Указатель на GID (для тега группы)
    acl_permset_t permset;       // Переменная для набора прав доступа
    char *name;                  // Имя пользователя/группы
    int entryId, permVal, opt;   // Идентификатор записи, значение прав, опции

    type = ACL_TYPE_ACCESS;      // По умолчанию показывать доступный ACL
    while ((opt = getopt(argc, argv, "d")) != -1) {  // Обработка аргументов командной строки
        switch (opt) {
        case 'd': type = ACL_TYPE_DEFAULT;  // Если опция -d, показывать стандартный ACL
            break;
        case '?': usageError(argv[0]);      // Ошибка в случае неправильных аргументов
        }
    }

    // Проверка на количество аргументов командной строки
    if (optind + 1 != argc)
        usageError(argv[0]);

    // Получаем ACL для указанного файла или каталога
    acl = acl_get_file(argv[optind], type);
    if (acl == NULL)
        errExit("acl_get_file");

    // Проходим по каждой записи в ACL
    for (entryId = ACL_FIRST_ENTRY; ; entryId = ACL_NEXT_ENTRY) {

        // Получаем следующую запись
        if (acl_get_entry(acl, entryId, &entry) != 1)
            break;  // Выход при ошибке или отсутствии записей

        // Получаем тип тега текущей записи
        if (acl_get_tag_type(entry, &tag) == -1)
            errExit("acl_get_tag_type");

        // Выводим тип тега записи
        printf("%-12s", (tag == ACL_USER_OBJ) ?  "user_obj" :
                        (tag == ACL_USER) ?      "user" :
                        (tag == ACL_GROUP_OBJ) ? "group_obj" :
                        (tag == ACL_GROUP) ?     "group" :
                        (tag == ACL_MASK) ?      "mask" :
                        (tag == ACL_OTHER) ?     "other" : "???");

        // Если тег — это пользователь или группа, получаем и выводим их имя
        if (tag == ACL_USER) {
            uidp = acl_get_qualifier(entry);
            if (uidp == NULL)
                errExit("acl_get_qualifier");

            name = userNameFromId(*uidp);   // Получаем имя пользователя
            if (name == NULL)
                printf("%-8d ", *uidp);
            else
                printf("%-8s ", name);

            // Освобождаем память, выделенную для UID
            if (acl_free(uidp) == -1)
                errExit("acl_free");

        } else if (tag == ACL_GROUP) {
            gidp = acl_get_qualifier(entry);
            if (gidp == NULL)
                errExit("acl_get_qualifier");

            name = groupNameFromId(*gidp);  // Получаем имя группы
            if (name == NULL)
                printf("%-8d ", *gidp);
            else
                printf("%-8s ", name);

            // Освобождаем память, выделенную для GID
            if (acl_free(gidp) == -1)
                errExit("acl_free");

        } else {
            printf("         ");  // Если тег не пользователь или группа
        }

        // Получаем и отображаем разрешения для этой записи
        if (acl_get_permset(entry, &permset) == -1)
            errExit("acl_get_permset");

        // Проверка и вывод прав доступа
        permVal = acl_get_perm(permset, ACL_READ);
        if (permVal == -1)
            errExit("acl_get_perm - ACL_READ");
        printf("%c", (permVal == 1) ? 'r' : '-');
        permVal = acl_get_perm(permset, ACL_WRITE);
        if (permVal == -1)
            errExit("acl_get_perm - ACL_WRITE");
        printf("%c", (permVal == 1) ? 'w' : '-');
        permVal = acl_get_perm(permset, ACL_EXECUTE);
        if (permVal == -1)
            errExit("acl_get_perm - ACL_EXECUTE");
        printf("%c", (permVal == 1) ? 'x' : '-');

        printf("\n");
    }

    // Освобождаем память, выделенную для ACL
    if (acl_free(acl) == -1)
        errExit("acl_free");

    exit(EXIT_SUCCESS);
}
/*

### Пояснение программы:
1. **Аргументы командной строки**:
   - Программа принимает один обязательный аргумент: имя файла или каталога, для которого нужно вывести ACL.
   - Если указан параметр `-d`, программа будет выводить стандартный ACL (только для каталогов), иначе она выводит доступный ACL.

2. **Работа с ACL**:
   - Функция `acl_get_file()` используется для получения списка контроля доступа для указанного файла или каталога.
   - Затем программа проходит по каждой записи в ACL с помощью `acl_get_entry()` и извлекает информацию о теге (например, пользователь, группа, другие).
   - Права доступа (чтение, запись, выполнение) извлекаются с помощью `acl_get_permset()` и выводятся.

3. **Теги ACL**:
   - В зависимости от типа записи (`ACL_USER`, `ACL_GROUP` и т. д.), программа выводит имя пользователя или группы. Для пользователя используется функция `userNameFromId()`, а для группы — `groupNameFromId()`.

4. **Вывод прав доступа**:
   - Для каждой записи ACL отображаются права доступа в виде `r`, `w`, `x` или `-`, если прав нет.

### Компиляция и запуск:
Для компиляции программы:

```bash
gcc acl_view.c -o acl_view -lacl
```

Чтобы запустить программу:

```bash
./acl_view -d mydirectory  # Для отображения стандартного ACL каталога
./acl_view myfile          # Для отображения доступного ACL файла
```

### Примечания:
- Необходима библиотека `libacl` для работы с ACL.
- Для работы с ACL в Linux требуется ядро, поддерживающее ACL (начиная с версии 2.6).

*/
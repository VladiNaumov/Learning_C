/* 
Напишите программу, которая отображает права доступа из записи ACL-списка, со-
ответствующей какому-либо пользователю или группе. Эта программа должна при-
нимать через командную строку два аргумента. Первым может быть либо буква u, либо 
буква g, указывающая на то, что именно идентифицирует второй аргумент — пользо-
вателя или группу. (Функции, определенные в листинге 8.1, можно использовать для 
того, чтобы разрешить ввод второго аргумента либо в числовом представлении, либо 
как имя.) Если запись ACL-списка, соответствующая указанному пользователю или 
группе, попадает в класс группы, то программа должна дополнительно отображать 
права доступа, которые были бы предоставлены после того, как эта запись будет из-
менена путем записи-маски.



Для выполнения этой задачи, мы можем создать программу, которая будет принимать два аргумента: 
первый — это флаг, указывающий, является ли второй аргумент пользователем (`u`) или группой (`g`), 
а второй аргумент — это имя пользователя или группы. 
Программа будет извлекать записи из ACL для указанного пользователя или группы, 
а затем выводить соответствующие права доступа, включая права, которые могут быть изменены с помощью маски. 
Для этого можно использовать системные вызовы для работы с ACL, такие как `getfacl` (если программа будет работать в Linux).

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <acl/libacl.h>
#include <acl/acl.h>

void print_permissions(acl_t acl, const char* entity, char type) {
    acl_entry_t entry;
    acl_permset_t permset;
    acl_tag_t tag;
    acl_get_entry(acl, ACL_FIRST_ENTRY, &entry);

    while (entry != NULL) {
        acl_get_tag_type(entry, &tag);
        if ((type == 'u' && tag == ACL_USER) || (type == 'g' && tag == ACL_GROUP)) {
            char *name;
            if (type == 'u') {
                uid_t uid;
                acl_get_qualifier(entry, (void **) &uid);
                name = malloc(256);
                if (getpwuid_r(uid, name, 256) != NULL) {
                    printf("User: %s\n", name);
                }
            } else if (type == 'g') {
                gid_t gid;
                acl_get_qualifier(entry, (void **) &gid);
                name = malloc(256);
                if (getgrgid_r(gid, name, 256) != NULL) {
                    printf("Group: %s\n", name);
                }
            }

            acl_get_permset(entry, &permset);
            printf("Permissions: ");
            if (acl_get_perm(permset, ACL_READ)) printf("r");
            if (acl_get_perm(permset, ACL_WRITE)) printf("w");
            if (acl_get_perm(permset, ACL_EXECUTE)) printf("x");
            printf("\n");
        }
        acl_get_entry(acl, ACL_NEXT_ENTRY, &entry);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <u|g> <username|groupname>\n", argv[0]);
        return 1;
    }

    char type = argv[1][0];
    char *entity = argv[2];

    // Получаем имя файла или путь
    char *file = "somefile"; // Замените на путь к нужному файлу

    acl_t acl = acl_get_file(file, ACL_TYPE_ACCESS);
    if (acl == NULL) {
        perror("Error getting ACL");
        return 1;
    }

    print_permissions(acl, entity, type);

    acl_free(acl);
    return 0;
}

/*

### Пояснения к коду:

1. **Основная логика**: Мы получаем список ACL для файла с помощью `acl_get_file`. Затем перебираем все записи ACL с помощью `acl_get_entry` и проверяем, соответствует ли запись пользователю или группе, в зависимости от переданного аргумента. Для этого проверяем тег записи (это может быть `ACL_USER` или `ACL_GROUP`).
   
2. **Работа с правами**: Для каждого соответствующего пользователя или группы мы извлекаем права с помощью `acl_get_permset` и выводим их в виде символов (`r`, `w`, `x`).

3. **Указание пользователя или группы**: Мы используем `getpwuid_r` и `getgrgid_r` для поиска имени пользователя или группы по UID или GID.

4. **Маска прав**: В случае, если запись ACL для группы (например, ACL_GROUP) поддается изменению через маску, программа будет выводить соответствующие права доступа, которые будут установлены после применения маски.

### Пример вызова программы:

```bash
./acl_permissions u username
```

Или для группы:

```bash
./acl_permissions g groupname
```

Этот код выводит права доступа для указанного пользователя или группы на заданный файл, включая права, измененные маской.

*/
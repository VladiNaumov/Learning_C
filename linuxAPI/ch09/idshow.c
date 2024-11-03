/*
 Этот код выводит информацию о пользователе и группах процесса, включая основные идентификаторы (UID, GID) и вспомогательные группы, для которых у него есть доступ.

### Основные функции кода

1. **Получение UID и GID**:
   - `getresuid(&ruid, &euid, &suid)` и `getresgid(&rgid, &egid, &sgid)` получают **реальный** (real), **действующий** (effective) и **сохранённый** (saved) идентификаторы пользователя (UID) и группы (GID).

2. **Получение файловых идентификаторов**:
   - `setfsuid(0)` и `setfsgid(0)` пытаются установить файловые идентификаторы (file-system IDs) для UID и GID. Это полезно для процессов, у которых есть root-права, чтобы выполнять операции с правами другого пользователя. Для непривилегированных процессов `setfsuid(0)` и `setfsgid(0)` просто возвращают текущие файловые идентификаторы.

3. **Вывод UID и GID**:
   - Используется функция `userNameFromId()` (предполагается, что она конвертирует UID в имя пользователя) и `groupNameFromId()` (конвертирует GID в название группы).
   - Если функция не может найти имя пользователя или группу, выводится `???`.
   - Выводятся **реальный**, **действующий**, **сохранённый** и **файловый** идентификаторы UID и GID.

4. **Получение вспомогательных групп**:
   - `getgroups(SG_SIZE, suppGroups)` получает список вспомогательных групп, к которым принадлежит процесс. Максимальное количество групп определяется `NGROUPS_MAX + 1`.

5. **Вывод вспомогательных групп**:
   - Каждая группа выводится в формате `группа (GID)`.

### Пример вывода

Примерный вывод программы может выглядеть следующим образом:
```
UID: real=user1 (1000); eff=root (0); saved=root (0); fs=user1 (1000);
GID: real=users (100); eff=root (0); saved=root (0); fs=users (100);
Supplementary groups (2): sudo (27) users (100)
```

### Примечания:
- **Файловые идентификаторы** (fsuid, fsgid) определяют права доступа к файловой системе.
- Эта программа полезна для отладки прав доступа процесса, например, в случае выполнения `set-user-ID` или `set-group-ID` программ, где права процесса могут изменяться.
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/fsuid.h>
#include <limits.h>
#include "ugid_functions.h"   /* userNameFromId() & groupNameFromId() */
#include "tlpi_hdr.h"

#define SG_SIZE (NGROUPS_MAX + 1)

int
main(int argc, char *argv[])
{
    uid_t ruid, euid, suid, fsuid;
    gid_t rgid, egid, sgid, fsgid;
    gid_t suppGroups[SG_SIZE];
    int numGroups, j;
    char *p;

    if (getresuid(&ruid, &euid, &suid) == -1)
        errExit("getresuid");
    if (getresgid(&rgid, &egid, &sgid) == -1)
        errExit("getresgid");

    /* Попытки изменить файловые идентификаторы всегда игнорируются
       для непривилегированных процессов, но, тем не менее, следующие
       вызовы возвращают текущие файловые идентификаторы */

    fsuid = setfsuid(0);
    fsgid = setfsgid(0);

    printf("UID: ");
    p = userNameFromId(ruid);
    printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long) ruid);
    p = userNameFromId(euid);
    printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long) euid);
    p = userNameFromId(suid);
    printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long) suid);
    p = userNameFromId(fsuid);
    printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long) fsuid);
    printf("\n");

    printf("GID: ");
    p = groupNameFromId(rgid);
    printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long) rgid);
    p = groupNameFromId(egid);
    printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long) egid);
    p = groupNameFromId(sgid);
    printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long) sgid);
    p = groupNameFromId(fsgid);
    printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long) fsgid);
    printf("\n");

    numGroups = getgroups(SG_SIZE, suppGroups);
    if (numGroups == -1)
        errExit("getgroups");

    printf("Supplementary groups (%d): ", numGroups);
    for (j = 0; j < numGroups; j++) {
        p = groupNameFromId(suppGroups[j]);
        printf("%s (%ld) ", (p == NULL) ? "???" : p, (long) suppGroups[j]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}


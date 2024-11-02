/* 9.Process Credentials */

/*
 Каждый процесс связан с набором числовых идентификаторов пользователя (UID) и группы (GID).
 Иногда их называют идентификаторами процесса.

 В число этих идентификаторов входят:
1. real user ID and group ID - реальный ID пользователя и группы);
2. effective  UID and GID - действующий ID пользователя и группы;
3. saved set-user-ID and saved set-group-ID - сохраненный установленный ID пользователя (saved set-user-ID), сохраненный установленный ID группы (saved set-group-ID);
4. file-system UID and GID (Linux-specific) - характерный для Linux пользовательский.

 */

/*
1. Системные вызовы:  getuid(), geteuid(), getgid(), getegid().
Эти функции возвращают идентификаторы текущего процесса:


Применимость: Эти функции могут пригодиться для проверки прав текущего пользователя перед выполнением операций, требующих привилегий суперпользователя, но не выполняют изменений.
*/
#include <unistd.h>

uid_t getuid(void); // возвращает реальный идентификатор пользователя (UID).
uid_t geteuid(void); // возвращает эффективный идентификатор пользователя (EUID), который определяет права доступа.
gid_t getgid(void); // возвращает реальный идентификатор группы (GID).
gid_t getegid(void); // возвращает эффективный идентификатор группы (EGID), используемый для проверки прав доступа

#include <stdio.h>
#include <unistd.h>

int main() {
    uid_t uid = getuid();         // Получаем реальный UID
    uid_t euid = geteuid();       // Получаем эффективный UID
    gid_t gid = getgid();         // Получаем реальный GID
    gid_t egid = getegid();       // Получаем эффективный GID

    printf("Реальный UID: %d\n", uid);
    printf("Эффективный UID: %d\n", euid);
    printf("Реальный GID: %d\n", gid);
    printf("Эффективный GID: %d\n", egid);

    return 0;
}


/*
2. Системные вызовы:  setreuid(), setregid(), getresuid(), getresgid(), setresuid(), setresgid()
Эти вызовы позволяют управлять реальными, эффективными и сохраненными идентификаторами процесса.


Применимость: Полезны для гибкого управления правами процесса, но не для изменения данных о пользователях или группах.
*/

#include <unistd.h>
#define _GNU_SOURCE

int setreuid(uid_t ruid, uid_t euid); // — устанавливает реальный и эффективный UID для текущего процесса.
int setregid(gid_t rgid, gid_t egid); //  — устанавливает реальный и эффективный GID для текущего процесса

int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid); // — возвращает реальный, эффективный и сохранённый UID.
int getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid); // — возвращает реальный, эффективный и сохранённый GID.


int setresuid(uid_t ruid, uid_t euid, uid_t suid); // — устанавливает реальный, эффективный и сохранённый UID
int setresgid(gid_t rgid, gid_t egid, gid_t sgid); // — устанавливает реальный, эффективный и сохранённый GID.


/* Этот пример показывает, как использовать setresuid() и getresuid() для изменения UID процесса и проверки изменений. */

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    uid_t ruid, euid, suid;

    // Получаем текущие UID (реальный, эффективный, сохранённый)
    if (getresuid(&ruid, &euid, &suid) == -1) {
        perror("getresuid failed");
        return 1;
    }
    printf("Текущий UID: real=%d, effective=%d, saved=%d\n", ruid, euid, suid);

    // Пробуем установить реальный и эффективный UID в 0 (суперпользователь)
    if (setresuid(0, 0, suid) == -1) {
        perror("setresuid failed");
        return 1;
    }

    // Снова получаем UID, чтобы проверить изменения
    if (getresuid(&ruid, &euid, &suid) == -1) {
        perror("getresuid failed");
        return 1;
    }
    printf("Новый UID: real=%d, effective=%d, saved=%d\n", ruid, euid, suid);

    return 0;
}



/*
3. getgroups(), setgroups(), initgroups() Эти вызовы работают с дополнительными группами:

getgroups() возвращает список групп, к которым принадлежит процесс.
setgroups() позволяет задать список дополнительных групп для процесса.
initgroups() добавляет пользователя в группы, указанные для него в системной конфигурации.

Привилегированный процесс может использовать setgroups() и initgroups() для изменения своего набора дополнительных идентификаторов групп.

Применимость: initgroups() может быть полезна для обновления групп пользователя после изменения его записи в системе, 
но для внесения изменений в группы пользователей потребуется использовать командные утилиты или специализированные функции, такие как usermod.

Системный вызов getgroups() возвращает набор групп, членом которых является вызывающий процесс в массив, на который указывает grouplist.

*/

#include <unistd.h>

int getgroups(int gidsetsize, gid_t grouplist[]);  // возвращает список групп, к которым принадлежит процесс.
int setgroups(size_t gidsetsize, const gid_t *grouplist); // озволяет задать список дополнительных групп для процесса.
int initgroups(const char *user, gid_t group); // добавляет пользователя в группы, указанные для него в системной конфигурации

#include <stdio.h>
#include <unistd.h>
#include <grp.h>

int main() {
    gid_t groups[NGROUPS_MAX];
    int ngroups;

    ngroups = getgroups(NGROUPS_MAX, groups);
    if (ngroups == -1) {
        perror("getgroups failed");
        return 1;
    }

    printf("Number of groups: %d\n", ngroups);
    for (int i = 0; i < ngroups; i++) {
        printf("Group ID: %d\n", groups[i]);
    }
    return 0;
}


#include <stdio.h>
#include <unistd.h>

int main() {
    // Получаем реальный и эффективный идентификаторы пользователя
    uid_t real_uid = getuid();
    uid_t effective_uid = geteuid();

    // Получаем реальный и эффективный идентификаторы группы
    gid_t real_gid = getgid();
    gid_t effective_gid = getegid();

    // Выводим полученные идентификаторы
    printf("Реальный UID: %d\n", real_uid);
    printf("Эффективный UID: %d\n", effective_uid);
    printf("Реальный GID: %d\n", real_gid);
    printf("Эффективный GID: %d\n", effective_gid);

    return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>

int main() {
    const char *username = "testuser";  // Имя пользователя, которому задаем группы
    gid_t base_gid = 1000;              // Основной GID пользователя
    gid_t additional_groups[] = {1001, 1002};  // Дополнительные группы (пример GID)
    int num_groups = sizeof(additional_groups) / sizeof(additional_groups[0]);

    // Использование setgroups() для задания дополнительных групп
    if (setgroups(num_groups, additional_groups) == -1) {
        perror("setgroups failed");
        return 1;
    }
    printf("Дополнительные группы установлены через setgroups().\n");

    // Использование initgroups() для автоматического добавления групп пользователя
    if (initgroups(username, base_gid) == -1) {
        perror("initgroups failed");
        return 1;
    }
    printf("Группы пользователя %s обновлены с помощью initgroups().\n", username);

    return 0;
}


/*
4. Системные вызовы setuid() и setgid() позволяют изменять идентификаторы пользователя и группы текущего процесса. 
Они используются для управления правами доступа и безопасностью, предоставляя процессам временные или постоянные привилегии другого пользователя или группы.

setuid(uid) — устанавливает эффективный, а иногда и реальный, сохраненный идентификаторы пользователя на значение uid. 
Этот вызов полезен, когда процесс должен временно обладать привилегиями другого пользователя для выполнения определенных задач.
setgid(gid) — аналогично, но для группы.
*/
#include <unistd.h>

int setuid(uid_t uid); // изменяет эффективный идентификатор пользователя 
int setgid(gid_t gid); // изменяет эффективный идентификатор групп

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    uid_t new_uid = 1001; // Установите идентификатор пользователя по вашему выбору
    gid_t new_gid = 1001; // Установите идентификатор группы по вашему выбору

    if (setgid(new_gid) == -1) {
        perror("setgid failed");
        return 1;
    }

    if (setuid(new_uid) == -1) {
        perror("setuid failed");
        return 1;
    }

    printf("Changed UID to %d and GID to %d\n", new_uid, new_gid);
    return 0;
}




/*
5. Системные вызовы seteuid() и setegid() позволяют процессу временно изменять свои эффективные идентификаторы пользователя и группы, предоставляя временные права на выполнение определённых задач.

seteuid(euid) — меняет только эффективный идентификатор пользователя на указанный euid, что позволяет процессу временно использовать привилегии другого пользователя, не изменяя реальный и сохранённый идентификаторы.
setegid(egid) — аналогично, но меняет только эффективный идентификатор группы на указанный egid.

Эти вызовы помогают временно повысить или понизить права процесса. 
Например, они полезны в программах, которым для выполнения административных задач нужно временно изменить уровень доступа, а затем вернуться к менее привилегированному состоянию для безопасности.
*/
#include <unistd.h>

int seteuid(uid_t euid); // меняет только эффективный идентификатор пользователя на указанный euid
int setegid(gid_t egid); // меняет только эффективный идентификатор группы на указанный egid

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    uid_t new_euid = 1001; // Эффективный идентификатор пользователя
    gid_t new_egid = 1001; // Эффективный идентификатор группы

    if (setegid(new_egid) == -1) {
        perror("setegid failed");
        return 1;
    }

    if (seteuid(new_euid) == -1) {
        perror("seteuid failed");
        return 1;
    }

    printf("Changed Effective UID to %d and Effective GID to %d\n", new_euid, new_egid);
    return 0;
}



/*
6. 
В Linux существуют специфичные системные вызовы setfsuid() и setfsgid(), которые позволяют изменить идентификаторы файловой системы отдельно от эффективных идентификаторов пользователя и группы. 
Эти вызовы обеспечивают контроль доступа к файловой системе независимо от привилегий, которыми обладает процесс.

setfsuid(fsuid) — устанавливает идентификатор файловой системы пользователя на значение fsuid. 
Это используется для определения прав доступа к файлам, независимо от текущего эффективного идентификатора пользователя.

setfsgid(fsgid) — аналогично, но изменяет идентификатор файловой системы группы на значение fsgid.
Эти вызовы полезны для случаев, когда процессу нужно выполнять операции с файловой системой с привилегиями определённого пользователя или группы, 
не меняя при этом своих основных привилегий.
*/
#include <sys/fsuid.h>

int setfsuid(uid_t fsuid);
int setfsgid(gid_t fsgid);

#include <stdio.h>
#include <unistd.h>
#include <sys/fsuid.h>
#include <errno.h>

int main() {
    uid_t new_fsuid = 1001;
    gid_t new_fsgid = 1001;

    if (setfsgid(new_fsgid) == -1) {
        perror("setfsgid failed");
        return 1;
    }

    if (setfsuid(new_fsuid) == -1) {
        perror("setfsuid failed");
        return 1;
    }

    printf("Changed File System UID to %d and File System GID to %d\n", new_fsuid, new_fsgid);
    return 0;
}




/*
Примечания:
Некоторые из этих вызовов могут потребовать прав суперпользователя для успешного выполнения.
Замените идентификаторы пользователей и групп на действительные значения, чтобы проверить код в вашей системе.
 */
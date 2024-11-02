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
1. Системные вызовы:  getuid(), geteuid(), getgid(), getegid()

Эти функции возвращают идентификаторы текущего процесса:

getuid(): возвращает реальный идентификатор пользователя (UID).
geteuid(): возвращает эффективный идентификатор пользователя (EUID), который определяет права доступа.
getgid(): возвращает реальный идентификатор группы (GID).
getegid(): возвращает эффективный идентификатор группы (EGID), используемый для проверки прав доступа

Применимость: Эти функции могут пригодиться для проверки прав текущего пользователя перед выполнением операций, требующих привилегий суперпользователя, но не выполняют изменений.
*/
#include <unistd.h>

uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);

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


int setreuid(uid_t ruid, uid_t euid); — устанавливает реальный и эффективный UID для текущего процесса.
int setregid(gid_t rgid, gid_t egid); — устанавливает реальный и эффективный GID для текущего процесса.


int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid); — возвращает реальный, эффективный и сохранённый UID.
int getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid); — возвращает реальный, эффективный и сохранённый GID.


int setresuid(uid_t ruid, uid_t euid, uid_t suid); — устанавливает реальный, эффективный и сохранённый UID.
int setresgid(gid_t rgid, gid_t egid, gid_t sgid); — устанавливает реальный, эффективный и сохранённый GID.


Применимость: Полезны для гибкого управления правами процесса, но не для изменения данных о пользователях или группах.
*/

#include <unistd.h>
#define _GNU_SOURCE

int setreuid(uid_t ruid, uid_t euid);
int setregid(gid_t rgid, gid_t egid);

int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);
int getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid);


int setresuid(uid_t ruid, uid_t euid, uid_t suid);
int setresgid(gid_t rgid, gid_t egid, gid_t sgid);


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

int getgroups(int gidsetsize, gid_t grouplist[]);

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
4. Системный вызов setuid() изменяет эффективный идентификатор пользователя — и, возможно, реальный идентификатор пользователя и сохраненный идентификатор пользователя — вызывающего процесса на значение, заданное аргументом uid. Системный вызов setgid() выполняет аналогичную задачу для соответствующих идентификаторов групп.
*/
#include <unistd.h>

int setuid(uid_t uid);
int setgid(gid_t gid);

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
5. Процесс может использовать seteuid() для изменения своего эффективного идентификатора пользователя (на значение, указанное в euid), и setegid() для изменения своего эффективного идентификатора группы (на значение, указанное в egid).
*/
#include <unistd.h>

int seteuid(uid_t euid);
int setegid(gid_t egid);

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
6. Все ранее описанные системные вызовы, которые изменяют эффективный идентификатор пользователя или группы процесса, 
также всегда изменяют соответствующий идентификатор файловой системы. 
Чтобы изменить идентификаторы файловой системы независимо от эффективных идентификаторов, 
мы должны использовать два специфичных для Linux системных вызова: setfsuid() и setfsgid().
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
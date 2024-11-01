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
1. Системные вызовы getuid() и getgid() возвращают, соответственно, реальный идентификатор пользователя и реальный идентификатор группы вызывающего процесса. Системные вызовы geteuid() и getegid() выполняют аналогичные задачи для эффективных идентификаторов. Эти системные вызовы всегда успешны.
*/
#include <unistd.h>

uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);

/*
2. Системный вызов setuid() изменяет эффективный идентификатор пользователя — и, возможно, реальный идентификатор пользователя и сохраненный идентификатор пользователя — вызывающего процесса на значение, заданное аргументом uid. Системный вызов setgid() выполняет аналогичную задачу для соответствующих идентификаторов групп.
*/
#include <unistd.h>

int setuid(uid_t uid);
int setgid(gid_t gid);

/*
3. Процесс может использовать seteuid() для изменения своего эффективного идентификатора пользователя (на значение, указанное в euid), и setegid() для изменения своего эффективного идентификатора группы (на значение, указанное в egid).
*/
#include <unistd.h>

int seteuid(uid_t euid);
int setegid(gid_t egid);

/*
4. Системный вызов setreuid() позволяет вызывающему процессу независимо изменить значения своих реального и эффективного идентификаторов пользователя. Системный вызов setregid() выполняет аналогичную задачу для реальных и эффективных идентификаторов групп.
*/
#include <unistd.h>

int setreuid(uid_t ruid, uid_t euid);
int setregid(gid_t rgid, gid_t egid);

/*
5. В большинстве UNIX-реализаций процесс не может напрямую получить (или обновить) свой сохраненный идентификатор пользователя и сохраненный идентификатор группы. Однако Linux предоставляет два (нестандартных) системных вызова, позволяющих сделать это: getresuid() и getresgid().
*/

#define _GNU_SOURCE
#include <unistd.h>

int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);
int getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid);

/*
6. Системный вызов setresuid() позволяет вызывающему процессу независимо изменить значения всех трех своих идентификаторов пользователя. Новые значения для каждого из идентификаторов пользователя задаются тремя аргументами системного вызова. Системный вызов setresgid() выполняет аналогичную задачу для идентификаторов групп.
*/

#define _GNU_SOURCE
#include <unistd.h>

int setresuid(uid_t ruid, uid_t euid, uid_t suid);
int setresgid(gid_t rgid, gid_t egid, gid_t sgid);

/*
7. Все ранее описанные системные вызовы, которые изменяют эффективный идентификатор пользователя или группы процесса, также всегда изменяют соответствующий идентификатор файловой системы. Чтобы изменить идентификаторы файловой системы независимо от эффективных идентификаторов, мы должны использовать два специфичных для Linux системных вызова: setfsuid() и setfsgid().
*/
#include <sys/fsuid.h>

int setfsuid(uid_t fsuid);
int setfsgid(gid_t fsgid);

/*
8. Системный вызов getgroups() возвращает набор групп, членом которых является вызывающий процесс в массив, на который указывает grouplist.
*/
#include <unistd.h>

int getgroups(int gidsetsize, gid_t grouplist[]);

/*
9. Привилегированный процесс может использовать setgroups() и initgroups() для изменения своего набора дополнительных идентификаторов групп.
*/

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
#include <unistd.h>

int main() {
    uid_t uid = getuid();
    gid_t gid = getgid();

    printf("Real User ID: %d\n", uid);
    printf("Real Group ID: %d\n", gid);

    return 0;
}

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


#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    uid_t new_ruid = 1001; // Новый реальный идентификатор пользователя
    uid_t new_euid = 1002; // Новый эффективный идентификатор пользователя

    if (setreuid(new_ruid, new_euid) == -1) {
        perror("setreuid failed");
        return 1;
    }

    printf("Changed Real UID to %d and Effective UID to %d\n", new_ruid, new_euid);
    return 0;
}


#include <stdio.h>
#include <unistd.h>

int main() {
    uid_t ruid, euid, suid;

    if (getresuid(&ruid, &euid, &suid) == -1) {
        perror("getresuid failed");
        return 1;
    }

    printf("Real UID: %d, Effective UID: %d, Saved UID: %d\n", ruid, euid, suid);
    return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    uid_t new_ruid = 1001, new_euid = 1002, new_suid = 1003;

    if (setresuid(new_ruid, new_euid, new_suid) == -1) {
        perror("setresuid failed");
        return 1;
    }

    printf("Changed Real UID to %d, Effective UID to %d, Saved UID to %d\n", new_ruid, new_euid, new_suid);
    return 0;
}

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
#include <grp.h>
#include <stdlib.h>

int main() {
    gid_t groups[] = {1001, 1002}; // Замените на идентификаторы групп по вашему выбору
    size_t ngroups = sizeof(groups) / sizeof(groups[0]);

    if (setgroups(ngroups, groups) == -1) {
        perror("setgroups failed");
        return 1;
    }

    printf("Changed supplementary groups successfully.\n");
    return 0;
}


/*
Примечания:
Некоторые из этих вызовов могут потребовать прав суперпользователя для успешного выполнения.
Замените идентификаторы пользователей и групп на действительные значения, чтобы проверить код в вашей системе.
 */
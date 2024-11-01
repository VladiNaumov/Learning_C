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
1. The getuid() and getgid() system calls return, respectively, the real user ID and real
group ID of the calling process. The geteuid() and getegid() system calls perform the
corresponding tasks for the effective IDs. These system calls are always successful.
 */
#include <unistd.h>

uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);

/*
2. The setuid() system call changes the effective user ID—and possibly the real user ID
and the saved set-user-ID—of the calling process to the value given by the uid argu-
ment. The setgid() system call performs the analogous task for the corresponding
group IDs.
 */
#include <unistd.h>

int setuid(uid_t  uid );
int setgid(gid_t  gid );

/*
3. A process can use seteuid() to change its effective user ID (to the value specified
by euid), and setegid() to change its effective group ID (to the value specified by egid).
 */
#include <unistd.h>

int seteuid(uid_t  euid );
int setegid(gid_t  egid );

/*
 4. The setreuid() system call allows the calling process to independently change the
values of its real and effective user IDs. The setregid() system call performs the anal-
ogous task for the real and effective group IDs.
 */
#include <unistd.h>

int setreuid(uid_t  ruid , uid_t  euid );
int setregid(gid_t  rgid , gid_t  egid );

/*
 5. On most UNIX implementations, a process can’t directly retrieve (or update) its
saved set-user-ID and saved set-group-ID. However, Linux provides two (nonstand-
ard) system calls allowing us to do just that: getresuid() and getresgid().
 */

#define _GNU_SOURCE
#include <unistd.h>

int getresuid(uid_t * ruid , uid_t * euid , uid_t * suid );
int getresgid(gid_t * rgid , gid_t * egid , gid_t * sgid);

/*
 6. The setresuid() system call allows the calling process to independently change the
values of all three of its user IDs. The new values for each of the user IDs are speci-
fied by the three arguments to the system call. The setresgid() system call performs
the analogous task for the group IDs.
 */

#define _GNU_SOURCE
#include <unistd.h>

int setresuid(uid_t  ruid , uid_t  euid , uid_t  suid );
int setresgid(gid_t  rgid , gid_t  egid , gid_t  sgid );

/*
 7. All of the previously described system calls that change the process’s effective user
or group ID also always change the corresponding file-system ID. To change the
file-system IDs independently of the effective IDs, we must employ two Linux-specific
system calls: setfsuid() and setfsgid().
 */

#include <sys/fsuid.h>

int setfsuid(uid_t  fsuid );
int setfsgid(gid_t  fsgid );

/*
 8. The getgroups() system call returns the set of groups of which the calling process is
currently a member, in the array pointed to by grouplist.
 */

#include <unistd.h>

int getgroups(int gidsetsize , gid_t grouplist []);

/*
 9. A privileged process can use setgroups() and initgroups() to change its set of supplementary group IDs.
 */

#define _BSD_SOURCE
#include <grp.h>
int setgroups(size_t  gidsetsize , const gid_t * grouplist );
int initgroups(const char * user , gid_t  group );


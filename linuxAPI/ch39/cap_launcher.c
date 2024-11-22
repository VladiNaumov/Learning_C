/* Дополнительная программа к Главе 39 */

/* cap_launcher.c

   Запускает программу с учетными данными (UID, GID, дополнительные GID)
   указанного пользователя и с указанными в командной строке способностями.

   По умолчанию программа использует механизмы "ambient capabilities" 
   (доступно с Linux 4.3). Программа, которую запускает данный код, должна
   быть непривилегированной (не иметь set-UID, set-GID или способностей).

   Если используется флаг -A, то программа поднимает указанные способности
   только в наборе наследуемых (inheritable), а не в "ambient".
*/

#define _GNU_SOURCE /* Для использования функций prctl() и других расширений */
#include <sys/prctl.h>
#include <sys/capability.h>
#include <linux/securebits.h>
#include <pwd.h>
#include <grp.h>
#include "cap_functions.h" /* Объявление modifyCapSetting() */
#include "tlpi_hdr.h"

/* Выводит инструкцию по использованию программы */
static void usage(char *pname) {
    fprintf(stderr, "Использование: %s [-A] user cap,... cmd arg...\n", pname);
    fprintf(stderr, "\t'user' — имя пользователя для запуска программы\n");
    fprintf(stderr, "\t'cap,...' — набор способностей для программы\n");
    fprintf(stderr, "\t'cmd' и 'arg...' — команда и аргументы для запуска\n");
    fprintf(stderr, "\nОпции:\n");
    fprintf(stderr, "\t    -A  Установить способности только как наследуемые\n");
    exit(EXIT_FAILURE);
}

/* Устанавливает список дополнительных групп для пользователя */
static void setSupplementaryGroupList(char *user, gid_t gid) {
    int ngroups = 0;
    getgrouplist(user, gid, NULL, &ngroups);

    gid_t *groups = calloc(ngroups, sizeof(gid_t));
    if (groups == NULL)
        errExit("calloc");

    if (getgrouplist(user, gid, groups, &ngroups) == -1)
        errExit("getgrouplist");

    if (setgroups(ngroups, groups) == -1)
        errExit("setgroups");
}

/* Переключает учетные данные на указанные для пользователя */
static void setCredentials(char *user) {
    struct passwd *pwd = getpwnam(user);
    if (pwd == NULL) {
        fprintf(stderr, "Неизвестный пользователь: %s\n", user);
        exit(EXIT_FAILURE);
    }

    setSupplementaryGroupList(user, pwd->pw_gid);

    if (setresgid(pwd->pw_gid, pwd->pw_gid, pwd->pw_gid) == -1)
        errExit("setresgid");

    if (setresuid(pwd->pw_uid, pwd->pw_uid, pwd->pw_uid) == -1)
        errExit("setresuid");
}

/* Преобразует имя способности в идентификатор cap_value_t */
static cap_value_t capFromName(char *p) {
    cap_value_t cap;
    if (cap_from_name(p, &cap) == -1) {
        fprintf(stderr, "Нераспознанное имя способности: %s\n", p);
        exit(EXIT_FAILURE);
    }
    return cap;
}

/* Поднимает указанную способность в inheritable и (опционально) ambient */
static void raiseCap(cap_value_t cap, char *capName, bool raiseAmbient) {
    if (modifyCapSetting(CAP_INHERITABLE, cap, CAP_SET) == -1) {
        fprintf(stderr, "Не удалось поднять '%s' в наследуемых (%s)\n", capName, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (raiseAmbient) {
        if (prctl(PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0) == -1) {
            fprintf(stderr, "Не удалось поднять '%s' в ambient (%s)\n", capName, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

/* Поднимает набор способностей в inheritable и ambient */
static void raiseInheritableAndAmbientCaps(char *capList, bool raiseAmbient) {
    for (char *capName = capList; (capName = strtok(capName, ",")); capName = NULL) {
        cap_value_t cap = capFromName(capName);
        raiseCap(cap, capName, raiseAmbient);
    }
}

/* Главная функция программы */
int main(int argc, char *argv[]) {
    bool raiseAmbient = true;
    int opt;

    while ((opt = getopt(argc, argv, "A")) != -1) {
        switch (opt) {
            case 'A':
                raiseAmbient = false;
                break;
            default:
                fprintf(stderr, "Неверная опция\n");
                usage(argv[0]);
                break;
        }
    }
    if (argc < optind + 3)
        usage(argv[0]);

    if (geteuid() != 0)
        fatal("Должен выполняться от имени root");

    if (prctl(PR_SET_SECUREBITS, SECBIT_NO_SETUID_FIXUP, 0, 0, 0) == -1)
        errExit("prctl");

    setCredentials(argv[optind]);

    raiseInheritableAndAmbientCaps(argv[optind + 1], raiseAmbient);

    execvp(argv[optind + 2], &argv[optind + 2]);
    errExit("execvp");
}

/* 
Резюме программы:

Программа `cap_launcher.c` предназначена для запуска другой программы 
с учетными данными указанного пользователя и с заданными способностями 
(capabilities). Она позволяет управлять наследуемыми и "ambient" способностями, 
что полезно для безопасного выполнения задач от имени привилегированного 
пользователя. Программа требует выполнения от имени root и обеспечивает гибкое 
управление безопасностью за счет минимизации используемых привилегий.
*/

/* t_chown.c

   Демонстрирует использование системного вызова chown() для изменения владельца
   и группы файла.

   Использование: t_chown owner group [file...]

   Аргументы owner и group могут быть указаны как "-", чтобы оставить их без изменений.
*/

#include <pwd.h>
#include <grp.h>
#include "ugid_functions.h"             /* Декларации функций userIdFromName()
                                           и groupIdFromName() */
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    uid_t uid;
    gid_t gid;
    int j;
    Boolean errFnd;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s owner group [file...]\n"
                 "        owner или group могут быть '-', "
                 "что означает оставить без изменений\n", argv[0]);

    if (strcmp(argv[1], "-") == 0) {            /* "-" означает не менять владельца */
        uid = -1;
    } else {                                    /* Преобразуем имя пользователя в UID */
        uid = userIdFromName(argv[1]);
        if (uid == -1)
            fatal("Нет такого пользователя (%s)", argv[1]);
    }

    if (strcmp(argv[2], "-") == 0) {            /* "-" означает не менять группу */
        gid = -1;
    } else {                                    /* Преобразуем имя группы в GID */
        gid = groupIdFromName(argv[2]);
        if (gid == -1)
            fatal("Нет такой группы (%s)", argv[2]);
    }

    /* Изменяем владельца и группу для всех файлов, указанных в оставшихся аргументах */

    errFnd = FALSE;
    for (j = 3; j < argc; j++) {
        if (chown(argv[j], uid, gid) == -1) {
            errMsg("chown: %s", argv[j]);
            errFnd = TRUE;
        }
    }

    exit(errFnd ? EXIT_FAILURE : EXIT_SUCCESS);
}

/*
Резюме:
Эта программа использует системный вызов chown() для изменения владельца и группы файлов.
Пользователь указывает нового владельца и группу через аргументы командной строки. Если
вместо имени указано "-", соответствующее значение (владелец или группа) остается
без изменений. Программа применяет изменения к каждому файлу, переданному в аргументах.
В случае ошибки выводится сообщение, и программа завершает работу с кодом ошибки.
*/

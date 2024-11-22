/* t_setpriority.c

   Демонстрация использования setpriority(2) и getpriority(2) для изменения и 
   получения "nice"-значения процесса.

   Использование: t_setpriority {p|g|u} id priority

   Параметры:
     - p: изменить приоритет процесса (по его PID)
     - g: изменить приоритет группы процессов (по PGID)
     - u: изменить приоритет всех процессов пользователя (по UID)
*/

#include <sys/time.h>
#include <sys/resource.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int which, prio;
    id_t who;

    if (argc != 4 || strchr("pgu", argv[1][0]) == NULL)
        usageErr("%s {p|g|u} who priority\n"
                 "    set priority of: p=process; g=process group; "
                 "u=processes for user\n", argv[0]);

    /* Определение цели изменения приоритета */
    which = (argv[1][0] == 'p') ? PRIO_PROCESS :
            (argv[1][0] == 'g') ? PRIO_PGRP : PRIO_USER;
    who = getLong(argv[2], 0, "who");  /* ID процесса, группы или пользователя */
    prio = getInt(argv[3], 0, "prio"); /* Новое значение nice */

    /* Установка приоритета */
    if (setpriority(which, who, prio) == -1)
        errExit("setpriority");

    /* Проверка установленного значения nice */
    errno = 0;  /* Сбрасываем errno, т.к. успешный вызов может вернуть -1 */
    prio = getpriority(which, who);
    if (prio == -1 && errno != 0)
        errExit("getpriority");

    printf("Nice value = %d\n", prio);

    exit(EXIT_SUCCESS);
}

/*
 * Пример использования:
 *   1. Установить nice-значение для процесса с PID 1234 на 10:
 *        ./t_setpriority p 1234 10
 *   2. Установить nice-значение для группы процессов с PGID 5678 на -5:
 *        ./t_setpriority g 5678 -5
 *   3. Установить nice-значение для всех процессов пользователя с UID 1000 на 15:
 *        ./t_setpriority u 1000 15
 */

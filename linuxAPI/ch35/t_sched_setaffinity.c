/* t_sched_setaffinity.c

   Установка привязки процесса к определённым CPU.

   Использование: t_sched_setaffinity pid mask

   Пример: привязать процесс с указанным PID ко всем процессорам, кроме первого,
   на системе с четырьмя процессорами:
        t_sched_setaffinity <pid> 0xe
   (0xe = 1110 в двоичной системе)

   Это программа специфична для Linux. Системные вызовы управления привязкой CPU
   доступны начиная с ядра версии 2.6.
*/

#define _GNU_SOURCE
#include <sched.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pid mask\n", argv[0]);

    pid_t pid = getInt(argv[1], GN_NONNEG, "pid");          /* Получение PID */
    unsigned long mask = getLong(argv[2], GN_ANY_BASE, "mask"); /* Получение маски */

    cpu_set_t set;
    CPU_ZERO(&set);                                         /* Очистка набора CPU */

    for (int cpu = 0; mask > 0; cpu++, mask >>= 1) {        /* Установка привязки */
        if (mask & 1)                                       /* Если бит равен 1 */
            CPU_SET(cpu, &set);                             /* Добавить CPU в набор */
    }

    if (sched_setaffinity(pid, sizeof(set), &set) == -1)    /* Применение привязки */
        errExit("sched_setaffinity");

    exit(EXIT_SUCCESS);
}

/*
 * Пример использования:
 *   1. Определите PID процесса, например, с помощью команды `ps`.
 *   2. Задайте маску в шестнадцатеричной или десятичной системе.
 *   3. Запустите программу:
 *        ./t_sched_setaffinity <pid> <mask>
 *   4. Проверьте изменения с помощью t_sched_getaffinity или `taskset -p`.
 */

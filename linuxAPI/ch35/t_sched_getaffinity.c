/* t_sched_getaffinity.c

   Демонстрация использования системного вызова sched_getaffinity() для
   получения привязки процесса к конкретным CPU.

   Использование: t_sched_getaffinity pid

   Это программа, специфичная для Linux. Системные вызовы управления
   привязкой CPU появились с ядра версии 2.6.
*/

#define _GNU_SOURCE
#include <sched.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pid\n", argv[0]);

    pid_t pid = getInt(argv[1], GN_NONNEG, "pid");  /* Получение PID */

    cpu_set_t set;
    int s = sched_getaffinity(pid, sizeof(cpu_set_t), &set);
    if (s == -1)
        errExit("sched_getaffinity");

    printf("CPUs:");
    for (int cpu = 0; cpu < CPU_SETSIZE; cpu++) {  /* Перебор всех CPU */
        if (CPU_ISSET(cpu, &set))                 /* Проверка, включён ли CPU */
            printf(" %d", cpu);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}

/*
 * Пример использования:
 *   1. Определите PID процесса, например, с помощью команды `ps`.
 *   2. Запустите программу, указав PID:
 *        ./t_sched_getaffinity <pid>
 *   3. Программа выведет список процессоров, к которым привязан процесс.
 */

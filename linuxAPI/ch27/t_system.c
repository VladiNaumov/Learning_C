/* t_system.c

   Демонстрация использования system(3) для выполнения команды оболочки.
*/
#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

#define MAX_CMD_LEN 200

int
main(int argc, char *argv[])
{
    char str[MAX_CMD_LEN];      /* Команда, которая будет выполнена через system() */
    int status;                 /* Статус, возвращаемый функцией system() */

    for (;;) {                  /* Чтение и выполнение команды оболочки */
        printf("Command: ");
        fflush(stdout);
        if (fgets(str, MAX_CMD_LEN, stdin) == NULL)
            break;              /* конец файла */

        status = system(str);
        printf("system() вернул: статус=0x%04x (%d,%d)\n",
                (unsigned int) status, status >> 8, status & 0xff);

        if (status == -1) {
            errExit("system");
        } else {
            if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
                printf("(Вероятно) не удалось вызвать оболочку\n");
            else                /* Оболочка успешно выполнила команду */
                printWaitStatus(NULL, status);
        }
    }

    exit(EXIT_SUCCESS);
}
/*
### Резюме кода

Программа использует функцию `system()` для выполнения команд оболочки, введенных пользователем. 
В бесконечном цикле программа считывает команду из стандартного ввода, вызывает `system()` для выполнения команды и выводит статус завершения этой команды. 
Статус выводится в формате шестнадцатеричного числа, и если команда не смогла запуститься (статус 127), 
программа выводит сообщение о возможной ошибке при запуске оболочки. В случае успешного выполнения команды выводится информация о завершении процесса.
*/
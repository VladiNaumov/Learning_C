/* t_execl.c

   Демонстрация использования execl() для выполнения команды printenv(1).
*/
#include <stdlib.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    printf("Initial value of USER: %s\n", getenv("USER"));  /* Выводим начальное значение переменной USER */
    if (putenv("USER=britta") != 0)                         /* Изменяем переменную окружения USER */
        errExit("putenv");

    /* Выполняем команду printenv для отображения переменных окружения USER и SHELL */

    execl("/usr/bin/printenv", "printenv", "USER", "SHELL", (char *) NULL);
    errExit("execl");           /* Если мы оказались здесь, значит, execl() завершился с ошибкой */
}
/*
### Резюме кода

Этот код демонстрирует использование функции `execl()` для запуска программы `printenv`, которая выводит значения переменных окружения. 
Сначала переменной окружения `USER` присваивается значение `"britta"`. 
Затем вызывается `execl()` для выполнения команды `printenv`, передавая в качестве аргументов `USER` и `SHELL`. Если `execl()` 
завершается неудачно, программа выводит сообщение об ошибке и завершает работу.
*/
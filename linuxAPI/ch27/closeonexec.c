/* closeonexec.c

   Демонстрация получения и изменения флага close-on-exec для файлового дескриптора.
*/
#include <fcntl.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int flags;

    if (argc > 1) {
        flags = fcntl(STDOUT_FILENO, F_GETFD);          /* Получаем текущие флаги для STDOUT */
        if (flags == -1)
            errExit("fcntl - F_GETFD");

        flags |= FD_CLOEXEC;                            /* Включаем флаг FD_CLOEXEC */

        if (fcntl(STDOUT_FILENO, F_SETFD, flags) == -1) /* Устанавливаем обновлённые флаги для STDOUT */
            errExit("fcntl - F_SETFD");
    }

    /* Выполняем команду 'ls -l' для отображения информации о текущем файле */

    execlp("ls", "ls", "-l", argv[0], (char *) NULL);
    errExit("execlp");  /* Если мы оказались здесь, значит, execlp() завершился с ошибкой */
}
/*

### Резюме кода

Программа демонстрирует управление флагом `FD_CLOEXEC` (close-on-exec) для файлового дескриптора. 
Если программа запущена с аргументом, она устанавливает этот флаг для стандартного потока вывода (`STDOUT_FILENO`). 
Флаг `FD_CLOEXEC` указывает системе закрывать файловый дескриптор при вызове `exec()`. 
Затем программа вызывает `execlp()` для выполнения команды `ls -l`, чтобы показать информацию о текущем файле. Если `execlp()` 
не удается выполнить, программа выводит сообщение об ошибке и завершает работу.
*/
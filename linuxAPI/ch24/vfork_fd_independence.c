/* vfork_fd_independence.c

   Показать, что после вызова vfork() дочерний процесс может закрыть файловый
   дескриптор (например, дескриптор 0), и это не повлияет на соответствующий
   дескриптор родителя.
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    switch (vfork()) {
        case -1:
            errExit("vfork");

        case 0:  /* Дочерний процесс */
            printf("Child before closing stdin\n");
            if (close(STDIN_FILENO) == -1)
                errExit("close - child");
            printf("Child after closing stdin\n");

            _exit(EXIT_SUCCESS);

        default:  /* Родительский процесс */
            sleep(1);  /* Ждем завершения дочернего процесса */
            printf("Parent reading from stdin (should not be affected by child)\n");

            char buffer[128];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL)
                errExit("fgets - parent");
            printf("Parent read: %s\n", buffer);

            exit(EXIT_SUCCESS);
    }
}

/*
 Описание программы:
1. Родительский и дочерний процессы оба используют стандартный ввод (дескриптор 0) до того, как дочерний процесс закроет его.
2. Дочерний процесс закрывает дескриптор STDIN_FILENO (дескриптор 0), но это не влияет на родительский процесс.
3. После выполнения vfork() дочерний процесс продолжает выполнение и закрывает стандартный ввод.
3. Родительский процесс, даже после того как дочерний процесс закрыл дескриптор, продолжает работать с stdin и может читать данные с клавиатуры.
*/
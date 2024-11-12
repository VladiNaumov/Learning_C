
/* t_fpathconf.c

   Демонстрация использования функции fpathconf() для получения значений
   ограничений, связанных с именем пути.
*/

#include "tlpi_hdr.h"

/* 
 * Выводит сообщение 'msg' и значение, возвращаемое fpathconf(fd, name).
 * Если значение определено, выводит его; если не определено, выводит "indeterminate".
 * В случае ошибки завершает работу программы.
 */
static void fpathconfPrint(const char *msg, int fd, int name)
{
    long lim;

    errno = 0;                  // Сбрасываем errno перед вызовом
    lim = fpathconf(fd, name);   // Получаем значение ограничения

    if (lim != -1) {             // Вызов успешен, ограничение определено
        printf("%s %ld\n", msg, lim);
    } else {
        if (errno == 0)          // Вызов успешен, но ограничение не определено
            printf("%s (indeterminate)\n", msg);
        else                     // Вызов завершился ошибкой
            errExit("fpathconf %s", msg);
    }
}

int main(int argc, char *argv[])
{
    // Выводим ограничения для стандартного ввода (STDIN)
    fpathconfPrint("_PC_NAME_MAX: ", STDIN_FILENO, _PC_NAME_MAX);   // Макс. длина имени файла
    fpathconfPrint("_PC_PATH_MAX: ", STDIN_FILENO, _PC_PATH_MAX);   // Макс. длина пути
    fpathconfPrint("_PC_PIPE_BUF: ", STDIN_FILENO, _PC_PIPE_BUF);   // Макс. буфер канала

    exit(EXIT_SUCCESS);
}

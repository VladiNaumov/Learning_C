/* t_sysconf.c

   Демонстрация использования функции sysconf() для получения значений системных ограничений.
*/

#include "tlpi_hdr.h"

/* 
 * Выводит сообщение 'msg' и значение, возвращаемое sysconf(name).
 * Если значение определено, выводит его; если не определено, выводит "indeterminate".
 * В случае ошибки завершает работу программы.
 */
static void sysconfPrint(const char *msg, int name)
{
    long lim;

    errno = 0;                // Сбрасываем errno перед вызовом
    lim = sysconf(name);      // Получаем значение системного ограничения

    if (lim != -1) {          // Вызов успешен, ограничение определено
        printf("%s %ld\n", msg, lim);
    } else {
        if (errno == 0)       // Вызов успешен, но ограничение не определено
            printf("%s (indeterminate)\n", msg);
        else                  // Вызов завершился ошибкой
            errExit("sysconf %s", msg);
    }
}

int main(int argc, char *argv[])
{
    // Выводим значения различных системных ограничений
    sysconfPrint("_SC_ARG_MAX:        ", _SC_ARG_MAX);          // Максимальный размер аргументов
    sysconfPrint("_SC_LOGIN_NAME_MAX: ", _SC_LOGIN_NAME_MAX);   // Максимальная длина имени пользователя
    sysconfPrint("_SC_OPEN_MAX:       ", _SC_OPEN_MAX);         // Максимальное количество открытых файлов
    sysconfPrint("_SC_NGROUPS_MAX:    ", _SC_NGROUPS_MAX);      // Максимальное количество групп
    sysconfPrint("_SC_PAGESIZE:       ", _SC_PAGESIZE);         // Размер страницы памяти
    sysconfPrint("_SC_RTSIG_MAX:      ", _SC_RTSIG_MAX);        // Максимальное количество RT-сигналов

    exit(EXIT_SUCCESS);
}

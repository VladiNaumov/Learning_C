/* print_rlimit.c

   Вывод значений мягкого (soft) и жесткого (hard) ограничения для указанного ресурса.
*/

#include <sys/resource.h>
#include "print_rlimit.h"       /* Объявление функции printRlimit */
#include "tlpi_hdr.h"

/*
 * printRlimit:
 * Выводит сообщение 'msg', за которым следуют текущие значения 
 * мягкого (soft) и жесткого (hard) ограничений для ресурса 'resource'.
 * 
 * Параметры:
 * - msg: сообщение, которое будет напечатано перед ограничениями.
 * - resource: идентификатор ограничиваемого ресурса.
 * 
 * Возвращаемое значение:
 * - 0 при успешной обработке;
 * - -1 в случае ошибки (например, если getrlimit() не удался).
 */
int printRlimit(const char *msg, int resource)
{
    struct rlimit rlim;

    /* Получаем текущие значения ограничения ресурса */
    if (getrlimit(resource, &rlim) == -1)
        return -1; /* Ошибка при получении значений ограничения */

    /* Печать сообщения и мягкого ограничения */
    printf("%s soft=", msg);
    if (rlim.rlim_cur == RLIM_INFINITY)
        printf("infinite"); /* Мягкое ограничение равно бесконечности */
#ifdef RLIM_SAVED_CUR           /* Проверяем наличие RLIM_SAVED_CUR */
    else if (rlim.rlim_cur == RLIM_SAVED_CUR)
        printf("unrepresentable"); /* Значение невозможно представить */
#endif
    else
        printf("%lld", (long long) rlim.rlim_cur); /* Текущее мягкое ограничение */

    /* Печать жесткого ограничения */
    printf("; hard=");
    if (rlim.rlim_max == RLIM_INFINITY)
        printf("infinite\n"); /* Жесткое ограничение равно бесконечности */
#ifdef RLIM_SAVED_MAX           /* Проверяем наличие RLIM_SAVED_MAX */
    else if (rlim.rlim_max == RLIM_SAVED_MAX)
        printf("unrepresentable"); /* Значение невозможно представить */
#endif
    else
        printf("%lld\n", (long long) rlim.rlim_max); /* Текущее жесткое ограничение */

    return 0; /* Успешное выполнение */
}

/*
Резюме:
Функция printRlimit используется для вывода текущих значений ресурсных ограничений:
мягкого (rlim_cur) и жесткого (rlim_max). Она поддерживает специальное значение RLIM_INFINITY
(бесконечность) и, если доступно, обрабатывает RLIM_SAVED_CUR/RLIM_SAVED_MAX (нерепрезентативные значения).
Основной вызов — getrlimit(), который извлекает текущие лимиты для указанного ресурса.
*/

/* itimerspec_from_str.c

   Реализация функции itimerspecFromStr().
*/
#ifndef __APPLE__       /* Mac OS X не определяет структуру 'itimerspec'
                           (или POSIX функции таймера (timer_*())) */
#include <string.h>
#include <stdlib.h>
#include "itimerspec_from_str.h"        /* Объявление функции, определенной здесь */

/* Преобразует строку следующего формата в структуру itimerspec:
   "value.sec[/value.nanosec][:interval.sec[/interval.nanosec]]".
   Необязательные компоненты, которые отсутствуют, приводят к присвоению
   значения 0 соответствующим полям структуры. */

void itimerspecFromStr(char *str, struct itimerspec *tsp) {
    char *dupstr, *cptr, *sptr;

    dupstr = strdup(str);  /* Дублируем строку, чтобы можно было модифицировать */

    /* Ищем разделитель ':' для указания интервала таймера */
    cptr = strchr(dupstr, ':');
    if (cptr != NULL)
        *cptr = '\0';

    /* Ищем разделитель '/' для указания наносекунд */
    sptr = strchr(dupstr, '/');
    if (sptr != NULL)
        *sptr = '\0';

    /* Устанавливаем начальное время таймера */
    tsp->it_value.tv_sec = atoi(dupstr);
    tsp->it_value.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;

    /* Устанавливаем интервал таймера, если он задан */
    if (cptr == NULL) {
        tsp->it_interval.tv_sec = 0;
        tsp->it_interval.tv_nsec = 0;
    } else {
        sptr = strchr(cptr + 1, '/');
        if (sptr != NULL)
            *sptr = '\0';
        tsp->it_interval.tv_sec = atoi(cptr + 1);
        tsp->it_interval.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;
    }
    free(dupstr);  /* Освобождаем дублированную строку */
}
#endif

/*

### Объяснение:
- **Функция** `itimerspecFromStr()`: Преобразует строку вида `"value.sec[/value.nanosec][:interval.sec[/interval.nanosec]]"` в структуру `itimerspec`.
- **Разделение строки**:
  - **Начальное время (`it_value`)**: Устанавливается из первой части строки.
  - **Интервал (`it_interval`)**: Если в строке есть `':'`, значит интервал присутствует и устанавливается после символа `':'`.
  - В обоих случаях, если поле наносекунд отсутствует, оно принимается равным `0`.
- **Освобождение памяти**: После использования `strdup`, выделенная память освобождается с помощью `free`. 
*/


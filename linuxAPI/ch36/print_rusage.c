/* print_rusage.c

   Вывод содержимого структуры 'rusage' (использование ресурсов),
   возвращаемой вызовом getrusage().
*/

#include <sys/resource.h>
#include "print_rusage.h" /* Заголовочный файл с декларацией функции */
#include "tlpi_hdr.h"

/*
 * printRusage:
 * Форматированный вывод статистики использования ресурсов,
 * предоставленных структурой rusage.
 *
 * Параметры:
 * - leader: строка, добавляемая перед каждой строкой вывода (может быть NULL).
 * - ru: указатель на структуру rusage, содержащую данные о ресурсах.
 */
void printRusage(const char *leader, const struct rusage *ru)
{
    const char *ldr = (leader == NULL) ? "" : leader; /* Используем пустую строку, если leader == NULL */

    printf("%sCPU time (secs):         user=%.3f; system=%.3f\n", ldr,
           ru->ru_utime.tv_sec + ru->ru_utime.tv_usec / 1000000.0, /* Время, затраченное пользователем */
           ru->ru_stime.tv_sec + ru->ru_stime.tv_usec / 1000000.0); /* Время, затраченное системой */

    printf("%sMax resident set size:   %ld\n", ldr, ru->ru_maxrss); /* Максимальный размер резидентного набора */
    printf("%sIntegral shared memory:  %ld\n", ldr, ru->ru_ixrss); /* Общее использование разделяемой памяти */
    printf("%sIntegral unshared data:  %ld\n", ldr, ru->ru_idrss); /* Неразделяемые данные */
    printf("%sIntegral unshared stack: %ld\n", ldr, ru->ru_isrss); /* Неразделяемый стек */

    printf("%sPage reclaims:           %ld\n", ldr, ru->ru_minflt); /* Возвраты страниц */
    printf("%sPage faults:             %ld\n", ldr, ru->ru_majflt); /* Ошибки страниц */
    printf("%sSwaps:                   %ld\n", ldr, ru->ru_nswap); /* Количество свопов */

    printf("%sBlock I/Os:              input=%ld; output=%ld\n",
           ldr, ru->ru_inblock, ru->ru_oublock); /* Вход/выход блоков */

    printf("%sSignals received:        %ld\n", ldr, ru->ru_nsignals); /* Количество полученных сигналов */

    printf("%sIPC messages:            sent=%ld; received=%ld\n",
           ldr, ru->ru_msgsnd, ru->ru_msgrcv); /* Сообщения IPC: отправленные/полученные */

    printf("%sContext switches:        voluntary=%ld; "
           "involuntary=%ld\n", ldr, ru->ru_nvcsw, ru->ru_nivcsw); /* Переключения контекста: добровольные/принудительные */
}


/*

### Резюме:

**Описание:**
Функция `printRusage` выводит в формате таблицы данные о системных ресурсах, использованных процессом или его дочерними процессами. Информация извлекается из структуры `rusage`, возвращаемой вызовом `getrusage()`.

---

**Функциональность:**
1. Форматированный вывод ключевых метрик:
   - Время процессора, затраченное пользователем и системой.
   - Использование памяти (размер резидентного набора, разделяемая/нераспределяемая память, стек).
   - Статистика страниц (возвраты, ошибки).
   - Ввод/вывод блоков данных.
   - Количество сигналов и сообщений межпроцессного взаимодействия (IPC).
   - Переключения контекста (добровольные и принудительные).

2. Возможность добавления строкового префикса `leader` перед каждой строкой вывода, что полезно для группировки данных.

---

**Применение:**
Используется для мониторинга использования ресурсов процессом. Это полезно для анализа производительности, профилирования и отладки.

*/
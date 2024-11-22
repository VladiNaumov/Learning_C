/* cap_functions.h

   Полезные функции для работы с привилегиями.
*/

#ifndef CAP_FUNCTIONS_H             /* Защита от двойного включения */
#define CAP_FUNCTIONS_H

#include <stdio.h>
#include <sys/capability.h>
#include <stdbool.h>

/**
 * Изменяет настройку привилегии в указанном наборе.
 *
 * @param flag Указывает набор привилегий: CAP_PERMITTED, CAP_EFFECTIVE или CAP_INHERITABLE.
 * @param capability Конкретная привилегия для изменения.
 * @param setting Указывает действие: CAP_SET или CAP_CLEAR.
 * @return 0 при успешном завершении, -1 при ошибке.
 */
int modifyCapSetting(cap_flag_t flag, int capability, int setting);

/**
 * Выводит маску securebits в краткой или полной форме.
 *
 * @param secbits Маска securebits для отображения.
 * @param verbose Если true, выводит полное описание.
 * @param fp Поток вывода для результатов.
 */
void printSecbits(int secbits, bool verbose, FILE *fp);

#endif

/*
### Обзор
1. **`modifyCapSetting`**:
   - Изменяет настройки конкретной привилегии в наборе `CAP_PERMITTED`, `CAP_EFFECTIVE`, или `CAP_INHERITABLE`.
   - Удобна для работы с привилегиями процесса.

2. **`printSecbits`**:
   - Выводит текущее состояние securebits.
   - Полезно для диагностики или документирования текущих настроек безопасности.

### Применение
Этот заголовочный файл обеспечивает декларации функций для управления и отображения привилегий процесса, делая код проекта структурированным и переиспользуемым.
*/
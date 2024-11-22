/* cap_functions.c

   Полезные функции для работы с привилегиями.
*/

#include <sys/prctl.h>
#include <linux/securebits.h>
#include "cap_functions.h"

/**
 * Изменяет настройку привилегии в указанном наборе (flag).
 *
 * Параметры:
 * - flag: CAP_PERMITTED, CAP_EFFECTIVE или CAP_INHERITABLE.
 * - capability: Изменяемая привилегия.
 * - setting: CAP_SET или CAP_CLEAR.
 *
 * Возвращает:
 * - 0 в случае успеха, -1 в случае ошибки.
 */
int modifyCapSetting(cap_flag_t flag, cap_value_t capability, int setting) {
    cap_t caps = cap_get_proc();  // Получение текущих привилегий процесса.
    if (caps == NULL)
        return -1;

    cap_value_t capList[1] = {capability};
    if (cap_set_flag(caps, flag, 1, capList, setting) == -1) {
        cap_free(caps);
        return -1;
    }

    // Применение изменённых привилегий.
    if (cap_set_proc(caps) == -1) {
        cap_free(caps);
        return -1;
    }

    // Освобождение памяти.
    if (cap_free(caps) == -1)
        return -1;

    return 0;
}

/**
 * Отображает маску securebits в краткой или подробной форме.
 *
 * Параметры:
 * - secbits: Маска securebits.
 * - verbose: Если true, используется подробный формат.
 * - fp: Указатель на поток вывода.
 */
void printSecbits(int secbits, bool verbose, FILE *fp) {
    struct secbitInfo {
        int   flag;
        char *name;
        char  letter;
    };

    struct secbitInfo secbitInfoList[] = {
        {SECBIT_NO_CAP_AMBIENT_RAISE,   "NO_CAP_AMBIENT_RAISE",   'a'},
        {SECBIT_NO_CAP_AMBIENT_RAISE_LOCKED, "NO_CAP_AMBIENT_RAISE_LOCKED", 'A'},
        {SECBIT_KEEP_CAPS,              "KEEP_CAPS",              'k'},
        {SECBIT_KEEP_CAPS_LOCKED,       "KEEP_CAPS_LOCKED",       'K'},
        {SECBIT_NOROOT,                 "NOROOT",                 'r'},
        {SECBIT_NOROOT_LOCKED,          "NOROOT_LOCKED",          'R'},
        {SECBIT_NO_SETUID_FIXUP,        "NO_SETUID_FIXUP",        's'},
        {SECBIT_NO_SETUID_FIXUP_LOCKED, "NO_SETUID_FIXUP_LOCKED", 'S'},
        {0, NULL, '\0'}
    };

    int printed = 0;

    if (verbose) {
        fprintf(fp, "[");
        for (struct secbitInfo *p = secbitInfoList; p->flag != 0; p++) {
            if (secbits & p->flag) {
                if (printed > 0)
                    fprintf(fp, ", ");
                fprintf(fp, "%s", p->name);
                printed++;
            }
        }
        fprintf(fp, "]");
    } else {
        for (struct secbitInfo *p = secbitInfoList; p->flag != 0; p++) {
            fputc((secbits & p->flag) ? p->letter : '-', fp);
        }
    }
}

/*
### Обзор
1. **`modifyCapSetting`**:  
   Функция для изменения привилегий процесса. Позволяет изменять флаги `CAP_PERMITTED`, `CAP_EFFECTIVE` или `CAP_INHERITABLE`.  
   Полезна для управления правами на уровне процессов.

2. **`printSecbits`**:  
   Выводит маску `securebits` в кратком (`-a-k`) или подробном (`[NO_CAP_AMBIENT_RAISE, KEEP_CAPS]`) формате.  
   Используется для диагностики и отладки настроек привилегий.

### Применение
Эти функции упрощают управление привилегиями и анализ текущих настроек безопасности процесса.
*/
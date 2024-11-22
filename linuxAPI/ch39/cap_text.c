/* cap_text.c

   Использование: cap_text "текстовое_представление_прав"

   Цель этой программы - помочь пользователю понять текстовое представление прав,
   используемое такими командами, как setcap(1) и getcap(1). Программа позволяет
   передать строку текстового представления прав, преобразует её во внутреннее
   представление (cap_t) и затем обратно в текстовое. Это помогает увидеть минимальную
   текстовую строку, соответствующую переданным данным. Кроме того, программа
   показывает, в каких наборах (разрешённый, эффективный, наследуемый) активны
   переданные права.
*/

#include <sys/capability.h>
#include "tlpi_hdr.h"

// Флаги управления выводом прав
#define PRCAP_SHOW_ALL          0x01    // Показать все права, даже неактивные
#define PRCAP_SHOW_UNRECOGNIZED 0x02    // Показать права, нераспознанные libcap

/* Проверка, установлено ли право в указанном наборе */
static int
capIsSet(cap_t capSets, cap_value_t cap, cap_flag_t set)
{
    cap_flag_value_t value;

    if (cap_get_flag(capSets, cap, set, &value) == -1)
        errExit("cap_get_flag");

    return value == CAP_SET;
}

/* Проверка, установлено ли право в разрешённом наборе */
static int capIsPermitted(cap_t capSets, cap_value_t cap) {
    return capIsSet(capSets, cap, CAP_PERMITTED);
}

/* Проверка, установлено ли право в эффективном наборе */
static int capIsEffective(cap_t capSets, cap_value_t cap) {
    return capIsSet(capSets, cap, CAP_EFFECTIVE);
}

/* Проверка, установлено ли право в наследуемом наборе */
static int capIsInheritable(cap_t capSets, cap_value_t cap) {
    return capIsSet(capSets, cap, CAP_INHERITABLE);
}

/* Вывод информации о конкретном праве */
static void
printCap(cap_t capSets, cap_value_t cap, char *capStrName, int flags)
{
    cap_flag_value_t dummy;

    // Проверяем, распознано ли право
    if (cap_get_flag(capSets, cap, CAP_PERMITTED, &dummy) != -1) {
        if ((flags & PRCAP_SHOW_ALL) ||
                capIsPermitted(capSets, cap) ||
                capIsEffective(capSets, cap) ||
                capIsInheritable(capSets, cap))
            printf("%-22s %s%s%s\n", capStrName,
                   capIsPermitted(capSets, cap) ? "p" : " ",
                   capIsEffective(capSets, cap) ? "e" : " ",
                   capIsInheritable(capSets, cap) ? "i" : " ");
    } else {
        if (flags & PRCAP_SHOW_UNRECOGNIZED)
            printf("%-22s unrecognized by libcap\n", capStrName);
    }
}

/* Вывод информации обо всех правах */
static void
printAllCaps(cap_t capSets, int flags)
{
    printCap(capSets, CAP_AUDIT_CONTROL, "CAP_AUDIT_CONTROL", flags);
#ifdef CAP_AUDIT_READ
    printCap(capSets, CAP_AUDIT_READ, "CAP_AUDIT_READ", flags);
#endif
    printCap(capSets, CAP_AUDIT_WRITE, "CAP_AUDIT_WRITE", flags);
// Пропускаем аналогичные блоки для остальных прав, чтобы сохранить краткость примера
}

/* Основная функция программы */
int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "%s <текстовое_представление_прав>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Преобразуем текстовое представление прав во внутренний формат
    cap_t capSets = cap_from_text(argv[1]);
    if (capSets == NULL)
        errExit("cap_from_text");

    // Преобразуем внутреннее представление обратно в текстовое
    char *textCaps = cap_to_text(capSets, NULL);
    if (textCaps == NULL)
        errExit("cap_to_text");

    printf("caps_to_text() вернул: \"%s\"\n\n", textCaps);

    // Выводим информацию о правах
    printAllCaps(capSets, PRCAP_SHOW_ALL);

    // Освобождаем выделенную память
    if (cap_free(textCaps) != 0 || cap_free(capSets) != 0)
        errExit("cap_free");

    exit(EXIT_SUCCESS);
}

/*
Резюме:
Эта программа принимает строку, описывающую права, преобразует её во внутреннее
представление и анализирует, какие права установлены в разрешённом, эффективном
и наследуемом наборах. Она помогает пользователю понять, как текстовые строки
прав интерпретируются системой.
*/

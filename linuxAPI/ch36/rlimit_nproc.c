/* rlimit_nproc.c

   Эксперимент с лимитом на максимальное количество процессов.

   Использование: rlimit_nproc hard-limit soft-limit

   ПРИМЕЧАНИЕ: Только Linux и BSD поддерживают лимит ресурса RLIMIT_NPROC.
*/

#include <sys/resource.h>      // Для работы с лимитами ресурсов
#include "print_rlimit.h"      // Декларация функции printRlimit()
#include "tlpi_hdr.h"          // Заголовочный файл с вспомогательными функциями и макросами

int
main(int argc, char *argv[])
{
    struct rlimit rl;          // Структура для хранения лимитов
    int j;                     // Счетчик числа созданных процессов
    pid_t childPid;            // PID создаваемого процесса

    if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s soft-limit [hard-limit]\n", argv[0]);

    printRlimit("Initial maximum process limits: ", RLIMIT_NPROC);

    /* Устанавливаем новые лимиты процессов (если не указан hard-limit, используем soft-limit) */

    rl.rlim_cur = (argv[1][0] == 'i') ? RLIM_INFINITY :
                                getInt(argv[1], 0, "soft-limit");   // Новый мягкий лимит
    rl.rlim_max = (argc == 2) ? rl.rlim_cur :
                (argv[2][0] == 'i') ? RLIM_INFINITY :
                                getInt(argv[2], 0, "hard-limit");  // Новый жесткий лимит
    if (setrlimit(RLIMIT_NPROC, &rl) == -1)
        errExit("setrlimit");

    printRlimit("New maximum process limits:     ", RLIMIT_NPROC);

    /* Создаем как можно больше процессов */

    for (j = 1; ; j++) {
        switch (childPid = fork()) {
        case -1: errExit("fork"); // Ошибка при создании нового процесса

        case 0:                  // Дочерний процесс
            _exit(EXIT_SUCCESS);

        default:                 // Родительский процесс: сообщение о новом процессе
            printf("Child %d (PID=%ld) started\n", j, (long) childPid);
            break;
        }
    }
}
/*

### Резюме:

**Описание:**
Этот код демонстрирует работу с ограничением на максимальное количество процессов, которые может создать пользователь. Он устанавливает новый предел и создает дочерние процессы до тех пор, пока не будет достигнут лимит.

---

**Функциональность:**
1. Отображение начального значения лимита с помощью `printRlimit()`.
2. Изменение мягкого (`soft`) и жесткого (`hard`) лимитов через `setrlimit()`.
3. Попытка создания дочерних процессов в бесконечном цикле. При превышении лимита вызов `fork()` завершится с ошибкой.

---

**Применение:**
- Используется для тестирования и понимания поведения системы при ограничении числа процессов.
- Полезно для изучения работы с системными ресурсами и их лимитами.

*/
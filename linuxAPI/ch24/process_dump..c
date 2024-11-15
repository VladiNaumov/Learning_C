/*
 *
Как получить дамп памяти процесса, позволяя исходному процессу продолжить выполнение:
Для создания дампа памяти процесса и продолжения его выполнения, можно использовать механизм ptrace,
который позволяет родительскому процессу управлять действиями дочернего процесса (например, остановить его и создать дамп памяти).

Примерный код для создания дампа памяти процесса с использованием ptrace:
 */

#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

void createMemoryDump(pid_t childPid)
{
    char dumpFile[] = "/tmp/process_dump.txt";
    FILE *dump = fopen(dumpFile, "w");
    if (dump == NULL)
        errExit("fopen");

    // Останавливаем дочерний процесс, чтобы создать дамп
    if (ptrace(PTRACE_ATTACH, childPid, NULL, NULL) == -1)
        errExit("ptrace - attach");

    // Ожидаем остановки дочернего процесса
    waitpid(childPid, NULL, 0);

    // Получаем дамп памяти дочернего процесса
    long data;
    for (long addr = 0x400000; addr < 0x500000; addr += sizeof(long)) {
        data = ptrace(PTRACE_PEEKDATA, childPid, (void *)addr, NULL);
        if (data == -1)
            break;  // Прекращаем, если достигнут конец памяти
        fprintf(dump, "Memory at 0x%lx: 0x%lx\n", addr, data);
    }

    fclose(dump);
    printf("Memory dump written to /tmp/process_dump.txt\n");

    // Отсоединяем процесс и разрешаем ему продолжить выполнение
    ptrace(PTRACE_DETACH, childPid, NULL, NULL);
}

int main(int argc, char *argv[])
{
    pid_t childPid;

    switch (childPid = fork()) {
        case -1:
            errExit("fork");

        case 0:  /* Дочерний процесс */
            printf("Child process running\n");
            while (1) {}  /* Бесконечный цикл, чтобы процесс продолжал работать */

        default:  /* Родительский процесс */
            sleep(2);  /* Даем время дочернему процессу запуститься */
            createMemoryDump(childPid);  /* Получаем дамп памяти дочернего процесса */
            waitpid(childPid, NULL, 0);  /* Ждем завершения дочернего процесса */
            exit(EXIT_SUCCESS);
    }
}

/*
### Описание:
1. Программа создает дочерний процесс, который работает в бесконечном цикле.
2. Родительский процесс использует `ptrace` для присоединения к дочернему процессу, чтобы сделать дамп памяти.
3. Дамп памяти сохраняется в файл `/tmp/process_dump.txt`.
4. После создания дампа, родительский процесс отсоединяется от дочернего процесса с помощью `ptrace(PTRACE_DETACH)` и позволяет дочернему процессу продолжить выполнение.

### Важное замечание:
Для использования `ptrace` программа должна иметь соответствующие права (например, быть запущена с правами суперпользователя, если это требуется для доступа к памяти другого процесса).
*/
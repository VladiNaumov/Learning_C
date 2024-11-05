/* Эта программа выводит дерево процессов, начиная с родительского процесса `init`, демонстрируя иерархию родительско-дочерних отношений для всех процессов в системе. 
Она использует файлы `/proc/[PID]/status`, чтобы получить информацию о каждом процессе, включая его идентификатор и команду, а также `PPid`, 
который указывает на идентификатор родительского процесса.
*/


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define MAX_LINE 256
#define MAX_PROCESSES 32768 // Максимальное количество процессов для большинства систем

// Структура для хранения информации о процессе
typedef struct Process {
    int pid;            // Идентификатор процесса
    int ppid;           // Идентификатор родительского процесса
    char name[MAX_LINE]; // Имя процесса
} Process;

Process processes[MAX_PROCESSES]; // Массив процессов
int process_count = 0;            // Счетчик процессов

// Функция для чтения информации о процессе из /proc/[PID]/status
void getProcessInfo(const char *pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/status", pid);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        if (errno != ENOENT) {
            perror("fopen");
        }
        return;
    }

    int proc_pid = atoi(pid);
    int proc_ppid = -1;
    char proc_name[MAX_LINE] = {0};
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "Name:", 5) == 0) {
            sscanf(line, "Name:\t%255s", proc_name);
        } else if (strncmp(line, "Pid:", 4) == 0) {
            sscanf(line, "Pid:\t%d", &proc_pid);
        } else if (strncmp(line, "PPid:", 5) == 0) {
            sscanf(line, "PPid:\t%d", &proc_ppid);
        }
    }
    fclose(file);

    processes[process_count].pid = proc_pid;
    processes[process_count].ppid = proc_ppid;
    strncpy(processes[process_count].name, proc_name, MAX_LINE);
    process_count++;
}

// Рекурсивная функция для вывода дерева процессов
void printProcessTree(int ppid, int level) {
    for (int i = 0; i < process_count; i++) {
        if (processes[i].ppid == ppid) {
            // Отступ для отображения уровня
            for (int j = 0; j < level; j++) {
                printf("  ");
            }
            printf("%d %s\n", processes[i].pid, processes[i].name);
            printProcessTree(processes[i].pid, level + 1);
        }
    }
}

int main() {
    DIR *procDir = opendir("/proc");
    if (procDir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(procDir)) != NULL) {
        // Проверяем, что имя каталога является числом (PID процесса)
        if (entry->d_type == DT_DIR && atoi(entry->d_name) > 0) {
            getProcessInfo(entry->d_name);
        }
    }
    closedir(procDir);

    // Вывод дерева процессов, начиная с init (PID 1)
    printf("Process Tree:\n");
    printProcessTree(1, 0);

    return EXIT_SUCCESS;
}

/*

### Пояснение работы программы

1. **Сканирование каталогов `/proc`**:
   - Программа открывает каталог `/proc` и перебирает все подкаталоги. Если имя подкаталога является числом (PID процесса), программа вызывает `getProcessInfo()` для получения информации о процессе.

2. **Функция `getProcessInfo()`**:
   - Открывает файл `/proc/[PID]/status` для каждого процесса, чтобы получить его `Name`, `Pid`, и `PPid`.
   - Считывает строки, которые содержат информацию об имени процесса (`Name`), его PID (`Pid`) и идентификаторе родительского процесса (`PPid`).
   - Сохраняет информацию в массив `processes`.

3. **Вывод дерева процессов**:
   - Функция `printProcessTree()` выполняет рекурсивный обход массива `processes`, выводя дерево процессов с отступами для каждого уровня.
   - Начинает с `PID` родительского процесса `init` (обычно `PID 1`) и рекурсивно выводит дочерние процессы.

### Пример использования

Скомпилируйте и запустите программу:

```bash
gcc -o process_tree process_tree.c
./process_tree
```

### Вывод

Программа выведет дерево процессов, начиная с `init`, и покажет иерархию процессов в системе.

*/
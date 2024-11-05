/* Эта программа выводит список всех процессов, у которых имеется открытый файл с указанным путём. 
Для этого она просматривает символьные ссылки в каталогах `/proc/[PID]/fd/`, чтобы найти нужный файл.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#define MAX_PATH 1024

// Функция для проверки, открыт ли файл с указанным путём в процессе
void checkProcessOpenFile(const char *pid, const char *targetPath) {
    char fdDir[MAX_PATH];
    snprintf(fdDir, sizeof(fdDir), "/proc/%s/fd", pid);

    DIR *dir = opendir(fdDir);
    if (dir == NULL) {
        // Игнорируем ошибку, если директория процесса уже не существует
        if (errno != ENOENT) {
            perror("opendir");
        }
        return;
    }

    struct dirent *entry;
    char linkPath[MAX_PATH];
    char resolvedPath[MAX_PATH];

    // Проходим по всем символьным ссылкам в каталоге /proc/[PID]/fd
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_LNK) {
            snprintf(linkPath, sizeof(linkPath), "%s/%s", fdDir, entry->d_name);

            ssize_t len = readlink(linkPath, resolvedPath, sizeof(resolvedPath) - 1);
            if (len != -1) {
                resolvedPath[len] = '\0';

                // Проверяем, соответствует ли путь целевому пути
                if (strcmp(resolvedPath, targetPath) == 0) {
                    printf("Process ID: %s has the file open\n", pid);
                    break;
                }
            } else {
                perror("readlink");
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file-path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *targetPath = argv[1];

    // Открываем каталог /proc
    DIR *procDir = opendir("/proc");
    if (procDir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(procDir)) != NULL) {
        // Проверяем, что имя каталога является числом (PID процесса)
        if (entry->d_type == DT_DIR && atoi(entry->d_name) > 0) {
            checkProcessOpenFile(entry->d_name, targetPath);
        }
    }

    closedir(procDir);
    return EXIT_SUCCESS;
}

/*

### Пояснение работы программы

1. **Проверка аргумента командной строки**:
   - Программа принимает путь к файлу как аргумент командной строки, который будет использоваться для поиска открытых файлов в процессах.

2. **Сканирование каталогов `/proc`**:
   - Программа открывает каталог `/proc` и проверяет каждый подкаталог, чтобы определить, является ли он числом (PID процесса).
   - Для каждого найденного PID программа вызывает `checkProcessOpenFile()`.

3. **Функция `checkProcessOpenFile()`**:
   - Создаёт путь `/proc/[PID]/fd` для каждого процесса и открывает этот каталог.
   - Перебирает символьные ссылки в каталоге `/proc/[PID]/fd`, которые представляют собой открытые файлы процесса.
   - Использует `readlink()` для чтения пути, на который указывает каждая ссылка. Если этот путь совпадает с указанным пользователем путём, программа выводит PID процесса, который открыл данный файл.

4. **Вывод информации**:
   - Если процесс имеет указанный файл в открытых, программа выводит идентификатор этого процесса.

### Пример использования

Скомпилируйте и запустите программу:

```bash
gcc -o find_opened_process find_opened_process.c
./find_opened_process /path/to/file
```

Программа выведет список процессов, у которых открыт файл с указанным путём.
*/
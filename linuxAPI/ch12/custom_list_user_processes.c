/*
Эта программа читает файлы `/proc/PID/status` для каждого процесса, чтобы получить `UID` владельца и имя команды. 
Она выводит список идентификаторов процессов и имен команд для всех процессов, запущенных пользователем, указанным в аргументе командной строки. 
Мы будем использовать функцию `userIdFromName()` для преобразования имени пользователя в его UID.
*/


#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <errno.h>

#define MAX_LINE 256

// Функция для получения UID по имени пользователя
uid_t userIdFromName(const char *name) {
    struct passwd *pwd = getpwnam(name);
    if (pwd == NULL) {
        fprintf(stderr, "No such user: %s\n", name);
        exit(EXIT_FAILURE);
    }
    return pwd->pw_uid;
}

// Функция для чтения UID и имени процесса из /proc/[PID]/status
int getProcessInfo(const char *pid, uid_t uid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/status", pid);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        // Каталог процесса мог исчезнуть, так что просто игнорируем ошибку
        if (errno != ENOENT) {
            perror("fopen");
        }
        return -1;
    }

    char line[MAX_LINE];
    uid_t processUid = -1;
    char processName[256] = {0};

    // Читаем файл, чтобы найти UID и имя процесса
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "Uid:", 4) == 0) {
            sscanf(line, "Uid:\t%u", &processUid);
        } else if (strncmp(line, "Name:", 5) == 0) {
            sscanf(line, "Name:\t%255s", processName);
        }
    }
    fclose(file);

    // Выводим информацию, если UID процесса совпадает с указанным
    if (processUid == uid) {
        printf("PID: %s, Command: %s\n", pid, processName);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <username>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    uid_t uid = userIdFromName(argv[1]);

    // Открываем каталог /proc
    DIR *procDir = opendir("/proc");
    if (procDir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(procDir)) != NULL) {
        // Проверяем, что имя каталога является числом (это означает PID процесса)
        if (entry->d_type == DT_DIR && atoi(entry->d_name) > 0) {
            getProcessInfo(entry->d_name, uid);
        }
    }

    closedir(procDir);
    return EXIT_SUCCESS;
}
/*

### Описание работы кода

1. **Функция `userIdFromName()`**:
   - Эта функция преобразует имя пользователя, указанное в аргументе командной строки, в UID с помощью функции `getpwnam`.

2. **Функция `getProcessInfo()`**:
   - Формирует путь к файлу `/proc/[PID]/status` для каждого процесса.
   - Открывает файл и читает его построчно, чтобы найти `Uid` и `Name`.
   - Если `Uid` процесса совпадает с UID пользователя, указанного в аргументе, программа выводит PID и имя процесса.

3. **Чтение каталогов /proc**:
   - Открывается каталог `/proc` и перебираются все его подкаталоги.
   - Для каждого каталога проверяется, является ли его имя числом (PID процесса). Если это так, программа вызывает `getProcessInfo()` для вывода информации о процессе.

### Запуск программы
Для запуска программы с указанием имени пользователя:
```bash
./procfs_pidmax <username>
```

Программа выведет список процессов с идентификаторами и именами команд для указанного пользователя.
*/
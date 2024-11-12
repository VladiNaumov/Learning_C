/*
код на C создает нужное количество однобайтных файлов с случайными именами, а затем удаляет их по порядку, сортируя по имени файла. 
Программа выводит время, затраченное на создание и удаление файлов. Директорию и количество файлов можно указать в командной строке.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>

void usageError(const char *progName) {
    fprintf(stderr, "Usage: %s directory num_files\n", progName);
    exit(EXIT_FAILURE);
}

// Функция для генерации случайного шестизначного числа в строке
void generateRandomFilename(char *buffer) {
    int randomNum = rand() % 1000000;
    snprintf(buffer, 10, "x%06d", randomNum);
}

// Функция сравнения для сортировки имен файлов
int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        usageError(argv[0]);
    }

    const char *dirPath = argv[1];
    int numFiles = atoi(argv[2]);
    char filePath[PATH_MAX];
    char **fileNames = malloc(numFiles * sizeof(char *));
    if (fileNames == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    // Замер времени для создания файлов
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Создание файлов
    for (int i = 0; i < numFiles; i++) {
        fileNames[i] = malloc(10);  // Выделяем память под имя файла
        if (fileNames[i] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        generateRandomFilename(fileNames[i]);

        snprintf(filePath, PATH_MAX, "%s/%s", dirPath, fileNames[i]);
        int fd = open(filePath, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        write(fd, "", 1);  // Записываем 1 байт в файл
        close(fd);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double creationTime = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("File creation time: %.6f seconds\n", creationTime);

    // Сортировка имен файлов для удаления по возрастанию
    qsort(fileNames, numFiles, sizeof(char *), compare);

    // Замер времени для удаления файлов
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < numFiles; i++) {
        snprintf(filePath, PATH_MAX, "%s/%s", dirPath, fileNames[i]);
        if (unlink(filePath) == -1) {
            perror("unlink");
            exit(EXIT_FAILURE);
        }
        free(fileNames[i]);  // Освобождаем память после удаления файла
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double deletionTime = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("File deletion time: %.6f seconds\n", deletionTime);

    free(fileNames);
    return 0;
}
/*

### Пояснение

1. **Аргументы командной строки**:
   - Путь к директории, где будут создаваться файлы.
   - Количество файлов для создания и удаления.

2. **Генерация имени файла**: Функция `generateRandomFilename()` создает шестизначное случайное число и добавляет его к префиксу "x".

3. **Создание файлов**: В цикле создается указанный в командной строке `numFiles` и записывается 1 байт в каждый файл. Время, затраченное на создание файлов, измеряется.

4. **Сортировка имен для удаления**: Имена файлов сортируются по алфавиту с использованием функции `qsort()`, чтобы файлы удалялись в порядке возрастания.

5. **Удаление файлов**: Каждый файл удаляется в отсортированном порядке, и измеряется время, потраченное на удаление всех файлов.

### Исследование зависимости

Запуская программу с различными значениями `numFiles` и на разных файловых системах, можно выявить следующие зависимости:
- **Время на создание и удаление**: Обычно время увеличивается с ростом числа файлов.
- **Разные файловые системы**: Журналируемые файловые системы, такие как ext3 или ext4, могут показывать более высокую производительность для удаления большого количества файлов за счет ведения журнала метаданных.

Изменяя порядок создания файлов (например, создавая файлы в порядке возрастания и удаляя их в том же порядке), можно обнаружить, что файловые системы, оптимизированные для последовательного доступа, могут показать лучшие результаты в таких условиях.

*/
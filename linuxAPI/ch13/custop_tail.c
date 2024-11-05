/* Для реализации `tail` с использованием системных вызовов (`lseek`, `read`, `write`) можно сделать следующее:

1. Использовать `lseek` для перемещения к концу файла.
2. Читая данные с конца файла, проверять символы новой строки (`\n`) для подсчёта нужного количества строк.
3. После нахождения `num` строк или достижения начала файла, вывести их с помощью `write`.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 8192  // Размер буфера для считывания данных

void tail(const char *filename, int num) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    off_t file_size = lseek(fd, 0, SEEK_END);  // Определение размера файла
    if (file_size == -1) {
        perror("Ошибка поиска конца файла");
        close(fd);
        exit(EXIT_FAILURE);
    }

    int count = 0; // Количество найденных строк
    off_t pos = file_size; // Текущая позиция в файле
    char buf[BUF_SIZE];
    int bytes_read;

    // Чтение файла с конца, пока не найдены нужные строки
    while (pos > 0 && count <= num) {
        ssize_t chunk_size = (pos >= BUF_SIZE) ? BUF_SIZE : pos;  // Определяем размер для lseek
        pos = lseek(fd, -chunk_size, SEEK_CUR); // Переход на chunk_size байт назад
        bytes_read = read(fd, buf, chunk_size); // Чтение chunk_size байт
        pos = lseek(fd, -chunk_size, SEEK_CUR); // Возвращение на chunk_size для корректного read

        if (bytes_read == -1) {
            perror("Ошибка чтения файла");
            close(fd);
            exit(EXIT_FAILURE);
        }

        // Обратный проход по буферу для поиска символов новой строки
        for (int i = bytes_read - 1; i >= 0; i--) {
            if (buf[i] == '\n') {
                count++;
                if (count > num) {
                    pos += i + 1;  // Определяем позицию начала требуемых строк
                    break;
                }
            }
        }
    }

    // Печать найденных строк
    if (lseek(fd, pos, SEEK_SET) == -1) {
        perror("Ошибка установки позиции для вывода");
        close(fd);
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(fd, buf, BUF_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, bytes_read) != bytes_read) {
            perror("Ошибка записи вывода");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        fprintf(stderr, "Использование: %s [ -n num ] file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num = 10; // Количество строк по умолчанию
    const char *filename;

    // Обработка аргументов командной строки
    if (argc == 2) {
        filename = argv[1];
    } else if (argc == 4 && strcmp(argv[1], "-n") == 0) {
        num = atoi(argv[2]);
        if (num <= 0) {
            fprintf(stderr, "Неверное количество строк: %d\n", num);
            exit(EXIT_FAILURE);
        }
        filename = argv[3];
    } else {
        fprintf(stderr, "Неверные аргументы\n");
        exit(EXIT_FAILURE);
    }

    tail(filename, num);

    exit(EXIT_SUCCESS);
}
/*

1. **Чтение с конца файла**: Функция `tail` открывает файл и перемещается к его концу.
2. **Буферизация**: Используется буфер `BUF_SIZE`, чтобы читать по `chunk_size` байт за раз, перемещаясь в обратном направлении и проверяя наличие символов новой строки.
3. **Подсчёт строк**: При нахождении `num` строк, программа фиксирует начальную позицию для вывода.
4. **Вывод строк**: Считанные строки выводятся с помощью `write`.

Этот подход позволяет эффективно работать даже с большими файлами, минимизируя количество обращений к диску.
*/
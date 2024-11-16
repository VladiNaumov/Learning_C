/* 4. File I/O: The Universal I/O */

#define _BSD_SOURCE
#define _GNU_SOURCE
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * open - открывает файл.
 * pathname - путь к файлу.
 * flags - режим открытия (O_RDONLY, O_WRONLY и т.д.).
 * mode - права доступа (необязательно, требуется при создании нового файла).
 * Возвращает файловый дескриптор или -1 при ошибке.
 */
int open(const char *pathname, int flags, ... /* mode_t mode */);

/*
 * creat - создаёт новый файл или перезаписывает существующий.
 * pathname - путь к файлу.
 * mode - права доступа к файлу.
 * Возвращает файловый дескриптор или -1 при ошибке.
 */
int creat(const char *pathname, mode_t mode);

/*
 * read - читает данные из файла в буфер.
 * fd - файловый дескриптор.
 * buffer - указатель на буфер для данных.
 * count - количество байтов для чтения.
 * Возвращает количество прочитанных байтов, 0 на EOF, или -1 при ошибке.
 */
ssize_t read(int fd, void *buffer, size_t count);

/*
 * write - записывает данные из буфера в файл.
 * fd - файловый дескриптор.
 * buffer - указатель на буфер с данными.
 * count - количество байтов для записи.
 * Возвращает количество записанных байтов или -1 при ошибке.
 */
ssize_t write(int fd, void *buffer, size_t count);

/*
 * close - закрывает файловый дескриптор.
 * fd - файловый дескриптор.
 * Возвращает 0 при успешном выполнении или -1 при ошибке.
 */
int close(int fd);

/*
 * lseek - изменяет позицию указателя чтения/записи в файле.
 * fd - файловый дескриптор.
 * offset - смещение.
 * whence - базовая точка (SEEK_SET, SEEK_CUR, SEEK_END).
 * Возвращает новую позицию или -1 при ошибке.
 */
off_t lseek(int fd, off_t offset, int whence);

/*
 * ioctl - управляет устройствами.
 * fd - файловый дескриптор.
 * request - код команды.
 * argp - необязательный аргумент.
 * Возвращает 0 при успешном выполнении или -1 при ошибке.
 */
int ioctl(int fd, int request, ... /* argp */);

/*
 * fcntl - управляет файловым дескриптором.
 * fd - файловый дескриптор.
 * cmd - команда.
 * ... - дополнительные аргументы в зависимости от команды.
 * Возвращает результат выполнения команды или -1 при ошибке.
 */
int fcntl(int fd, int cmd, ...);

/*
 * dup - дублирует файловый дескриптор.
 * oldfd - оригинальный дескриптор.
 * Возвращает новый файловый дескриптор или -1 при ошибке.
 */
int dup(int oldfd);

/*
 * dup2 - дублирует файловый дескриптор в указанный.
 * oldfd - оригинальный дескриптор.
 * newfd - целевой дескриптор.
 * Возвращает новый файловый дескриптор или -1 при ошибке.
 */
int dup2(int oldfd, int newfd);

/*
 * dup3 - дублирует файловый дескриптор с указанными флагами.
 * oldfd - оригинальный дескриптор.
 * newfd - целевой дескриптор.
 * flags - дополнительные флаги (например, O_CLOEXEC).
 * Возвращает новый файловый дескриптор или -1 при ошибке.
 */
int dup3(int oldfd, int newfd, int flags);

/*
 * pread - читает данные из файла с указанного смещения.
 * fd - файловый дескриптор.
 * buf - буфер для данных.
 * count - количество байтов для чтения.
 * offset - начальное смещение.
 * Возвращает количество прочитанных байтов или -1 при ошибке.
 */
ssize_t pread(int fd, void *buf, size_t count, off_t offset);

/*
 * pwrite - записывает данные в файл с указанного смещения.
 * fd - файловый дескриптор.
 * buf - буфер с данными.
 * count - количество байтов для записи.
 * offset - начальное смещение.
 * Возвращает количество записанных байтов или -1 при ошибке.
 */
ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);

/*
 * readv - читает данные из файла в массив структур iovec.
 * fd - файловый дескриптор.
 * iov - массив структур iovec.
 * iovcnt - количество элементов в массиве.
 * Возвращает количество прочитанных байтов, 0 на EOF, или -1 при ошибке.
 */
ssize_t readv(int fd, const struct iovec *iov, int iovcnt);

/*
 * writev - записывает данные из массива структур iovec в файл.
 * fd - файловый дескриптор.
 * iov - массив структур iovec.
 * iovcnt - количество элементов в массиве.
 * Возвращает количество записанных байтов или -1 при ошибке.
 */
ssize_t writev(int fd, const struct iovec *iov, int iovcnt);

/*
 * preadv - читает данные из файла в массив iovec с определённого смещения.
 * fd - файловый дескриптор.
 * iov - массив структур iovec.
 * iovcnt - количество элементов в массиве.
 * offset - начальное смещение.
 * Возвращает количество прочитанных байтов или -1 при ошибке.
 */
ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset);

/*
 * pwritev - записывает данные из массива iovec в файл с определённого смещения.
 * fd - файловый дескриптор.
 * iov - массив структур iovec.
 * iovcnt - количество элементов в массиве.
 * offset - начальное смещение.
 * Возвращает количество записанных байтов или -1 при ошибке.
 */
ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset);

/*
 * truncate - изменяет размер файла.
 * pathname - путь к файлу.
 * length - новая длина файла.
 * Возвращает 0 при успешном выполнении или -1 при ошибке.
 */
int truncate(const char *pathname, off_t length);

/*
 * ftruncate - изменяет размер файла через файловый дескриптор.
 * fd - файловый дескриптор.
 * length - новая длина файла.
 * Возвращает 0 при успешном выполнении или -1 при ошибке.
 */
int ftruncate(int fd, off_t length);

/*
 * mkstemp - создаёт временный файл.
 * template - шаблон имени файла.
 * Возвращает файловый дескриптор или -1 при ошибке.
 */
int mkstemp(char *template);

/*
 * tmpfile - создаёт временный файл.
 * Возвращает указатель на FILE или NULL при ошибке.
 */
FILE *tmpfile(void);


// 1. int creat(const char *pathname, mode_t mode);
// Создаёт новый файл или перезаписывает существующий.
int file_desc = creat("file.txt", 0644);
if (file_desc == -1) {
    perror("Error creating file");
    return -1; // Обработка ошибки
}

// 2. int close(int fd);
// Закрывает файловый дескриптор.
if (close(file_desc) == -1) {
    perror("Error closing file");
    return -1; // Обработка ошибки
}

// 3. off_t lseek(int fd, off_t offset, int whence);
// Устанавливает позицию указателя в файле.
file_desc = creat("file.txt", 0644); // Пересоздание файла для примера
if (file_desc == -1) {
    perror("Error creating file");
    return -1; // Обработка ошибки
}
if (lseek(file_desc, 0, SEEK_SET) == (off_t) -1) {
    perror("Error seeking in file");
    return -1; // Обработка ошибки
}

// 4. int ioctl(int fd, int request, ... /* argp */);
// Управляет устройствами, связанными с дескриптором.
int ret = ioctl(file_desc, some_request, some_argument);
if (ret == -1) {
    perror("Error in ioctl");
    return -1; // Обработка ошибки
}

// 5. int fcntl(int fd, int cmd, ...);
// Управляет файлами.
if (fcntl(file_desc, F_SETFL, O_NONBLOCK) == -1) {
    perror("Error in fcntl");
    return -1; // Обработка ошибки
}

// 6. int dup(int oldfd);
// Дублирует файловый дескриптор.
int new_fd = dup(file_desc);
if (new_fd == -1) {
    perror("Error duplicating file descriptor");
    return -1; // Обработка ошибки
}

// 7. int dup2(int oldfd, int newfd);
// Дублирует `oldfd` в `newfd`.
if (dup2(file_desc, STDOUT_FILENO) == -1) {
    perror("Error duplicating file descriptor to STDOUT");
    return -1; // Обработка ошибки
}

// 8. int dup3(int oldfd, int newfd, int flags);
// То же, что dup2, но с дополнительными флагами.
if (dup3(file_desc, STDOUT_FILENO, O_CLOEXEC) == -1) {
    perror("Error duplicating file descriptor to STDOUT with O_CLOEXEC");
    return -1; // Обработка ошибки
}

// 9. ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
// Считывает данные из файла в несколько буферов.
struct iovec iov[2];
iov[0].iov_base = buf1;
iov[0].iov_len = sizeof(buf1);
iov[1].iov_base = buf2;
iov[1].iov_len = sizeof(buf2);
if (readv(file_desc, iov, 2) == -1) {
    perror("Error reading vectors");
    return -1; // Обработка ошибки
}

// 10. ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
// Записывает данные из нескольких буферов в файл.
if (writev(file_desc, iov, 2) == -1) {
    perror("Error writing vectors");
    return -1; // Обработка ошибки
}

// 11. int mkstemp(char *template);
// Создаёт временный файл с уникальным именем.
char template[] = "tempXXXXXX";
int temp_fd = mkstemp(template);
if (temp_fd == -1) {
    perror("Error creating temporary file");
    return -1; // Обработка ошибки
}

// 12. FILE *tmpfile(void);
// Создаёт временный файл, который удаляется при закрытии.
FILE *temp_file = tmpfile();
if (temp_file == NULL) {
    perror("Error creating temporary file");
    return -1; // Обработка ошибки
}


#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_WRONLY | O_CREAT, 0644);  // Создаем или открываем файл для записи
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    // Записываем данные в файл
    write(fd, "Hello, World!\n", 14);
    close(fd);  // Закрываем файл

    return 0;
}

/*
 * Пример использования read() и write():
 * read() читает данные из файла, а write() записывает их.
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_RDONLY); // Открываем файл только для чтения
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    char buffer[100];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));  // Читаем данные в buffer
    if (bytes_read == -1) {
        perror("read failed");
        close(fd);
        return 1;
    }

    // Печатаем прочитанные данные
    write(STDOUT_FILENO, buffer, bytes_read);
    close(fd);

    return 0;
}

/*
 * Пример использования dup() и dup2():
 * dup() дублирует файловый дескриптор, позволяя перенаправить вывод,
 * например, в файл, а не в стандартный вывод.
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    int new_fd = dup2(fd, STDOUT_FILENO);  // Перенаправляем stdout в файл
    if (new_fd == -1) {
        perror("dup2 failed");
        close(fd);
        return 1;
    }

    printf("This message will be written to the file instead of the console.\n");
    close(fd);

    return 0;
}

/*
 * Пример использования lseek():
 * Позволяет устанавливать положение курсора чтения/записи внутри файла.
 * Например, можно переместить курсор в начало, в конец или в определённую позицию.
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_RDWR);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    lseek(fd, 0, SEEK_END);  // Перемещаем указатель на конец файла
    write(fd, "\nAppended Text", 14);  // Дописываем данные в конец

    close(fd);
    return 0;
}

/*
 * Пример использования pread() и pwrite():
 * Чтение и запись по определённому смещению без изменения текущего положения указателя.
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_RDWR);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    char buffer[100];
    pread(fd, buffer, sizeof(buffer), 0);  // Чтение с начала файла
    printf("Read from file: %s\n", buffer);

    pwrite(fd, "Random Access", 13, 50);  // Запись по смещению 50 байт

    close(fd);
    return 0;
}

/*
 * Пример использования truncate():
 * Устанавливаем новый размер файла (сокращаем или удлиняем).
 */

#include <unistd.h>
#include <stdio.h>

int main() {
    if (truncate("example.txt", 50) == -1) { // Сокращаем файл до 50 байт
        perror("truncate failed");
        return 1;
    }

    printf("File truncated successfully.\n");
    return 0;
}

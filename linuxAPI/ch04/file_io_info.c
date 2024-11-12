/* 4. File I/O: The Universal I/O */

#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

// 1. int creat(const char *pathname, mode_t mode);
// Создаёт новый файл или перезаписывает существующий.
// Используйте для создания файла с определёнными правами доступа.
int file_desc = creat("file.txt", 0644);

// 2. int close(int fd);
// Закрывает файловый дескриптор.
// Используйте для освобождения ресурсов после работы с файлом.
close(file_desc);

// 3. off_t lseek(int fd, off_t offset, int whence);
// Устанавливает позицию указателя в файле. `whence` может быть SEEK_SET, SEEK_CUR, SEEK_END.
// Используйте для перемещения в определённую позицию внутри файла.
lseek(file_desc, 0, SEEK_SET); // Вернуться в начало файла

// 4. int ioctl(int fd, int request, ... /* argp */);
// Управляет устройствами, связанными с дескриптором. 
// Применяйте для специальных операций над устройствами.
ioctl(file_desc, some_request, some_argument);

// 5. int fcntl(int fd, int cmd, ...);
// Управляет файлами, например, устанавливает флаги. 
// Используйте для изменения состояния файла.
fcntl(file_desc, F_SETFL, O_NONBLOCK); // Устанавливает неблокирующий режим

// 6. int dup(int oldfd);
// Дублирует файловый дескриптор.
// Используйте для создания копии файлового дескриптора.
int new_fd = dup(file_desc);

// 7. int dup2(int oldfd, int newfd);
// Дублирует `oldfd` в `newfd`.
// Применяйте, если нужно перенаправить дескриптор на другой.
dup2(file_desc, STDOUT_FILENO); // Перенаправление вывода в файл

// 8. int dup3(int oldfd, int newfd, int flags);
// То же, что dup2, но с дополнительными флагами.
// Применяйте при необходимости добавления флагов при дублировании.
dup3(file_desc, STDOUT_FILENO, O_CLOEXEC);

// 9. ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
// Считывает данные из файла в несколько буферов.
// Используйте для эффективного чтения в несколько областей памяти.
struct iovec iov[2];
iov[0].iov_base = buf1;
iov[0].iov_len = sizeof(buf1);
iov[1].iov_base = buf2;
iov[1].iov_len = sizeof(buf2);
readv(file_desc, iov, 2);

// 10. ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
// Записывает данные из нескольких буферов в файл.
// Используйте для эффективной записи из нескольких областей памяти.
writev(file_desc, iov, 2);

// 11. int mkstemp(char *template);
// Создаёт временный файл с уникальным именем.
// Применяйте для создания временного файла, который можно сразу использовать.
char template[] = "tempXXXXXX";
int temp_fd = mkstemp(template);

// 12. FILE *tmpfile(void);
// Создаёт временный файл, который удаляется при закрытии.
// Используйте для временного хранения данных, где не нужно имя файла.
FILE *temp_file = tmpfile();


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

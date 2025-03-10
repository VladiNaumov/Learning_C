/*
 * compile: gcc copy.c ../lib/error_functions.c -o copy
*/
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../lib/tlpi_hdr.h"  // Включаем заголовочный файл с функциями для обработки ошибок и другим функционалом

#ifndef BUF_SIZE
#define BUF_SIZE 1024  // Определяем размер буфера для чтения данных
#endif

int main(int ac, char *av[])
{
    int inputFd, outputFd, openFlags;  // Дескрипторы файлов для входного и выходного файлов, а также флаги для открытия файлов
    mode_t filePerms;  // Права доступа для нового файла
    ssize_t numRead;  // Переменная для хранения количества прочитанных байт
    char buf[BUF_SIZE];  // Буфер для временного хранения прочитанных данных

    // Проверяем количество аргументов командной строки
    if (ac != 3) {
        usageErr("%s old-file new-file\n", av[0]);  // Если аргументов недостаточно, выводим сообщение об ошибке и завершаем программу
    }

    // Открываем входной файл для чтения
    inputFd = open(av[1], O_RDONLY);
    if (inputFd == -1) {  // Проверяем, удалось ли открыть файл
        errExit("opening file %s", av[1]);  // В случае ошибки выводим сообщение и завершаем выполнение
    }

    // Устанавливаем флаги для создания нового файла:
    // O_CREAT - создать файл, если его нет
    // O_WRONLY - открыть файл только для записи
    // O_TRUNC - если файл существует, обрезать его до нуля
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = 0666;  // Устанавливаем права доступа для нового файла (чтение и запись для всех)
    outputFd = open(av[2], openFlags, filePerms);  // Открываем или создаем файл
    if (outputFd == -1) {  // Проверяем, удалось ли открыть файл
        errExit("opening file %s", av[2]);  // В случае ошибки выводим сообщение и завершаем выполнение
    }

    // Передаем данные из одного файла в другой, пока не достигнем конца файла или ошибки
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {  // Читаем данные из входного файла
        // Пишем в выходной файл столько байт, сколько было прочитано
        if (write(outputFd, buf, numRead) != numRead) {
            fatal("couldn't write whole buffer");  // Если не удалось записать все данные, выводим фатальную ошибку
        }
    }

    // Если произошла ошибка при чтении файла
    if (numRead == -1) {
        errExit("read");  // Выводим сообщение об ошибке чтения
    }

    // Закрываем входной файл
    if (close(inputFd) == -1) {
        errExit("close input");  // Если закрытие файла не удалось, выводим сообщение об ошибке
    }

    // Закрываем выходной файл
    if (close(outputFd) == -1) {
        errExit("close output");  // Если закрытие файла не удалось, выводим сообщение об ошибке
    }

    // Завершаем программу с успешным результатом
    exit(EXIT_SUCCESS);
}

/*
 * Вот основные моменты программы:

1. **Проверка аргументов**: Программа ожидает два аргумента — исходный файл и файл для копирования. Если аргументов не хватает, она завершает работу с ошибкой.

2. **Открытие файлов**:
   - Исходный файл открывается на чтение (`O_RDONLY`).
   - Новый файл открывается на запись с созданием нового файла, если его нет, и с обрезкой содержимого, если файл существует (`O_CREAT | O_WRONLY | O_TRUNC`).

3. **Копирование данных**: Программа читает данные из исходного файла блоками, размер которых определяется константой `BUF_SIZE` (по умолчанию 1024 байта), и записывает их в новый файл.

4. **Обработка ошибок**: Если возникают ошибки при чтении, записи, открытии или закрытии файлов, программа завершает работу с сообщением об ошибке.

5. **Закрытие файлов**: По завершении работы оба файла закрываются, чтобы освободить ресурсы.


 * */
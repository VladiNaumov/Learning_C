/*

### Краткое резюме:

Программа `write_bytes.c` записывает заданное количество байт в файл, используя указанный размер буфера для каждой записи. 
Она предназначена для тестирования производительности ввода-вывода (I/O) при работе с файлами. 
Дополнительно, при компиляции с определенными флагами программа может выполнять синхронизацию данных на диск, чтобы обеспечить их немедленную запись.
*/

/* write_bytes.c

   Записывает байты в файл. (Простая программа для тестирования производительности ввода-вывода.)

   Использование: write_bytes файл количество_байт размер_буфера

   Записывает 'количество_байт' в 'файл', используя размер буфера 'размер_буфера'
   для каждой записи (write()).

   Если компилировать с флагом -DUSE_O_SYNC, то файл открывается с флагом O_SYNC,
   и все данные и метаданные сразу сбрасываются на диск.

   Если компилировать с флагом -DUSE_FDATASYNC, то после каждой записи выполняется fdatasync(),
   что сбрасывает данные и, возможно, метаданные на диск.

   Если компилировать с флагом -DUSE_FSYNC, то после каждой записи выполняется fsync(),
   что сбрасывает и данные, и метаданные на диск.
*/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    // Проверка количества аргументов командной строки
    if (argc != 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s файл количество_байт размер_буфера\n", argv[0]);

    size_t numBytes = getLong(argv[2], GN_GT_0, "количество_байт");  // Количество байт для записи
    size_t bufSize = getLong(argv[3], GN_GT_0, "размер_буфера");     // Размер буфера

    char *buf = malloc(bufSize); // Выделение памяти под буфер
    if (buf == NULL)
        errExit("malloc");

    int openFlags = O_CREAT | O_WRONLY;

    // Если компилируется с USE_O_SYNC, открыть файл с O_SYNC для немедленной записи
#if defined(USE_O_SYNC) && defined(O_SYNC)
    openFlags |= O_SYNC;
#endif

    int fd = open(argv[1], openFlags, S_IRUSR | S_IWUSR);  // Открытие файла на запись
    if (fd == -1)
        errExit("open");

    size_t thisWrite, totWritten;
    // Цикл записи в файл, пока не будет записано нужное количество байт
    for (totWritten = 0; totWritten < numBytes; totWritten += thisWrite) {
        thisWrite = min(bufSize, numBytes - totWritten);  // Определение размера следующей записи

        if (write(fd, buf, thisWrite) != thisWrite)  // Выполнение записи
            fatal("partial/failed write");

        // Синхронизация после каждой записи, если компилируется с определенными флагами
#ifdef USE_FSYNC
        if (fsync(fd))
            errExit("fsync");
#endif
#ifdef USE_FDATASYNC
        if (fdatasync(fd))
            errExit("fdatasync");
#endif
    }

    if (close(fd) == -1)  // Закрытие файла
        errExit("close");
    exit(EXIT_SUCCESS);    // Завершение программы
}

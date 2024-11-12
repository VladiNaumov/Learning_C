/* file_type_stats.c

   Пример использования nftw(): обходит дерево директорий, указанное в командной строке,
   и выводит статистику по типам файлов в дереве.
*/

#if defined(__sun)
#define _XOPEN_SOURCE 500   /* Для Solaris 8 необходимо использовать именно это значение */
#else
#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 600
#define _XOPEN_SOURCE 600   /* Подключаем nftw() и объявления для S_IFSOCK */
#endif
#endif

#include <ftw.h>
#include "tlpi_hdr.h"

/* Счетчики для различных типов файлов */
static int numReg = 0, numDir = 0, numSymLk = 0, numSocket = 0,
           numFifo = 0, numChar = 0, numBlock = 0,
           numNonstatable = 0;

/* Функция, вызываемая nftw() для каждого файла */
static int countFile(const char *path, const struct stat *sb, int flag, struct FTW *ftwb)
{
    if (flag == FTW_NS) {  // Если файл не может быть "статизирован" (не удается вызвать stat)
        numNonstatable++;
        return 0;
    }

    // Определяем тип файла и увеличиваем соответствующий счетчик
    switch (sb->st_mode & S_IFMT) {
    case S_IFREG:  numReg++;    break;   // Обычный файл
    case S_IFDIR:  numDir++;    break;   // Директория
    case S_IFCHR:  numChar++;   break;   // Символьное устройство
    case S_IFBLK:  numBlock++;  break;   // Блочное устройство
    case S_IFLNK:  numSymLk++;  break;   // Символическая ссылка
    case S_IFIFO:  numFifo++;   break;   // FIFO (именованный канал)
    case S_IFSOCK: numSocket++; break;   // Сокет
    }
    return 0;  // Сообщаем nftw() продолжить обход
}

/* Печатает статистику для определенного типа файла */
static void printStats(const char *msg, int num, int numFiles)
{
    printf("%-15s   %6d %6.1f%%\n", msg, num, num * 100.0 / numFiles);
}

int main(int argc, char *argv[])
{
    // Проверяем корректность аргументов командной строки
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s dir-path\n", argv[0]);

    /* Обходим дерево директорий и считаем файлы, не следуем по символическим ссылкам */
    if (nftw(argv[1], &countFile, 20, FTW_PHYS) == -1)
        errExit("nftw");

    // Подсчитываем общее количество файлов
    int numFiles = numReg + numDir + numSymLk + numSocket +
                   numFifo + numChar + numBlock + numNonstatable;

    if (numFiles == 0) {
        printf("No files found\n");
    } else {
        printf("Total files:      %6d\n", numFiles);
        printStats("Regular:", numReg, numFiles);
        printStats("Directory:", numDir, numFiles);
        printStats("Char device:", numChar, numFiles);
        printStats("Block device:", numBlock, numFiles);
        printStats("Symbolic link:", numSymLk, numFiles);
        printStats("FIFO:", numFifo, numFiles);
        printStats("Socket:", numSocket, numFiles);
        printStats("Non-statable:", numNonstatable, numFiles);
    }
    exit(EXIT_SUCCESS);
}

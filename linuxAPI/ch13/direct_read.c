/* direct_read.c

   Демонстрация использования флага O_DIRECT для выполнения ввода-вывода
   без использования буферной кэш-памяти ("прямой I/O").

   Использование: direct_read файл длина [смещение [выравнивание]]

   Эта программа специфична для Linux.
*/

#define _GNU_SOURCE     /* Подключение определения O_DIRECT из <fcntl.h> */
#include <fcntl.h>
#include <malloc.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int fd;
    ssize_t numRead;
    size_t length, alignment;
    off_t offset;
    char *buf;

    // Проверка аргументов командной строки
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s файл длина [смещение [выравнивание]]\n", argv[0]);

    length = getLong(argv[2], GN_ANY_BASE, "длина");       // Количество байт для чтения
    offset = (argc > 3) ? getLong(argv[3], GN_ANY_BASE, "смещение") : 0;  // Смещение в файле
    alignment = (argc > 4) ? getLong(argv[4], GN_ANY_BASE, "выравнивание") : 4096;  // Выравнивание

    fd = open(argv[1], O_RDONLY | O_DIRECT);  // Открытие файла в режиме прямого I/O
    if (fd == -1)
        errExit("open");

    /* Функция memalign() выделяет блок памяти, выровненный по адресу,
       который кратен первому аргументу. Указав этот аргумент как 2 * 'выравнивание'
       и добавив 'выравнивание' к возвращенному указателю, мы добиваемся,
       что 'buf' выравнивается по нечетному кратному 'выравнивания'.
       Это позволяет, например, при запросе буфера с выравниванием 256 байт
       избежать получения буфера, выровненного по границе 512 байт. */

    buf = memalign(alignment * 2, length + alignment); // Выделение выровненной памяти
    if (buf == NULL)
        errExit("memalign");

    buf += alignment;  // Смещение указателя для точного выравнивания

    if (lseek(fd, offset, SEEK_SET) == -1)  // Установка смещения в файле
        errExit("lseek");

    numRead = read(fd, buf, length);  // Чтение данных из файла
    if (numRead == -1)
        errExit("read");
    printf("Прочитано %ld байт\n", (long) numRead);

    exit(EXIT_SUCCESS);
}

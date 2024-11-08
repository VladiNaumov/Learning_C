/* chiflag.c

   Изменяет флаги i-узла (также известные как расширенные атрибуты файловой системы ext2) 
   для файлов, указанных в командной строке. Использование программы указано в usageError().

   Эта программа не соберётся на системе с версией ядра <linux/fs.h>, выпущенной до 2.6.19.
   В таком случае, возможно, потребуется включить <linux/ext2_fs.h> и заменить 
   все FS_* на EXT2_*.

   Программа специфична для Linux.
*/

#define _GNU_SOURCE
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include "tlpi_hdr.h"

/*
 * Функция usageError - Выводит сообщение об использовании программы и описание
 * поддерживаемых атрибутов.
 * Параметры:
 *   progName - имя программы.
 * Вызывает exit(EXIT_FAILURE) для завершения программы.
 */
static void usageError(const char *progName)
{
    fprintf(stderr, "Usage: %s {+-=}{attrib-chars} file...\n\n", progName);
#define fpe(str) fprintf(stderr, "    " str)            /* Упрощение записи */
    fpe("+ добавляет атрибут; - удаляет атрибут; "
                        "= устанавливает атрибуты абсолютно\n\n");
    fpe("'attrib-chars' содержит один или несколько символов:\n");
    fpe("    a   Принудительное включение O_APPEND (требуются привилегии)\n");
    fpe("    A   Не обновлять время последнего доступа\n");
    fpe("    c   Сжатие (требуется пакет e2compr)\n");
    fpe("    d   Исключить из резервного копирования dump(8)\n");
    fpe("    D   Синхронное обновление каталогов\n");
    fpe("    i   Неизменяемость (требуются привилегии)\n");
    fpe("    j   Включение журналирования данных для ext3/ext4\n");
    fpe("    s   Безопасное удаление (не реализовано)\n");
    fpe("    S   Синхронное обновление файлов\n");
    fpe("    t   Отключить упаковку хвостов (только для Reiserfs)\n");
    fpe("    T   Пометить как корневой каталог для алгоритма Orlov\n");
    fpe("    u   Восстановление (не реализовано)\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc < 3 || strchr("+-=", argv[1][0]) == NULL || strcmp(argv[1], "--help") == 0)
        usageError(argv[0]);

    /* Создание битовой маски атрибутов на основе строки в argv[1] */

    int attr = 0;
    for (char *p = &argv[1][1]; *p != '\0'; p++) {
        switch (*p) {
        case 'a': attr |= FS_APPEND_FL;         break; // Добавить атрибут "добавление в конец"
        case 'A': attr |= FS_NOATIME_FL;        break; // Отключить обновление времени доступа
        case 'c': attr |= FS_COMPR_FL;          break; // Сжатие
        case 'd': attr |= FS_NODUMP_FL;         break; // Исключить из резервного копирования
        case 'D': attr |= FS_DIRSYNC_FL;        break; // Синхронное обновление каталогов
        case 'i': attr |= FS_IMMUTABLE_FL;      break; // Сделать файл неизменяемым
        case 'j': attr |= FS_JOURNAL_DATA_FL;   break; // Журналирование данных
        case 's': attr |= FS_SECRM_FL;          break; // Безопасное удаление (не реализовано)
        case 'S': attr |= FS_SYNC_FL;           break; // Синхронное обновление
        case 't': attr |= FS_NOTAIL_FL;         break; // Отключить упаковку хвостов (для Reiserfs)
        case 'T': attr |= FS_TOPDIR_FL;         break; // Корневой каталог для алгоритма Orlov
        case 'u': attr |= FS_UNRM_FL;           break; // Восстановление (не реализовано)
        default:  usageError(argv[0]);
        }
    }

    /* Открываем каждый файл по очереди и изменяем его атрибуты */

    for (int j = 2; j < argc; j++) {
        int fd = open(argv[j], O_RDONLY);
        if (fd == -1) {         /* Скорее всего, ошибка из-за отсутствующего файла */
            errMsg("open: %s", argv[j]);
            continue;
        }

        /* Если argv[1] начинается с + или -, то получаем текущие атрибуты
           и изменяем их по необходимости */

        if (argv[1][0] == '+' || argv[1][0] == '-') {
            int oldAttr;
            if (ioctl(fd, FS_IOC_GETFLAGS, &oldAttr) == -1)
                errExit("ioctl1: %s", argv[j]);
            attr = (*argv[1] == '-') ? (oldAttr & ~attr) : (oldAttr | attr);
        }

        if (ioctl(fd, FS_IOC_SETFLAGS, &attr) == -1)
            errExit("ioctl2: %s", argv[j]);
        if (close(fd) == -1)
            errExit("close");
    }

    exit(EXIT_SUCCESS);
}

/*
Резюме:
Эта программа позволяет изменять атрибуты файлов (i-node flags), такие как неизменяемость, 
сжатие, синхронное обновление и другие, на системах Linux. Программа принимает команду
на установку, удаление или абсолютное задание атрибутов и применяет её к каждому 
файлу, указанному в командной строке. 
Используются системные вызовы ioctl для получения и установки флагов.
*/

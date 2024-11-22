/* demo_file_caps.c

   Отображает полномочия и возможности процесса, а также пытается открыть файл,
   имя которого передано в аргументе argv[1].

   Этот пример используется для демонстрации возможностей файловых прав.
   Если для исполняемого файла задать право CAP_DAC_READ_SEARCH:

        setcap cap_dac_read_search=pe

   то программа сможет открывать любой файл для чтения.
*/

#define _GNU_SOURCE
#include <sys/capability.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

// Макрос для обработки ошибок
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    /* Получение и отображение возможностей процесса */

    cap_t caps = cap_get_proc(); // Получаем текущие полномочия процесса
    if (caps == NULL)
        errExit("cap_get_proc");

    char *str = cap_to_text(caps, NULL); // Преобразуем полномочия в строку
    if (str == NULL)
        errExit("cap_to_text");

    printf("Capabilities: %s\n", str); // Выводим полномочия

    // Освобождаем память, выделенную под строку и структуру cap_t
    cap_free(caps);
    cap_free(str);

    /* Если передан аргумент, пытаемся открыть указанный файл */

    if (argc > 1) {
        int fd = open(argv[1], O_RDONLY); // Пытаемся открыть файл только для чтения
        if (fd >= 0) {
            printf("Successfully opened %s\n", argv[1]);
            close(fd); // Закрываем файл, если он был успешно открыт
        } else {
            printf("Open failed: %s\n", strerror(errno)); // Сообщаем об ошибке
        }
    }

    exit(EXIT_SUCCESS); // Успешное завершение программы
}

/*
### Объяснение:
1. **Вывод полномочий**: 
   - `cap_get_proc()` получает текущие полномочия процесса.
   - `cap_to_text()` преобразует их в удобочитаемую строку.

2. **Попытка открыть файл**:
   - `open(argv[1], O_RDONLY)` проверяет возможность открытия файла на чтение.
   - Если файл открыт, выводится сообщение об успехе; в противном случае — ошибка.

3. **Демонстрация**:
   - С помощью `setcap cap_dac_read_search=pe` исполняемому файлу можно дать право обходить ограничения проверки доступа. Это позволяет открыть файл независимо от стандартных правил.

4. **Пример использования**:
   - Создайте защищённый файл: `touch protected_file && chmod 600 protected_file`.
   - Попробуйте открыть его через программу до и после назначения полномочий.
*/
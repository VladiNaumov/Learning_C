/* t_cap_set_file.c

   Программа для установки полномочий на файл.

   Использование: ./t_cap_set_file "<textual-cap-set>" <pathname>
   Пример: ./t_cap_set_file "cap_net_admin=ep" /path/to/file
*/

#include <sys/capability.h>
#include "tlpi_hdr.h" // Включает обработку ошибок и стандартные библиотеки

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <textual-cap-set> <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Преобразование текстового представления полномочий в структуру cap_t
    cap_t capSets = cap_from_text(argv[1]);
    if (capSets == NULL)
        errExit("cap_from_text");

    // Проверяем и выводим минимальное текстовое представление
    char *textCaps = cap_to_text(capSets, NULL);
    if (textCaps == NULL)
        errExit("cap_to_text");

    printf("caps_to_text() returned \"%s\"\n\n", textCaps);

    // Установка полномочий на указанный файл
    if (cap_set_file(argv[2], capSets) == -1)
        errExit("cap_set_file");

    // Освобождаем память, выделенную под строку и структуру
    if (cap_free(textCaps) != 0 || cap_free(capSets) != 0)
        errExit("cap_free");

    exit(EXIT_SUCCESS);
}

/*
### Объяснение кода:
1. **Аргументы командной строки**:
   - Первый аргумент (`argv[1]`) — текстовое описание полномочий, например, `"cap_net_admin=ep"`.
   - Второй аргумент (`argv[2]`) — путь к файлу, для которого задаются полномочия.

2. **Функции:**
   - `cap_from_text()`: Преобразует текстовое представление полномочий в формат, понятный ядру Linux.
   - `cap_set_file()`: Устанавливает полномочия для указанного файла.
   - `cap_to_text()`: Выводит минимальное текстовое представление полномочий.
   - `cap_free()`: Освобождает ресурсы, выделенные для структуры cap_t и строки.

3. **Пример использования**:
   ```bash
   # Добавить полномочия к файлу
   ./t_cap_set_file "cap_net_bind_service=ep" /path/to/file
   ```

   После выполнения можно проверить права файла:
   ```bash
   getcap /path/to/file
   ```

4. **Замечание**:
   - Для использования этой программы могут потребоваться привилегии суперпользователя (`sudo`), так как изменение полномочий файла требует особых прав.
*/
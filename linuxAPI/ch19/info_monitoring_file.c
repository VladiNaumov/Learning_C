/* 19 Monitoring File Events */

/*
 * inotify_init - инициализирует inotify и создает файловый дескриптор.
 * Возвращает файловый дескриптор для дальнейшей работы с inotify или -1 при ошибке.
 */
#include <sys/inotify.h>
int inotify_init(void);

/*
 * inotify_add_watch - добавляет наблюдатель для указанного пути.
 * fd - файловый дескриптор inotify.
 * pathname - путь к файлу или каталогу, который нужно отслеживать.
 * mask - маска событий, которые будут отслеживаться (например, IN_MODIFY, IN_CREATE, IN_DELETE).
 * Возвращает идентификатор наблюдателя (wd) или -1 при ошибке.
 */
#include <sys/inotify.h>
int inotify_add_watch(int fd, const char *pathname, uint32_t mask);

/*
 * inotify_rm_watch - удаляет наблюдатель для указанного пути.
 * fd - файловый дескриптор inotify.
 * wd - идентификатор наблюдателя, который нужно удалить.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <sys/inotify.h>
int inotify_rm_watch(int fd, uint32_t wd);


/*

### 1. **`inotify_init()`**
Эта функция инициализирует inotify и возвращает дескриптор, который используется для добавления и удаления наблюдателей. Если функция возвращает -1, то произошла ошибка.

*/
#include <sys/inotify.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd;

    // Инициализация inotify
    fd = inotify_init();
    if (fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    printf("Inotify initialized successfully with fd: %d\n", fd);

    // Закрытие дескриптора inotify
    close(fd);

    return 0;
}
/*
**Объяснение:**
- `inotify_init()` создает новый inotify дескриптор, который будет использоваться для дальнейших операций. Этот дескриптор должен быть закрыт с помощью `close(fd)` после завершения работы.
- В случае ошибки будет возвращено -1, и следует вызвать `perror` для вывода сообщения об ошибке.

*/


/*
### 2. **`inotify_add_watch()`**
Эта функция добавляет наблюдатель для указанного файла или каталога. Она отслеживает изменения, указанные в маске (например, создание, удаление, изменение файлов).

*/

#include <sys/inotify.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd, wd;

    // Инициализация inotify
    fd = inotify_init();
    if (fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Добавляем наблюдатель для каталога "mydir"
    wd = inotify_add_watch(fd, "mydir", IN_CREATE | IN_DELETE | IN_MODIFY);
    if (wd == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    printf("Watching directory 'mydir' with wd: %d\n", wd);

    // Закрытие дескриптора inotify
    close(fd);

    return 0;
}
/*

**Объяснение:**
- `inotify_add_watch(fd, pathname, mask)` добавляет наблюдателя для файла или каталога по пути `pathname`.
- `mask` — это комбинация флагов, которые определяют, какие события нужно отслеживать (например, `IN_CREATE`, `IN_DELETE`, `IN_MODIFY` и т. д.).
  - `IN_CREATE`: Файлы или каталоги создаются.
  - `IN_DELETE`: Файлы или каталоги удаляются.
  - `IN_MODIFY`: Файл был изменен.

Если функция возвращает -1, произошла ошибка.

*/

/*

### 3. **`inotify_rm_watch()`**
Эта функция удаляет наблюдателя для ранее добавленного пути, используя идентификатор наблюдателя (watch descriptor), который был получен при вызове `inotify_add_watch()`.

*/

#include <sys/inotify.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd, wd;

    // Инициализация inotify
    fd = inotify_init();
    if (fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Добавляем наблюдатель для каталога "mydir"
    wd = inotify_add_watch(fd, "mydir", IN_CREATE | IN_DELETE | IN_MODIFY);
    if (wd == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    printf("Watching directory 'mydir' with wd: %d\n", wd);

    // Удаляем наблюдатель
    if (inotify_rm_watch(fd, wd) == -1) {
        perror("inotify_rm_watch");
        exit(EXIT_FAILURE);
    }

    printf("Stopped watching 'mydir'.\n");

    // Закрытие дескриптора inotify
    close(fd);

    return 0;
}

/*
**Объяснение:**
- `inotify_rm_watch(fd, wd)` удаляет наблюдателя, используя дескриптор `wd`, который был возвращен функцией `inotify_add_watch()`.
- После удаления наблюдателя `wd` больше не будет отслеживать изменения в директории или файле.



### Основные моменты:

1. **Инициализация и работа с дескриптором:**
   - `inotify_init()` создает новый дескриптор, который используется для добавления наблюдателей с помощью `inotify_add_watch()` и для их удаления с помощью `inotify_rm_watch()`.
   
2. **Маски событий:**
   - Маска, передаваемая в `inotify_add_watch()`, может содержать одно или несколько значений, определяющих типы событий, которые следует отслеживать. Например, `IN_CREATE` отслеживает создание файлов, а `IN_MODIFY` отслеживает изменения.

3. **Закрытие дескриптора:**
   - После завершения работы с inotify необходимо закрыть дескриптор с помощью `close(fd)`.

4. **Сложности и ограничения:**
   - inotify работает в реальном времени, но важно помнить, что система ограничена количеством наблюдателей и событий, которые могут быть обработаны одновременно (ограничения задаются ядром Linux).
   - Следует следить за производительностью при отслеживании большого числа файлов или каталогов.

Эти функции полезны для реализации механизма мониторинга изменений в файлах или каталогах в реальном времени.

*/

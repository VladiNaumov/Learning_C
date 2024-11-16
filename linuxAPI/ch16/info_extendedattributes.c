/* 16 Extended Attributes */


/* Расширенные атрибуты служат для реализации списков контроля доступа
   и других возможностей файлов (например, метаданных). */

#include <sys/xattr.h>

/*
 * setxattr - устанавливает расширенный атрибут для файла.
 * pathname - путь к файлу.
 * name - имя атрибута.
 * value - значение атрибута.
 * size - размер значения.
 * flags - флаги.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <sys/xattr.h>
int setxattr(const char *pathname, const char *name, const void *value, size_t size, int flags);

/*
 * lsetxattr - устанавливает расширенный атрибут для файла, следуя символическим ссылкам.
 * pathname - путь к файлу.
 * name - имя атрибута.
 * value - значение атрибута.
 * size - размер значения.
 * flags - флаги.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <sys/xattr.h>
int lsetxattr(const char *pathname, const char *name, const void *value, size_t size, int flags);

/*
 * fsetxattr - устанавливает расширенный атрибут для файла по файловому дескриптору.
 * fd - файловый дескриптор.
 * name - имя атрибута.
 * value - значение атрибута.
 * size - размер значения.
 * flags - флаги.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <sys/xattr.h>
int fsetxattr(int fd, const char *name, const void *value, size_t size, int flags);

/*
 * getxattr - получает расширенный атрибут для файла.
 * pathname - путь к файлу.
 * name - имя атрибута.
 * value - буфер для значения атрибута.
 * size - размер буфера.
 * Возвращает размер значения атрибута или -1 при ошибке.
 */
#include <sys/xattr.h>
ssize_t getxattr(const char *pathname, const char *name, void *value, size_t size);

/*
 * lgetxattr - получает расширенный атрибут для файла, следуя символическим ссылкам.
 * pathname - путь к файлу.
 * name - имя атрибута.
 * value - буфер для значения атрибута.
 * size - размер буфера.
 * Возвращает размер значения атрибута или -1 при ошибке.
 */
#include <sys/xattr.h>
ssize_t lgetxattr(const char *pathname, const char *name, void *value, size_t size);

/*
 * fgetxattr - получает расширенный атрибут для файла по файловому дескриптору.
 * fd - файловый дескриптор.
 * name - имя атрибута.
 * value - буфер для значения атрибута.
 * size - размер буфера.
 * Возвращает размер значения атрибута или -1 при ошибке.
 */
#include <sys/xattr.h>
ssize_t fgetxattr(int fd, const char *name, void *value, size_t size);

/*
 * removexattr - удаляет расширенный атрибут для файла.
 * pathname - путь к файлу.
 * name - имя атрибута.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <sys/xattr.h>
int removexattr(const char *pathname, const char *name);

/*
 * lremovexattr - удаляет расширенный атрибут для файла, следуя символическим ссылкам.
 * pathname - путь к файлу.
 * name - имя атрибута.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <sys/xattr.h>
int lremovexattr(const char *pathname, const char *name);

/*
 * fremovexattr - удаляет расширенный атрибут для файла по файловому дескриптору.
 * fd - файловый дескриптор.
 * name - имя атрибута.
 * Возвращает 0 при успехе, -1 при ошибке.
 */
#include <sys/xattr.h>
int fremovexattr(int fd, const char *name);

/*
 * listxattr - перечисляет имена всех расширенных атрибутов файла.
 * pathname - путь к файлу.
 * list - буфер для хранения имен атрибутов.
 * size - размер буфера.
 * Возвращает количество байт, записанных в list, или -1 при ошибке.
 */
ssize_t listxattr(const char *pathname, char *list, size_t size);

/*
 * llistxattr - перечисляет имена всех расширенных атрибутов файла, следуя символическим ссылкам.
 * pathname - путь к файлу.
 * list - буфер для хранения имен атрибутов.
 * size - размер буфера.
 * Возвращает количество байт, записанных в list, или -1 при ошибке.
 */
ssize_t llistxattr(const char *pathname, char *list, size_t size);

/*
 * flistxattr - перечисляет имена всех расширенных атрибутов файла по файловому дескриптору.
 * fd - файловый дескриптор.
 * list - буфер для хранения имен атрибутов.
 * size - размер буфера.
 * Возвращает количество байт, записанных в list, или -1 при ошибке.
 */
ssize_t flistxattr(int fd, char *list, size_t size);


/*

### Резюме:
Расширенные атрибуты позволяют хранить дополнительные метаданные о файлах и каталогах, которые не могут быть сохранены в стандартных атрибутах файловой системы. Это может включать данные, связанные с контролем доступа (например, списки контроля доступа, ACL), безопасности и других настроек.

Основные функции:
- **setxattr**, **lsetxattr**, **fsetxattr** — для установки атрибутов на файлы, символические ссылки и открытые файлы.
- **getxattr**, **lgetxattr**, **fgetxattr** — для получения атрибутов файлов и символических ссылок.
- **removexattr**, **lremovexattr**, **fremovexattr** — для удаления атрибутов.
- **listxattr**, **llistxattr**, **flistxattr** — для получения списка всех установленных атрибутов.

Эти функции полезны для реализации специфических возможностей файлов, таких как безопасный доступ, метаданные, или другие атрибуты, которые могут быть использованы операционной системой или приложениями.

*/
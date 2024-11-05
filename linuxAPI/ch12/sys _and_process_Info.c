/*
Рассматриваются способы получения различной информации о системе
и процессе. Основное внимание в ней уделяется файловой системе /proc. Кроме того,
дается описание системного вызова uname(), используемого для извлечения различных
идентификаторов системы.
 */

#include <sys/utsname.h>
int uname(struct utsname * utsbuf );

/* Функция `uname` заполняет структуру `utsname` информацией о текущей операционной системе и оборудовании. Она используется для получения данных, таких как имя операционной системы, имя узла, версия ядра и другие системные характеристики.

### Структура `utsname`
Структура `utsname` обычно выглядит так:

struct utsname {
    char sysname[];    // Название операционной системы
    char nodename[];   // Имя узла в сети
    char release[];    // Версия ядра
    char version[];    // Дополнительная версия ядра
    char machine[];    // Аппаратная платформа (архитектура)
};


### Возвращаемое значение
- Успешное выполнение: возвращает `0`.
- При ошибке: возвращает `-1` и устанавливает значение `errno` для указания типа ошибки.

### Пример использования
Пример показывает, как можно использовать `uname` для вывода информации о системе.

*/

#include <stdio.h>
#include <sys/utsname.h>

int main() {
    struct utsname sys_info;
    
    // Вызов uname для получения системной информации
    if (uname(&sys_info) == 0) {
        printf("System Name:    %s\n", sys_info.sysname);
        printf("Node Name:      %s\n", sys_info.nodename);
        printf("Release:        %s\n", sys_info.release);
        printf("Version:        %s\n", sys_info.version);
        printf("Machine:        %s\n", sys_info.machine);
    } else {
        perror("uname error");
    }

    return 0;
}

/*
### Описание примера
- Функция `uname` вызывается с указателем на `sys_info`, который будет заполнен системной информацией.
- Если `uname` выполнится успешно, программа выведет информацию о системе.
- При ошибке будет показано сообщение об ошибке с помощью `perror`. 

Этот код может быть полезен для диагностики или для проверки системной совместимости перед выполнением операций.
*/
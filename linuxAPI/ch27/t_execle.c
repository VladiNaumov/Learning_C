/* t_execle.c

   Демонстрация использования execle() для выполнения программы.
*/
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    char *envVec[] = { "GREET=salut", "BYE=adieu", NULL };  /* Устанавливаем переменные окружения */
    char *filename;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    /* Выполняем программу, указанную в argv[1] */

    filename = strrchr(argv[1], '/');       /* Получаем имя файла из полного пути */
    if (filename != NULL)
        filename++;
    else
        filename = argv[1];                 /* Если путь не содержит '/', берем argv[1] как имя файла */

    execle(argv[1], filename, "hello world", "goodbye", (char *) NULL, envVec);
    errExit("execle");          /* Если мы оказались здесь, значит, execle() завершился с ошибкой */
}
/*
### Резюме кода

Этот код демонстрирует использование функции `execle()` для выполнения программы, заданной первым аргументом командной строки. 
Переменные окружения передаются через `envVec`. `execle()` заменяет текущий процесс на новую программу, 
и если выполнение завершается ошибкой, вызывается `errExit()`.
*/
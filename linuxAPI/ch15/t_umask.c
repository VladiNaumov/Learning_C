/* t_umask.c

   Демонстрирует влияние функции umask() в сочетании с open() и mkdir().
*/

#include <sys/stat.h>
#include <fcntl.h>
#include "file_perms.h"
#include "tlpi_hdr.h"

#define MYFILE "myfile"           // Имя файла для создания
#define MYDIR  "mydir"            // Имя директории для создания
#define FILE_PERMS    (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) // Запрашиваемые права для файла
#define DIR_PERMS     (S_IRWXU | S_IRWXG | S_IRWXO)           // Запрашиваемые права для директории
#define UMASK_SETTING (S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH) // Устанавливаемая маска umask

int
main(int argc, char *argv[])
{
    int fd;
    struct stat sb;
    mode_t u;

    umask(UMASK_SETTING);         // Устанавливаем umask, запрещая указанные права

    fd = open(MYFILE, O_RDWR | O_CREAT | O_EXCL, FILE_PERMS);
    if (fd == -1)
        errExit("open-%s", MYFILE);
    if (mkdir(MYDIR, DIR_PERMS) == -1)
        errExit("mkdir-%s", MYDIR);

    u = umask(0);                 // Получаем текущее значение umask и сбрасываем его

    if (stat(MYFILE, &sb) == -1)
        errExit("stat-%s", MYFILE);
    printf("Requested file perms: %s\n", filePermStr(FILE_PERMS, 0));
    printf("Process umask:        %s\n", filePermStr(u, 0));
    printf("Actual file perms:    %s\n\n", filePermStr(sb.st_mode, 0));

    if (stat(MYDIR, &sb) == -1)
        errExit("stat-%s", MYDIR);
    printf("Requested dir. perms: %s\n", filePermStr(DIR_PERMS, 0));
    printf("Process umask:        %s\n", filePermStr(u, 0));
    printf("Actual dir. perms:    %s\n", filePermStr(sb.st_mode, 0));

    if (unlink(MYFILE) == -1)
        errMsg("unlink-%s", MYFILE); // Удаляем созданный файл
    if (rmdir(MYDIR) == -1)
        errMsg("rmdir-%s", MYDIR);   // Удаляем созданную директорию
    exit(EXIT_SUCCESS);
}

/*
Резюме:
Эта программа демонстрирует работу функции umask, влияющей на права доступа при создании
файлов и директорий. Устанавливается маска umask, создаются файл и директория с указанными
правами доступа, затем сравниваются запрашиваемые и фактические права. Выводится текущая 
маска и реальные права, наложенные на файл и директорию. В конце файл и директория удаляются.
*/

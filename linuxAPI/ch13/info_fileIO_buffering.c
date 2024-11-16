/* 13. File I/O Buffering */

/*  Резюме
Буферизация входных и выходных данных выполняется ядром, а также библиотекой 
stdio. В некоторых случаях может понадобиться предотвратить буферизацию, но при 
этом нужно учитывать влияние, оказываемое на производительность приложения. Для 
управления буферизацией, выполняемой в ядре и осуществляемой библиотечными 
функциями, и для однократных сбросов буферов можно использовать разнообразные 
системные вызовы и библиотечные функции.
Для уведомления ядра о предпочитаемой схеме обращения к данным из указанного 
файла процесс может воспользоваться функцией posix_fadvise(). Ядро может применить 
эту информацию для оптимизации применения буферной кэш-памяти, повысив таким 
образом производительность ввода-вывода.
Характерный для Linux флаг O_DIRECT, используемый при системном вызове open(), 
позволяет специализированным приложениям обходить буферную кэш-память.
Функции fileno() и fdopen() помогают решить задачу смешивания системных вызовов 
и стандартных библиотечных функций языка C, чтобы выполнять ввод-вывод в отношении 
одного и того же файла. Для заданного потока функция fileno() возвращает соответствую-
щий дескриптор файла, а функция fdopen() выполняет обратную операцию, создавая новый 
поток, который использует указанный открытый дескриптор файла. */

#include <stdio.h>

/* Устанавливает тип буферизации для потока:
 * stream - указатель на поток
 * buf - указатель на буфер
 * mode - режим буферизации (_IOFBF - полная, _IOLBF - построчная, _IONBF - без буфера)
 * size - размер буфера */
int setvbuf(FILE *stream, char *buf, int mode, size_t size);

#include <stdio.h>

/* Устанавливает буферизацию для потока (эквивалент setvbuf с _IONBF) */
void setbuf(FILE *stream, char *buf);

#define _BSD_SOURCE
#include <stdio.h>

/* Устанавливает буфер для потока
 * stream - указатель на поток
 * buf - указатель на буфер
 * size - размер буфера */
void setbuffer(FILE *stream, char *buf, size_t size);

#include <stdio.h>

/* Сбрасывает буфер потока
 * stream - указатель на поток */
int fflush(FILE *stream);

#include <unistd.h>

/* Синхронизирует данные из буферов с диском
 * fd - дескриптор файла */
int fsync(int fd);

#include <unistd.h>

/* Синхронизирует только данные (без метаданных) с диском
 * fd - дескриптор файла */
int fdatasync(int fd);

#include <unistd.h>

/* Синхронизирует данные всех файлов с диском */
void sync(void);

#define _XOPEN_SOURCE 600
#include <fcntl.h>

/* Задает предпочтительный режим обращения к файлу
 * fd - дескриптор файла
 * offset - смещение
 * len - длина
 * advice - режим доступа (POSIX_FADV_*) */
int posix_fadvise(int fd, off_t offset, off_t len, int advice);

#include <stdio.h>

/* Возвращает дескриптор файла для потока
 * stream - указатель на поток */
int fileno(FILE *stream);

/* Создает поток на основе дескриптора файла
 * fd - дескриптор файла
 * mode - режим открытия ("r", "w" и т.д.) */
FILE *fdopen(int fd, const char *mode);

/*
Вот несколько простых примеров использования функций для управления буферизацией и синхронизацией ввода-вывода в C:

### Пример 1: Изменение режима буферизации потока
Функция `setvbuf()` позволяет изменить режим буферизации для потока. В этом примере поток файла переводится в режим без буферизации.
*/
#include <stdio.h>

int main() {
    FILE *file = fopen("example.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    // Установить режим без буферизации
    setvbuf(file, NULL, _IONBF, 0);
    fprintf(file, "This text is written without buffering.\n");

    fclose(file);
    return 0;
}


/*
### Пример 2: Сброс буфера в файл с помощью `fflush()`
Если поток используется в режиме буферизации, можно вручную сбросить буфер в файл, не закрывая его.

*/
#include <stdio.h>

int main() {
    FILE *file = fopen("example.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    fprintf(file, "Buffered text.\n");
    fflush(file);  // Сбросить данные из буфера в файл

    fprintf(file, "More buffered text.\n");

    fclose(file);
    return 0;
}


/*
### Пример 3: Принудительная запись данных с помощью `fsync()`
Функция `fsync()` используется для записи данных на диск, обеспечивая, что они не останутся в системном кэше.
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("example.txt", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        return 1;
    }

    write(fd, "Data to be synced.\n", 19);
    fsync(fd);  // Принудительно записать данные на диск

    close(fd);
    return 0;
}

/*
### Пример 4: Рекомендация ядру для работы с файлом с `posix_fadvise()`
Функция `posix_fadvise()` позволяет дать ядру рекомендации для оптимизации доступа к файлу. Например, можно указать, что файл будет использоваться только один раз.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("example.txt", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return 1;
    }

    // Сообщить ядру, что файл будет прочитан только один раз
    posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED);

    // Чтение данных из файла
    char buffer[100];
    read(fd, buffer, sizeof(buffer));

    close(fd);
    return 0;
}




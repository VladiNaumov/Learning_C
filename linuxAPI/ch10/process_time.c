/* Этот код демонстрирует использование функций `clock()` и `times()` для получения процессорного времени, используемого текущим процессом (виртуального времени процесса). 
Программа выполняет большое количество вызовов `getppid()`, чтобы измерить, сколько процессорного времени потребляет выполнение этих вызовов. */

/* process_time.c

   Демонстрация использования clock(3) и times(2) для получения времени работы процесса.

   Использование: process_time [num-calls]

   Выполняет 'num-calls' вызовов getppid(), а затем отображает время работы процесса.
*/

#include <sys/times.h>
#include <time.h>
#include "tlpi_hdr.h"

/* Функция отображает сообщение 'msg' и время работы процесса */
static void displayProcessTimes(const char *msg)
{
    struct tms t;
    clock_t clockTime;
    static long clockTicks = 0;

    if (msg != NULL)
        printf("%s", msg);

    // Получаем количество тактов в секунду при первом вызове
    if (clockTicks == 0) {
        clockTicks = sysconf(_SC_CLK_TCK);
        if (clockTicks == -1)
            errExit("sysconf");
    }

    // Вызываем clock() для получения времени в тиках
    clockTime = clock();
    if (clockTime == -1)
        errExit("clock");

    printf("        clock() возвращает: %ld тиков за секунду (%.2f сек)\n",
            (long) clockTime, (double) clockTime / CLOCKS_PER_SEC);

    // Вызываем times() для получения времени работы процесса
    if (times(&t) == -1)
        errExit("times");
    printf("        times() возвращает: user CPU=%.2f; system CPU: %.2f\n",
            (double) t.tms_utime / clockTicks,
            (double) t.tms_stime / clockTicks);
}

int main(int argc, char *argv[])
{
    int numCalls, j;

    printf("CLOCKS_PER_SEC=%ld  sysconf(_SC_CLK_TCK)=%ld\n\n",
            (long) CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));

    displayProcessTimes("В начале программы:\n");

    /* Вызов getppid() несколько раз, чтобы потребить
       некоторое количество времени CPU */

    numCalls = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-calls") : 100000000;
    for (j = 0; j < numCalls; j++)
        (void) getppid();

    displayProcessTimes("После цикла getppid():\n");

    exit(EXIT_SUCCESS);
}

/*

### Пояснение
1. **displayProcessTimes()**:
   - Отображает сообщение `msg` и текущее процессорное время, потребленное программой. Использует `clock()` для общего времени и `times()` для разбивки на пользовательское и системное время.
   
2. **clock()**:
   - Возвращает количество тактов (или "тик") с момента запуска программы. Для получения времени в секундах это значение делится на `CLOCKS_PER_SEC`.

3. **times()**:
   - Возвращает структуру `tms`, содержащую время работы в пользовательском режиме (`tms_utime`) и в системном (`tms_stime`). Эти значения также делятся на количество тактов в секунду (`clockTicks`) для перевода в секунды.

4. **Цикл вызова getppid()**:
   - Выполняется заданное количество раз для загрузки CPU. Функция `getppid()` быстро выполняется, но ее многократное выполнение позволяет измерить процессорное время, затрачиваемое на выполнение операций.

Этот код полезен для измерения нагрузки на процессор и оценки, сколько процессорного времени использует программа в разных состояниях.
*/
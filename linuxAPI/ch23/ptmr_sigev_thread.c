/* ptmr_sigev_thread.c

   Программа демонстрирует использование потоков как механизма уведомлений
   для срабатывания POSIX-таймеров. Каждый аргумент командной строки задает
   начальное значение и интервал для таймера. Формат аргументов описан в функции
   itimerspecFromStr().

   Программа создает и активирует таймер для каждого аргумента командной строки.
   Таймеры настроены на отправку уведомлений через поток (SIGEV_THREAD), который
   вызывает функцию threadFunc(). Эта функция отображает информацию о срабатывании
   таймера, увеличивает глобальный счетчик срабатываний таймера и посылает
   сигнал условия для уведомления главного потока о том, что счетчик изменился.
   Главный поток в цикле ждет сигнал условия и выводит значение глобального
   счетчика при каждом уведомлении.

   Поддержка таймеров в ядре Linux добавлена с версии 2.6.
*/
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include "curr_time.h"              /* Декларация currTime() */
#include "tlpi_hdr.h"
#include "itimerspec_from_str.h"    /* Декларация itimerspecFromStr() */

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int expireCnt = 0;           /* Счетчик срабатываний всех таймеров */

static void                         /* Функция уведомления через поток */
threadFunc(union sigval sv)
{
    timer_t *tidptr;
    int s;

    tidptr = sv.sival_ptr;

    printf("[%s] Thread notify\n", currTime("%T"));
    printf("    timer ID=%ld\n", (long) *tidptr);
    printf("    timer_getoverrun()=%d\n", timer_getoverrun(*tidptr));

    /* Увеличиваем счетчик и сигнализируем об изменении главному потоку */

    s = pthread_mutex_lock(&mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_lock");

    expireCnt += 1 + timer_getoverrun(*tidptr);

    s = pthread_mutex_unlock(&mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_unlock");

    s = pthread_cond_signal(&cond);
    if (s != 0)
        errExitEN(s, "pthread_cond_signal");
}

int main(int argc, char *argv[])
{
    struct sigevent sev;
    struct itimerspec ts;
    timer_t *tidlist;
    int s, j;

    if (argc < 2)
        usageErr("%s secs[/nsecs][:int-secs[/int-nsecs]]...\n", argv[0]);

    tidlist = calloc(argc - 1, sizeof(timer_t));
    if (tidlist == NULL)
        errExit("malloc");

    sev.sigev_notify = SIGEV_THREAD;            /* Уведомление через поток */
    sev.sigev_notify_function = threadFunc;     /* Функция потока */
    sev.sigev_notify_attributes = NULL;
            /* Мог быть указатель на структуру pthread_attr_t */

    /* Создаем и запускаем таймер для каждого аргумента */

    for (j = 0; j < argc - 1; j++) {
        itimerspecFromStr(argv[j + 1], &ts);

        sev.sigev_value.sival_ptr = &tidlist[j];
                /* Передается в качестве аргумента в threadFunc() */

        if (timer_create(CLOCK_REALTIME, &sev, &tidlist[j]) == -1)
            errExit("timer_create");
        printf("Timer ID: %ld (%s)\n", (long) tidlist[j], argv[j + 1]);

        if (timer_settime(tidlist[j], 0, &ts, NULL) == -1)
            errExit("timer_settime");
    }

    /* Главный поток ждет на переменной условия, чтобы вывести значение
       счетчика при каждом уведомлении из threadFunc(). */

    s = pthread_mutex_lock(&mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_lock");

    for (;;) {
        s = pthread_cond_wait(&cond, &mtx);
        if (s != 0)
            errExitEN(s, "pthread_cond_wait");
        printf("main(): expireCnt = %d\n", expireCnt);
    }
}
/*

### Объяснение
1. **Создание таймеров**: Программа создает и активирует один таймер для каждого аргумента командной строки, используя `timer_create()`.
2. **threadFunc()**: Потоковая функция вызывается по каждому срабатыванию таймера. Она увеличивает глобальный счетчик `expireCnt`, сигнализирует переменную условия для уведомления главного потока о том, что счетчик изменился.
3. **Главный поток**: Ожидает на условной переменной `cond`, и при каждом срабатывании таймера выводит значение счетчика `expireCnt`.
*/
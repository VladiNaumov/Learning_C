/* 21. Timers and Sleeping */


/* Time-related Functions - Timers, Clocks, and Sleep */

#include <sys/time.h>

/*
 * setitimer - устанавливает таймер, который будет отправлять сигнал по истечении времени.
 * 
 * which - тип таймера (например, ITIMER_REAL, ITIMER_VIRTUAL, ITIMER_PROF).
 * new_value - новая структура itimerval, содержащая значения для интервала и времени до первого срабатывания.
 * old_value - структура, куда сохраняются старые значения таймера.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);

#include <sys/time.h>

/*
 * getitimer - получает текущее состояние таймера.
 * 
 * which - тип таймера.
 * curr_value - структура, в которую сохраняются текущее состояние таймера.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int getitimer(int which, struct itimerval *curr_value);

#include <unistd.h>

/*
 * alarm - задает таймер для сигнализации после указанного времени.
 * 
 * seconds - количество секунд до получения сигнала SIGALRM.
 * 
 * Возвращает оставшееся количество секунд до сигнала, если был активен предыдущий таймер.
 */
unsigned int alarm(unsigned int seconds);

#include <unistd.h>

/*
 * sleep - приостанавливает выполнение процесса на указанное количество секунд.
 * 
 * seconds - количество секунд для сна.
 * 
 * Возвращает оставшееся количество секунд, если процесс был прерван сигналом.
 */
unsigned int sleep(unsigned int seconds);

#define _POSIX_C_SOURCE 199309
#include <time.h>

/*
 * nanosleep - приостанавливает выполнение процесса на заданное количество времени (в наносекундах).
 * 
 * request - структура timespec, задающая время сна.
 * remain - структура, куда сохраняются оставшееся время сна, если оно было прервано.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int nanosleep(const struct timespec *request, struct timespec *remain);

#define _POSIX_C_SOURCE 199309
#include <time.h>

/*
 * clock_gettime - получает текущее время для заданного источника времени.
 * 
 * clockid - идентификатор источника времени (например, CLOCK_REALTIME, CLOCK_MONOTONIC).
 * tp - структура timespec, куда сохраняется текущее время.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int clock_gettime(clockid_t clockid, struct timespec *tp);

/*
 * clock_getres - получает разрешение (точность) для заданного источника времени.
 * 
 * clockid - идентификатор источника времени.
 * res - структура timespec, куда сохраняется разрешение источника времени.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int clock_getres(clockid_t clockid, struct timespec *res);

#define _POSIX_C_SOURCE 199309
#include <time.h>

/*
 * clock_settime - задает новое время для заданного источника времени.
 * 
 * clockid - идентификатор источника времени.
 * tp - структура timespec, задающая новое время.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int clock_settime(clockid_t clockid, const struct timespec *tp);

#define _XOPEN_SOURCE 600
#include <time.h>

/*
 * clock_getcpuclockid - получает идентификатор времени процессора для указанного процесса.
 * 
 * pid - идентификатор процесса.
 * clockid - структура, куда сохраняется идентификатор времени процессора.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int clock_getcpuclockid(pid_t pid, clockid_t *clockid);

#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <time.h>

/*
 * pthread_getcpuclockid - получает идентификатор времени процессора для указанного потока.
 * 
 * thread - идентификатор потока.
 * clockid - структура, куда сохраняется идентификатор времени процессора.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int pthread_getcpuclockid(pthread_t thread, clockid_t *clockid);

#define _XOPEN_SOURCE 600
#include <time.h>

/*
 * clock_nanosleep - приостанавливает выполнение потока на заданное количество времени (с точностью до наносекунд) для указанного источника времени.
 * 
 * clockid - идентификатор источника времени.
 * flags - флаги, которые могут изменить поведение функции.
 * request - структура timespec, задающая время сна.
 * remain - структура, куда сохраняются оставшееся время сна, если оно было прервано.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int clock_nanosleep(clockid_t clockid, int flags, const struct timespec *request, struct timespec *remain);

#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <time.h>

/*
 * timer_create - создает таймер, который может использоваться для отправки сигналов по истечении времени.
 * 
 * clockid - идентификатор источника времени.
 * evp - структура sigevent, задающая параметры уведомления при срабатывании таймера.
 * timerid - структура, куда сохраняется идентификатор созданного таймера.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid);

#define _POSIX_C_SOURCE 199309
#include <time.h>

/*
 * timer_settime - устанавливает время для таймера, который будет срабатывать по истечении заданного интервала.
 * 
 * timerid - идентификатор таймера.
 * flags - флаги, управляющие поведением таймера.
 * value - структура itimerspec, задающая интервал и время срабатывания таймера.
 * old_value - структура, куда сохраняются старые значения таймера.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int timer_settime(timer_t timerid, int flags, const struct itimerspec *value, struct itimerspec *old_value);

/*
 * timer_gettime - получает текущее состояние таймера.
 * 
 * timerid - идентификатор таймера.
 * curr_value - структура itimerspec, куда сохраняются текущее состояние таймера.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int timer_gettime(timer_t timerid, struct itimerspec *curr_value);

#define _POSIX_C_SOURCE 199309
#include <time.h>

/*
 * timer_delete - удаляет таймер, освобождая связанные ресурсы.
 * 
 * timerid - идентификатор таймера.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int timer_delete(timer_t timerid);

/*
 * timer_getoverrun - получает количество пропущенных срабатываний таймера.
 * 
 * timerid - идентификатор таймера.
 * 
 * Возвращает количество пропущенных срабатываний.
 */
int timer_getoverrun(timer_t timerid);

#include <sys/timerfd.h>

/*
 * timerfd_create - создает файловый дескриптор для работы с таймером.
 * 
 * clockid - идентификатор источника времени.
 * flags - флаги для управления поведением таймера.
 * 
 * Возвращает файловый дескриптор или -1 в случае ошибки.
 */
int timerfd_create(int clockid, int flags);

#include <sys/timerfd.h>

/*
 * timerfd_settime - устанавливает время для таймера, работающего с файловым дескриптором.
 * 
 * fd - файловый дескриптор таймера.
 * flags - флаги для настройки таймера.
 * new_value - структура itimerspec, задающая новое время для таймера.
 * old_value - структура, куда сохраняются старые значения таймера.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);

#include <sys/timerfd.h>

/*
 * timerfd_gettime - получает текущее состояние таймера, работающего с файловым дескриптором.
 * 
 * fd - файловый дескриптор таймера.
 * curr_value - структура itimerspec, куда сохраняются текущее состояние таймера.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int timerfd_gettime(int fd, struct itimerspec *curr_value);


#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <unistd.h>

// Устаревшие функции и их альтернативы

/* Устаревшие функции для работы с сигналами */

/* sigvec - устаревшая функция для установки обработчика сигнала (BSD) */
#define _BSD_SOURCE
#include <signal.h>
// Альтернатива: sigaction
// void (*sigvec(int sig, struct sigvec *vec, struct sigvec *ovec))(int);
// Используйте sigaction вместо sigvec:
int sigaction(int sig, const struct sigaction *act, struct sigaction *oldact);


/* sigset - устаревшая функция для установки обработчика сигнала (X/Open) */
#define _XOPEN_SOURCE 500
#include <signal.h>
// Альтернатива: signal или sigaction
// void (*sigset(int sig, void (*handler)(int)))(int);
// Используйте signal или sigaction вместо sigset:
void (*signal(int sig, void (*handler)(int)))(int);
int sigaction(int sig, const struct sigaction *act, struct sigaction *oldact);


/* sighold, sigrelse, sigignore, sigpause - устаревшие функции для работы с сигналами (X/Open) */
#define _XOPEN_SOURCE 500
#include <signal.h>
// Альтернатива: sigprocmask
// int sighold(int sig); // Устарела
// int sigrelse(int sig); // Устарела
// int sigignore(int sig); // Устарела
// int sigpause(int sig); // Устарела
// Вместо них используйте sigprocmask для блокировки и изменения маски сигналов:
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);


/* Устаревшие функции для работы с таймерами */

/* setitimer, getitimer - устаревшие функции для работы с интервальными таймерами */
#include <sys/time.h>
// Альтернатива: nanosleep или timerfd_create (для Linux)
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);  // Устарела
int getitimer(int which, struct itimerval *curr_value);  // Устарела
// Используйте nanosleep или timerfd_create для более точного контроля времени:

int nanosleep(const struct timespec *request, struct timespec *remain);  // Альтернатива setitimer

/* timer_create, timer_settime, timer_gettime - старые альтернативы для работы с таймерами */
#include <time.h>
// Альтернатива: timerfd_create для работы с таймерами через файловые дескрипторы
timer_t timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid);  // Устарела
int timer_settime(timer_t timerid, int flags, const struct itimerspec *value, struct itimerspec *old_value);  // Устарела
int timer_gettime(timer_t timerid, struct itimerspec *curr_value);  // Устарела
// Вместо этого используйте:
int timerfd_create(int clockid, int flags);  // Для создания таймера
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);  // Для установки времени
int timerfd_gettime(int fd, struct itimerspec *curr_value);  // Для получения времени


/* Использование сигнала вместо старых устаревших функций */
int sigqueue(pid_t pid, int sig, const union sigval value);  // Новая альтернатива для сигнальной очереди

/* Альтернативы для работы с асинхронными событиями */
int timerfd_create(int clockid, int flags);  // Для создания файлового дескриптора таймера
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);  // Для установки времени таймера
int timerfd_gettime(int fd, struct itimerspec *curr_value);  // Для получения времени таймера

/* Использование sigsuspend, sigwaitinfo и sigtimedwait для ожидания сигналов с блокировкой */
#include <signal.h>
#include <time.h>
int sigsuspend(const sigset_t *mask);  // Ожидание сигналов
int sigwaitinfo(const sigset_t *set, siginfo_t *info);  // Ожидание информации о сигнале
int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout);  // Ожидание с тайм-аутом

// Используйте более современные методы для работы с сигналами и таймерами вместо устаревших

#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <unistd.h>

// Устаревшие функции и их альтернативы

/* Устаревшие функции для работы с сигналами */

/* sigvec - устаревшая функция для установки обработчика сигнала (BSD) */
#define _BSD_SOURCE
#include <signal.h>
// Альтернатива: sigaction
// void (*sigvec(int sig, struct sigvec *vec, struct sigvec *ovec))(int);
// Используйте sigaction вместо sigvec:
int sigaction(int sig, const struct sigaction *act, struct sigaction *oldact);


/* sigset - устаревшая функция для установки обработчика сигнала (X/Open) */
#define _XOPEN_SOURCE 500
#include <signal.h>
// Альтернатива: signal или sigaction
// void (*sigset(int sig, void (*handler)(int)))(int);
// Используйте signal или sigaction вместо sigset:
void (*signal(int sig, void (*handler)(int)))(int);
int sigaction(int sig, const struct sigaction *act, struct sigaction *oldact);


/* sighold, sigrelse, sigignore, sigpause - устаревшие функции для работы с сигналами (X/Open) */
#define _XOPEN_SOURCE 500
#include <signal.h>
// Альтернатива: sigprocmask
// int sighold(int sig); // Устарела
// int sigrelse(int sig); // Устарела
// int sigignore(int sig); // Устарела
// int sigpause(int sig); // Устарела
// Вместо них используйте sigprocmask для блокировки и изменения маски сигналов:
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);


/* Устаревшие функции для работы с таймерами */

/* setitimer, getitimer - устаревшие функции для работы с интервальными таймерами */
#include <sys/time.h>
// Альтернатива: nanosleep или timerfd_create (для Linux)
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);  // Устарела
int getitimer(int which, struct itimerval *curr_value);  // Устарела
// Используйте nanosleep или timerfd_create для более точного контроля времени:

int nanosleep(const struct timespec *request, struct timespec *remain);  // Альтернатива setitimer

/* timer_create, timer_settime, timer_gettime - старые альтернативы для работы с таймерами */
#include <time.h>
// Альтернатива: timerfd_create для работы с таймерами через файловые дескрипторы
timer_t timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid);  // Устарела
int timer_settime(timer_t timerid, int flags, const struct itimerspec *value, struct itimerspec *old_value);  // Устарела
int timer_gettime(timer_t timerid, struct itimerspec *curr_value);  // Устарела
// Вместо этого используйте:
int timerfd_create(int clockid, int flags);  // Для создания таймера
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);  // Для установки времени
int timerfd_gettime(int fd, struct itimerspec *curr_value);  // Для получения времени


/* Использование сигнала вместо старых устаревших функций */
int sigqueue(pid_t pid, int sig, const union sigval value);  // Новая альтернатива для сигнальной очереди

/* Альтернативы для работы с асинхронными событиями */
int timerfd_create(int clockid, int flags);  // Для создания файлового дескриптора таймера
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);  // Для установки времени таймера
int timerfd_gettime(int fd, struct itimerspec *curr_value);  // Для получения времени таймера

/* Использование sigsuspend, sigwaitinfo и sigtimedwait для ожидания сигналов с блокировкой */
#include <signal.h>
#include <time.h>
int sigsuspend(const sigset_t *mask);  // Ожидание сигналов
int sigwaitinfo(const sigset_t *set, siginfo_t *info);  // Ожидание информации о сигнале
int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout);  // Ожидание с тайм-аутом

/*

### Пояснения:
1. **`sigvec`, `sigset`, `sighold`, `sigrelse`, `sigignore`, `sigpause`** — устаревшие функции, предлагается использовать **`sigaction`**, **`signal`**, и **`sigprocmask`** для обработки сигналов.
2. **`setitimer`**, **`getitimer`** — устаревшие функции для управления интервальными таймерами, предлагается использовать **`nanosleep`** и **`timerfd_create`**.
3. **`timer_create`, `timer_settime`, `timer_gettime`** — старые функции для работы с таймерами, предлагается использовать **`timerfd_create`** и **`timerfd_settime`** для более точного и современного управления временем с использованием файловых дескрипторов.
4. **`sigsuspend`**, **`sigwaitinfo`**, **`sigtimedwait`** — альтернативы для более точного контроля над сигналами.

Этот код демонстрирует, как устаревшие функции могут быть заменены более современными и гибкими методами для работы с сигналами и таймерами.

*/

				  
				  
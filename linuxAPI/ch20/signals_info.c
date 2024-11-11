/* 20 Signals */

#include <signal.h>

/*
 * signal - устанавливает обработчик для сигнала.
 * 
 * sig - номер сигнала (например, SIGINT, SIGTERM и т.д.).
 * handler - указатель на функцию-обработчик, которая будет вызвана при получении сигнала.
 * 
 * Возвращает указатель на предыдущий обработчик для данного сигнала.
 */
void (*signal(int sig, void (*handler)(int)))(int);

/*
 * kill - посылает сигнал процессу.
 * 
 * pid - идентификатор процесса, которому посылается сигнал. Если pid == 0, сигнал отправляется всем процессам в группе.
 * sig - номер сигнала.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int kill(pid_t pid, int sig);

/*
 * raise - посылает сигнал текущему процессу.
 * 
 * sig - номер сигнала.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int raise(int sig);

/*
 * strsignal - возвращает строковое представление сигнала.
 * 
 * sig - номер сигнала.
 * 
 * Возвращает строку с названием сигнала (например, "SIGINT").
 */
#define _BSD_SOURCE
#include <signal.h>
extern const char *const sys_siglist[];

/*
 * strsignal - возвращает строковое представление сигнала (с дополнительными возможностями GNU).
 * 
 * sig - номер сигнала.
 * 
 * Возвращает строку с названием сигнала.
 */
#define _GNU_SOURCE
#include <string.h>
char *strsignal(int sig);

/*
 * psignal - выводит сообщение с текстом, связанным с сигналом.
 * 
 * sig - номер сигнала.
 * msg - строка с сообщением.
 * 
 * Функция выводит сообщение в стандартный поток ошибок, при этом также указывается текстовое описание сигнала.
 */
#include <signal.h>
void psignal(int sig, const char *msg);

/*
 * sigemptyset - инициализирует множество сигналов как пустое.
 * 
 * set - указатель на множество сигналов.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
#include <signal.h>
int sigemptyset(sigset_t *set);

/*
 * sigfillset - инициализирует множество сигналов как полное (все сигналы включены).
 * 
 * set - указатель на множество сигналов.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int sigfillset(sigset_t *set);

/*
 * sigaddset - добавляет сигнал в множество сигналов.
 * 
 * set - указатель на множество сигналов.
 * sig - номер сигнала, который добавляется.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
#include <signal.h>
int sigaddset(sigset_t *set, int sig);

/*
 * sigdelset - удаляет сигнал из множества сигналов.
 * 
 * set - указатель на множество сигналов.
 * sig - номер сигнала, который удаляется.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int sigdelset(sigset_t *set, int sig);

/*
 * sigismember - проверяет, включен ли сигнал в множество.
 * 
 * set - указатель на множество сигналов.
 * sig - номер сигнала.
 * 
 * Возвращает 1, если сигнал включен, и 0, если нет.
 */
#include <signal.h>
int sigismember(const sigset_t *set, int sig);

/*
 * sigandset - выполняет побитовую операцию "И" для двух множеств сигналов.
 * 
 * set - указатель на результат.
 * left - указатель на левое множество.
 * right - указатель на правое множество.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
#define _GNU_SOURCE
#include <signal.h>
int sigandset(sigset_t *set, sigset_t *left, sigset_t *right);

/*
 * sigorset - выполняет побитовую операцию "ИЛИ" для двух множеств сигналов.
 * 
 * set - указатель на результат.
 * left - указатель на левое множество.
 * right - указатель на правое множество.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int sigorset(sigset_t *dest, sigset_t *left, sigset_t *right);

/*
 * sigisemptyset - проверяет, является ли множество сигналов пустым.
 * 
 * set - указатель на множество сигналов.
 * 
 * Возвращает 1, если множество пусто, и 0 в противном случае.
 */
int sigisemptyset(const sigset_t *set);

/*
 * sigprocmask - изменяет маску сигналов для текущего процесса.
 * 
 * how - определяет, как должна изменяться маска сигналов (SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK).
 * set - указатель на множество сигналов, которое будет использоваться.
 * oldset - указатель на старую маску сигналов (можно передать NULL).
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
#include <signal.h>
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

/*
 * sigpending - получает множество сигналов, которые в данный момент ожидают обработки.
 * 
 * set - указатель на множество сигналов.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int sigpending(sigset_t *set);

/*
 * sigaction - устанавливает обработчик для сигнала с возможностью сохранения старого обработчика.
 * 
 * sig - номер сигнала.
 * act - указатель на структуру sigaction с новым обработчиком.
 * oldact - указатель на структуру sigaction для сохранения старого обработчика.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
#include <signal.h>
int sigaction(int sig, const struct sigaction *act, struct sigaction *oldact);

/*
 * pause - приостанавливает выполнение текущего процесса до получения сигнала.
 * 
 * Функция возвращает -1 в случае ошибки.
 */
#include <unistd.h>
int pause(void);

/*
 Вот обновленный текст с добавленными пунктами:

Все эти функции работают с сигналами в Unix-подобных операционных системах.  
Сигналы представляют собой механизм асинхронной обработки событий, которые могут быть использованы для управления процессами.  
Некоторые функции позволяют устанавливать обработчики сигналов, посылать сигналы между процессами, манипулировать множествами сигналов  
(для блокировки или проверки наличия сигналов) и ожидать получения сигналов. Основной целью является эффективное управление асинхронными событиями в многозадачных системах.  

Дополнительно, функции позволяют:

1. **Управлять приоритетом обработки**: Например, с помощью `sigprocmask` можно временно блокировать сигналы, чтобы предотвратить их обработку в критические моменты выполнения программы, управляя приоритетом их обработки.

2. **Контролировать обработчики сигналов**: Функции вроде `signal` и `sigaction` обеспечивают гибкость в управлении обработчиками сигналов, позволяя устанавливать и сохранять старые обработчики, что полезно при сложной логике обработки сигналов.

3. **Использовать функции в зависимости от стандартов**: Некоторые функции, такие как `strsignal`, имеют расширения для разных стандартов (например, `_BSD_SOURCE`, `_GNU_SOURCE`), что дает дополнительные возможности в зависимости от требований проекта и системы.

Таким образом, эти функции позволяют не только управлять асинхронной обработкой сигналов, но и обеспечивают гибкость в их блокировке, приоритизации и настройке совместимости.
 */




/* 21 Signals: Signal Handlers (обработчики сигналов) */

#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int sigdelset(sigset_t *set, int sig);  // Удаляет сигнал из множества

/* Функция sigsetjmp сохраняет состояние выполнения в буфер
 * 
 * env - буфер, в котором сохраняется состояние выполнения.
 * savesigs - если ненулевое, сохраняются блокировки сигналов на момент вызова.
 * 
 * Возвращает 0 при первом вызове, и значение, переданное в siglongjmp(), при возвращении
 * через siglongjmp().
 */
int sigsetjmp(sigjmp_buf env, int savesigs) {
    // Сохраняет контекст выполнения, если savesigs ненулевое, то сохраняет блокировки сигналов
    return 0;
}

/* Функция siglongjmp восстанавливает состояние выполнения
 * 
 * env - буфер, в котором хранится сохранённое состояние.
 * val - значение, которое будет возвращено в sigsetjmp() после восстановления состояния.
 * 
 * Возвращает управление в точку, где была вызвана sigsetjmp(), с переданным значением val.
 */
void siglongjmp(sigjmp_buf env, int val) {
    // Восстанавливает сохраненный контекст выполнения, возвращает управление в точку вызова sigsetjmp
}

/* Функция abort завершает программу с ошибкой
 * 
 * Завершаем программу с ошибкой, никогда не возвращает управление.
 * Возвращает управление не может, так как программа завершится немедленно.
 */
void abort(void) {
    // Завершаем программу с ошибкой, никогда не возвращает управление
    exit(1);
}

/* Функция sigaltstack устанавливает альтернативный стек для сигналов
 * 
 * sigstack - указатель на структуру stack_t, которая описывает новый стек для сигналов.
 * old_sigstack - указатель на структуру stack_t, в которую будет записан старый стек, если он был.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int sigaltstack(const stack_t *sigstack, stack_t *old_sigstack) {
    // Устанавливаем альтернативный стек для обработки сигналов
    // sigstack — новый стек для сигналов, old_sigstack — старый стек, если он был
    return 0;
}

/* Функция siginterrupt управляет прерыванием системных вызовов сигналами
 * 
 * sig - сигнал, который будет использоваться для управления прерыванием системных вызовов.
 * flag - флаг, если ненулевое значение, системные вызовы не будут прерываться этим сигналом.
 * 
 * Возвращает 0 в случае успеха или -1 в случае ошибки.
 */
int siginterrupt(int sig, int flag) {
    // Управляем прерыванием системных вызовов для заданного сигнала
    // sig — сигнал, flag — флаг (если ненулевое значение — вызовы не будут прерываться этим сигналом)
    return 0;
}

/*
Эти функции работают в контексте обработки сигналов и управления выполнением программы при возникновении сигналов. 
Они полезны для обеспечения устойчивости программы в многозадачных средах, где сигналы могут прерывать выполнение операций.
*/

/* 22 Signals: Advanced Features */

#define _GNU_SOURCE
#include <signal.h>
void ( *sysv_signal(int  sig , void (* handler )(int)) ) (int);

#define _POSIX_C_SOURCE 199309
#include <signal.h>
int sigqueue(pid_t  pid , int  sig , const union sigval  value );

#include <signal.h>
int sigsuspend(const sigset_t * mask );

#define _POSIX_C_SOURCE 199309
#include <signal.h>
int sigwaitinfo(const sigset_t * set , siginfo_t * info );

#define _POSIX_C_SOURCE 199309
#include <signal.h>
int sigtimedwait(const sigset_t * set , siginfo_t * info ,
                 const struct timespec * timeout );
				 
#include <sys/signalfd.h>
int signalfd(int  fd , const sigset_t * mask , int  flags );

#define _XOPEN_SOURCE 500
#include <signal.h>
void (*sigset(int  sig , void (* handler )(int)))(int);

#define _XOPEN_SOURCE 500
#include <signal.h>
int sighold(int  sig );
int sigrelse(int  sig );
int sigignore(int  sig );
int sigpause(int  sig );

#define _BSD_SOURCE
#include <signal.h>
int sigvec(int  sig , struct sigvec * vec , struct sigvec * ovec );

#define _BSD_SOURCE
#include <signal.h>
int sigblock(int  mask );
int sigsetmask(int  mask );
Both return previous signal mask
int sigpause(int  sigmask );
Always returns –1 with errno set to  EINTR
int sigmask( sig );

Ваш код содержит несколько функций, связанных с сигналами в Unix-подобных системах. Я добавлю краткие пояснения к каждой из них, а также упомяну некоторые аспекты использования этих функций.

```c
/* 22 Signals: Advanced Features */

#define _GNU_SOURCE
#include <signal.h>
void ( *sysv_signal(int sig, void (*handler)(int)) ) (int);
/* Функция sysv_signal() предоставляет механизм установки обработчика для сигналов в стиле System V.
   Она является устаревшей и была заменена на sigaction(). */

/* Для использования сигнальных функций POSIX */
#define _POSIX_C_SOURCE 199309
#include <signal.h>
int sigqueue(pid_t pid, int sig, const union sigval value);
/* Функция sigqueue() отправляет сигнал с дополнительными данными (value) другому процессу (pid).
   Используется для отправки сигналов с информацией, в отличие от стандартных сигналов. */

/* Функции для блокировки и ожидания сигналов */
int sigsuspend(const sigset_t *mask);
/* Функция sigsuspend() приостанавливает выполнение процесса и ожидает сигнала, который не в маске. */

#define _POSIX_C_SOURCE 199309
#include <signal.h>
int sigwaitinfo(const sigset_t *set, siginfo_t *info);
/* Функция sigwaitinfo() ожидает получение сигнала, который находится в заданном наборе сигналов (set).
   Возвращает информацию о сигнале в структуре siginfo_t. */

/* Функция sigtimedwait() аналогична sigwaitinfo(), но с возможностью задать тайм-аут. */
int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout);
/* Ожидает сигнал из набора с тайм-аутом, после чего либо возвращает информацию о сигнале,
   либо возвращает -1 в случае истечения времени. */

#include <sys/signalfd.h>
int signalfd(int fd, const sigset_t *mask, int flags);
/* Функция signalfd() позволяет использовать файловый дескриптор для блокировки сигналов.
   Позволяет использовать сигналы как обычные события ввода-вывода, что полезно для многозадачных приложений. */

#define _XOPEN_SOURCE 500
#include <signal.h>
void (*sigset(int sig, void (*handler)(int)))(int);
/* Функция sigset() устарела. Она аналогична signal() и позволяет устанавливать обработчик сигнала, но имеет некоторые ограничения. */

/* Функции для управления сигналами (устарели, но могут быть полезны в старых системах) */
#define _XOPEN_SOURCE 500
#include <signal.h>
int sighold(int sig);
/* Устаревшая функция, блокирует сигнал sig. */

int sigrelse(int sig);
/* Разблокирует сигнал sig. */

int sigignore(int sig);
/* Игнорирует сигнал sig. */

int sigpause(int sig);
/* Приостанавливает выполнение процесса до получения сигнала, возвращает -1 с errno = EINTR. */

#define _BSD_SOURCE
#include <signal.h>
int sigvec(int sig, struct sigvec *vec, struct sigvec *ovec);
/* Устаревшая функция для установки обработчиков сигналов в стиле BSD. Рекомендуется использовать sigaction(). */

#define _BSD_SOURCE
#include <signal.h>
int sigblock(int mask);
int sigsetmask(int mask);
/* Функции для блокировки и установки маски сигналов в старых системах. */

/* Другие функции для работы с сигналами */
int sigpause(int sigmask);
/* Функция приостанавливает выполнение процесса с переданной маской сигналов. Возвращает -1 с errno = EINTR. */

/*
### Резюме по функционалу:

1. **sysv_signal()** и **sigset()** — устаревшие функции для установки обработчиков сигналов, обычно заменяются на более современный `sigaction()`.
2. **sigqueue()** — позволяет отправить сигнал с дополнительными данными в процесс.
3. **sigsuspend()**, **sigwaitinfo()**, и **sigtimedwait()** — функции для управления сигналами в процессе выполнения программы.
4. **signalfd()** — позволяет работать с сигналами как с файловыми дескрипторами, что полезно в многозадачных системах.
5. **sighold()**, **sigrelse()**, и **sigignore()** — устаревшие функции для управления блокировкой сигналов.
6. **sigpause()** — используется для приостановки процесса до получения сигнала, и возвращает -1 при прерывании сигнала.

Убедитесь, что используете актуальные и поддерживаемые функции для работы с сигналами, такие как `sigaction()` вместо устаревших методов.

*/




/*


### Современные и устаревшие функции для обработки сигналов

1. **Устаревшие функции**:
   - `sysv_signal()`, `sigset()`, `sigvec()` — эти функции были заменены более мощными и гибкими функциями, такими как `sigaction()`. Они уже не рекомендуются для использования в новых проектах, так как не предоставляют необходимого уровня контроля и надежности, особенно в многозадачных системах.
   
2. **Современные функции**:
   - `sigaction()` — основной инструмент для установки обработчиков сигналов. Он позволяет надежно настроить обработку сигналов с учетом возможности сохранить старый обработчик и использовать флаги, такие как `SA_RESTART` или `SA_SIGINFO`.
   
3. **Функции для отправки сигналов**:
   - `kill()`, `raise()`, `sigqueue()` — стандартные методы для отправки сигналов процессам. `sigqueue()` полезна, когда нужно отправить сигнал с дополнительными данными, например, с структурой `sigval`, которая может содержать целое число или указатель.

4. **Функции для работы с масками сигналов**:
   - `sigprocmask()`, `sigblock()`, `sigsetmask()` — используемые для блокировки сигналов. Эти функции полезны для управления тем, какие сигналы могут быть приняты процессом в конкретный момент времени.
   - `sigsuspend()`, `sigwaitinfo()`, `sigtimedwait()` — позволяют приостанавливать выполнение процесса, ожидая определенные сигналы. Эти функции полезны для синхронизации, особенно при работе с многозадачностью.

5. **Обработчики сигналов**:
   - `signal()`, `sigaction()` — позволяют установить функцию-обработчик для конкретного сигнала. Современный подход предпочитает `sigaction()` из-за его гибкости и возможностей, например, обработки сигнала с дополнительной информацией через `siginfo_t`.

6. **Альтернативный стек**:
   - `sigaltstack()` — позволяет назначить альтернативный стек для обработки сигналов, что может быть полезно для предотвращения переполнения стека в случае обработки слишком глубоких вложенных сигналов (например, при обработке переполнения стека самого процесса).

7. **Прерывание системных вызовов**:
   - `siginterrupt()` — контролирует, будет ли системный вызов прерван сигналом. Это полезно для обеспечения атомарности операций, которые не должны быть прерваны сигналами, например, при блокировке мьютексов.

8. **Специальные функции**:
   - `signalfd()` — позволяет использовать сигналы в качестве событий ввода-вывода, что удобно для работы с событийно-ориентированными архитектурами или в многозадачных приложениях.
   - `sigqueue()`, `sigwaitinfo()`, и `sigtimedwait()` — продвинутые функции для обработки сигналов с возможностью передачи дополнительной информации и установки тайм-аутов.

### Резюме
Большинство старых функций, таких как `signal()` и `sysv_signal()`, теперь считаются устаревшими и не рекомендуются к использованию в новых проектах. Рекомендуется использовать `sigaction()` для установки обработчиков сигналов и функции из POSIX, такие как `sigqueue()`, `sigwaitinfo()`, и `sigsuspend()`, для более сложной работы с сигналами и их блокировкой.

Для работы с многозадачными приложениями и асинхронными событиями использование сигналов остается важной частью архитектуры системы, позволяя управлять процессами, синхронизацией и обработкой исключений.

*/
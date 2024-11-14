/* pdeath_signal.c

   В Linux дочерний процесс может запросить получение сигнала при смерти
   родителя. Однако существуют странности в случае, если родитель
   многопоточный или если существуют процессы-предки с ролью subreaper.

   Пример использования:

        ./pdeath_signal 1 20 @4:+5:6 @3:+2 2
*/
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

/* Структура, определяющая параметры, используемые каждым потоком */

struct threadParam {
    int    sleepTime;          // Время сна потока перед завершением
    char **argv;               // Аргументы командной строки
    int    threadNum;          // Номер текущего потока
    int    ancestorNum;        // Номер предка процесса
    bool   createNextChild;    // Флаг, указывающий, что поток создаст следующего потомка
};

static int childPreSleep, childPostSleep;

static void createAncestor(char **argv, int ancestorNum);

/* Печатает ошибку использования программы и завершает работу */
static void
usageError(char *pname)
{
    fprintf(stderr, "Usage: %s child-pre-sleep "
            "child-post-sleep [ancestor-arg...]\n", pname);
    fprintf(stderr,
"Создаёт цепочку процессов с родительскими связями:\n"
"\n"
"       ancestor1 -> ancestor2 -> ... ancestorN -> child\n"
"\n"
"для изучения поведения параметра prctl() PR_SET_PDEATHSIG.\n"
"\n"
"'child-pre-sleep' — время сна потомка перед установкой PR_SET_PDEATHSIG.\n"
"'child-post-sleep' — время сна потомка после установки PR_SET_PDEATHSIG.\n"
"'ancestor-arg...' задаёт атрибуты для процесса-предка.\n"
"       Для каждого аргумента создаётся один процесс-предок, где\n"
"       первый — самый дальний предок, а последний — непосредственный\n"
"       предок процесса 'child'.\n"
    );
    exit(EXIT_FAILURE);
}

/* Обработчик сигнала "смерти родителя" для дочернего процесса */
static void
handler(int sig, siginfo_t *si, void *ucontext)
{
    static int cnt = 0;

    printf("\n");

    cnt++;
    printf("*********** Потомок (%ld) получил сигнал [кол-во = %d]\n",
            (long) getpid(), cnt);
    printf("\t\tsi_pid = %d; si_uid = %d\n", si->si_pid, si->si_uid);
    printf("\t\tТеперь родительский PID %ld\n\n", (long) getppid());
}

/* Создаёт дочерний процесс, который станет сиротой после завершения всех предков */
static void
createOrphanChild(int ancestorNum)
{
    struct sigaction sa;

    printf("    Поток %ld (PID %ld) вызывает fork()\n",
            syscall(SYS_gettid), (long) getpid());

    switch (fork()) {
    case -1:
        errExit("fork");

    case 0:
        printf("Потомок (PID %ld) создан; родитель %ld\n",
                (long) getpid(), (long) getppid());

        /* Устанавливаем обработчик для сигнала "смерти родителя" */

        sa.sa_flags = SA_SIGINFO;
        sigemptyset(&sa.sa_mask);
        sa.sa_sigaction = handler;
        if (sigaction(SIGUSR1, &sa, NULL) == -1)
            errExit("sigaction");

        /* Пауза перед запросом сигнала "смерти родителя" */
        if (childPreSleep > 0) {
            printf("\tПотомок (PID %ld) спит %d секунд перед PR_SET_PDEATHSIG\n", 
                   (long)getpid(), childPreSleep);
            sleep(childPreSleep);
        }

        /* Запрашиваем отправку сигнала при завершении родителя */

        printf("\tПотомок (PID %ld) устанавливает PR_SET_PDEATHSIG\n", 
               (long)getpid());
        if (prctl(PR_SET_PDEATHSIG, SIGUSR1) == -1)
            errExit("prctl");

        printf("\tПотомок (PID %ld) спит %d секунд\n", 
               (long)getpid(), childPostSleep);
        for (int j = 0; j < childPostSleep; j++)
            sleep(1);

        printf("Потомок завершает работу\n");
        exit(EXIT_SUCCESS);

    default:
        return;
    }
}

/* Выполняет задачи для каждого потока */
static void
performPerThreadSteps(struct threadParam *tparam)
{
    pid_t tid = syscall(SYS_gettid);

    usleep(tparam->threadNum * 1000);

    if (tparam->createNextChild) {
        if (*(tparam->argv) != NULL)
            createAncestor(tparam->argv, tparam->ancestorNum + 1);
        else
            createOrphanChild(tparam->ancestorNum + 1);
    }

    printf("\tПоток %ld (PID %ld; предок: %d, номер: %d) спит %d секунд\n", 
           (long)tid, (long)getpid(), tparam->ancestorNum, tparam->threadNum, 
           tparam->sleepTime);

    sleep(tparam->sleepTime);

    printf("Поток %ld (PID %ld; предок: %d, номер: %d) завершился "
           "(после сна %d секунд)\n",
           (long)tid, (long)getpid(), tparam->ancestorNum, tparam->threadNum, 
           tparam->sleepTime);
}

/* Функция запуска для каждого потока */
static void *
threadStartFunc(void * arg)
{
    struct threadParam *tparam = arg;

    performPerThreadSteps(tparam);

    free(tparam);
    pthread_exit(NULL);
}

/* Создаёт цепочку потоков в вызывающем процессе */
static void
createThreads(char *ancestorArg, char **argv, int ancestorNum)
{
    struct threadParam *tparam;
    struct threadParam tparamInit;
    bool nextParentMarked = false;

    for (int tnum = 0; ; tnum++) {
        char *tokenp = strtok((tnum == 0) ? ancestorArg : NULL, ":");

        if (tokenp == NULL)
            break;

        tparam = malloc(sizeof(struct threadParam));
        if (tparam == NULL)
            errExit("malloc");

        tparam->createNextChild = *tokenp == '+';

        if (tparam->createNextChild) {
            if (nextParentMarked) {
                fprintf(stderr, "Найдено '+' дважды!\n");
                exit(EXIT_FAILURE);
            }
            tokenp++;
            nextParentMarked = true;
        }

        tparam->sleepTime = atoi(tokenp);
        tparam->argv = argv + 1;
        tparam->threadNum = tnum;
        tparam->ancestorNum = ancestorNum;

        if (tnum == 0) {
            tparamInit = *tparam;
            free(tparam);
        } else {
            pthread_t thr;
            int s = pthread_create(&thr, NULL, threadStartFunc, tparam);
            if (s != 0) {
                fprintf(stderr, "Ошибка pthread_create()\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (!nextParentMarked)
        tparamInit.createNextChild = true;

    performPerThreadSteps(&tparamInit);

    pthread_exit(NULL);
}

/* Создаёт цепочку предков с помощью рекурсии */
static void
createAncestor(char **argv, int ancestorNum)
{
    pid_t childPid;

    usleep(10000);

    printf("    Поток %ld (PID %ld) вызывает fork()\n",
            syscall(SYS_gettid), (long)getpid());

    childPid = fork();
    if (childPid == -1)
        errExit("fork");

    if (childPid != 0)
        return;

    printf("Предок %d [PID %ld] создан; родитель %ld\n", ancestorNum,
            (long)getpid(), (long)getppid());

    char *ancestorArg = *argv;

    if (*ancestorArg == '@') {
        if (prctl(PR_SET_CHILD_SUBREAPER, 1) == -1)
            errExit("prctl");
        printf("    *** PID %ld стал субрипером\n",
                (long)getpid());
        ancestorArg++;
    }

    createThreads(ancestorArg, argv, ancestorNum);
}

int
main(int argc, char *argv[])
{
    setbuf(stdout, NULL); // Отключаем буферизацию stdout для немедленного вывода

    if (argc < 3)
        usageError(argv[0]);

    printf("Главная программа с PID %ld запущена\n", (long)getpid());

    childPreSleep = atoi(argv[1]);   // Время сна до PR_SET_PDEATHSIG
    childPostSleep = atoi(argv[2]);  // Время сна после PR_SET_PDEATHSIG

    // Создаем цепочку предков для потомка, передавая параметры из argv
    if (argc > 3) {
        createAncestor(&argv[3], 1);
    } else {
        createOrphanChild(1); // Если предки не заданы, создаем потомка без предков
    }

    // Ожидание завершения дочернего процесса
    int status;
    while (wait(&status) != -1) {
        if (WIFEXITED(status)) {
            printf("Потомок завершился с кодом %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Потомок завершился сигналом %d\n", WTERMSIG(status));
        }
    }

    if (errno != ECHILD)
        errExit("wait");

    printf("Главная программа завершает работу\n");
    exit(EXIT_SUCCESS);
}

/*
### Резюме кода

Этот код демонстрирует работу с системным вызовом `prctl` в Linux, с помощью которого дочерний процесс может получить сигнал при завершении своего родительского процесса. Основная цель программы — изучить, как `PR_SET_PDEATHSIG` реагирует на завершение предков дочернего процесса в различных условиях (например, в цепочке предков или с "суб-ребейперами").

#### Основные шаги программы:
1. **Параметры запуска**: программа принимает аргументы командной строки, чтобы указать задержки сна дочернего процесса до и после настройки `PR_SET_PDEATHSIG`, а также параметры для создания цепочки предков.
  
2. **Создание цепочки процессов и потоков**: программа создает цепочку предков, каждый из которых может состоять из нескольких потоков. Каждый поток может:
   - Засыпать на определенное время.
   - Вызвать `fork`, чтобы создать следующий процесс в цепочке предков или дочерний процесс.

3. **Сигнализация `PR_SET_PDEATHSIG`**: дочерний процесс устанавливает сигнал, который будет получен, если его родитель завершится. Это позволяет дочернему процессу "узнать" о смерти родителя, что может быть полезно для управления ресурсами или завершения работы в системах, где процессы зависят от родителя.

4. **Вывод статусов**: каждый поток и процесс выводит свой идентификатор и статус выполнения. Дочерний процесс обрабатывает сигнал, если родительский процесс завершается, и выводит уведомление.

5. **Завершение работы**: основная программа ждет завершения всех дочерних процессов, после чего выводит финальное сообщение и завершает выполнение.

Этот код позволяет изучить поведение системного вызова `prctl` с флагом `PR_SET_PDEATHSIG` в сложной иерархии процессов и потоков, где один из процессов может быть назначен суб-ребейпером для управления потомками.

*/
/* Чтобы исключить возможность состояния гонки, можно использовать сигналы для синхронизации между родительским и дочерним процессом. Вместо использования `sleep()`, мы можем отправить сигнал из дочернего процесса родителю, когда он завершит свою работу. Родитель будет ждать этот сигнал и только после этого будет вызывать `system()` или завершать свою работу.

Вот как можно изменить программу с использованием сигналов:

1. Родительский процесс будет использовать `sigaction()` для обработки сигнала от дочернего процесса.
2. Дочерний процесс будет отправлять сигнал родителю после завершения своей работы с помощью `kill()`.

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t child_done = 0;

// Обработчик сигнала, который устанавливает флаг, когда дочерний процесс завершился
void child_signal_handler(int sig) {
    child_done = 1;
}

int main() {
    pid_t pid;

    // Регистрация обработчика сигнала для SIGUSR1
    struct sigaction sa;
    sa.sa_handler = child_signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction failed");
        return 1;
    }

    pid = fork(); // Создаем дочерний процесс

    if (pid == -1) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) { // В дочернем процессе
        printf("Child process: PID = %d, Parent PID = %d\n", getpid(), getppid());
        // Здесь завершается работа дочернего процесса
        kill(getppid(), SIGUSR1); // Отправляем сигнал родителю
        exit(0);
    } else { // В родительском процессе
        // Ожидаем сигнал от дочернего процесса
        printf("Parent process: PID = %d\n", getpid());
        while (!child_done) {
            // Ожидаем получения сигнала от дочернего процесса
            pause(); // Ожидаем сигнала
        }
        printf("Child has completed. Parent can now proceed.\n");

        // После получения сигнала родитель может выполнять дальнейшие действия
        // Например, вызвать system() или завершить работу
        system("echo Parent process executing after child completion");
    }

    return 0;
}

/*
### Объяснение:
1. **Регистрация обработчика сигнала:** В родительском процессе используется `sigaction()` для регистрации обработчика сигнала `SIGUSR1`, который будет сигнализировать о завершении дочернего процесса.
2. **Отправка сигнала:** Дочерний процесс, после выполнения своей работы, отправляет сигнал родительскому процессу с помощью `kill(getppid(), SIGUSR1)`.
3. **Ожидание сигнала:** Родительский процесс использует `pause()`, чтобы приостановить выполнение до получения сигнала от дочернего процесса. Когда сигнал поступает, обработчик изменяет флаг `child_done`, и родитель продолжает выполнение.

### Преимущества:
- Исключается использование `sleep()`, что предотвращает состояние гонки.
- Процессы синхронизируются с помощью сигналов, что гарантирует, что родитель выполнит действия (например, вызов `system()`) только после завершения дочернего процесса.

Этот код будет работать на Unix-подобных системах, где поддерживаются сигналы.
 */
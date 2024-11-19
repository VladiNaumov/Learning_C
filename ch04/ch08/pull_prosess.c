#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define POOL_SIZE 4 // Количество процессов в пуле

// Функция, выполняющая задачу
void do_task(int task_id) {
    printf("Процесс %d выполняет задачу %d\n", getpid(), task_id);
    sleep(2); // Симуляция времени работы
}

int main() {
    pid_t pids[POOL_SIZE]; // Массив для хранения PID процессов

    // Создание пула процессов
    for (int i = 0; i < POOL_SIZE; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            // Дочерний процесс
            int task_id = i + 1; // Уникальный ID задачи
            do_task(task_id);
            exit(0); // Завершаем процесс после выполнения задачи
        }
    }

    // Родительский процесс - управление задачами
    for (int i = 0; i < POOL_SIZE; i++) {
        waitpid(pids[i], NULL, 0); // Ожидаем завершения всех процессов
    }

    printf("Все задачи выполнены.\n");
    return 0;
}

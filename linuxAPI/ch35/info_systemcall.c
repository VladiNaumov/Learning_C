/* 35 Process Priorities and Scheduling */

#include <sys/resource.h>

/*
 * getpriority - Получает приоритет планирования процесса, группы процессов или пользователя.
 * which - Тип объекта (PRIO_PROCESS, PRIO_PGRP, PRIO_USER).
 * who - Идентификатор объекта (или 0 для текущего).
 * Возвращает значение приоритета (или -1 при ошибке).
 */
int getpriority(int which, id_t who);

/*
 * setpriority - Устанавливает приоритет планирования процесса, группы процессов или пользователя.
 * which - Тип объекта (PRIO_PROCESS, PRIO_PGRP, PRIO_USER).
 * who - Идентификатор объекта (или 0 для текущего).
 * prio - Новое значение приоритета.
 * Возвращает 0 при успехе или -1 при ошибке.
 */
int setpriority(int which, id_t who, int prio);

#include <sched.h>

/*
 * sched_get_priority_min - Получает минимальный приоритет для указанной политики планирования.
 * policy - Политика (например, SCHED_FIFO, SCHED_RR, SCHED_OTHER).
 * Возвращает минимальный приоритет (или -1 при ошибке).
 */
int sched_get_priority_min(int policy);

/*
 * sched_get_priority_max - Получает максимальный приоритет для указанной политики планирования.
 * policy - Политика (например, SCHED_FIFO, SCHED_RR, SCHED_OTHER).
 * Возвращает максимальный приоритет (или -1 при ошибке).
 */
int sched_get_priority_max(int policy);

/*
 * sched_setscheduler - Устанавливает политику и параметры планирования для процесса.
 * pid - Идентификатор процесса (или 0 для текущего).
 * policy - Новая политика (например, SCHED_FIFO, SCHED_RR, SCHED_OTHER).
 * param - Указатель на структуру с параметрами планирования.
 * Возвращает 0 при успехе или -1 при ошибке.
 */
int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);

/*
 * sched_setparam - Устанавливает параметры планирования для процесса.
 * pid - Идентификатор процесса (или 0 для текущего).
 * param - Указатель на структуру с параметрами планирования.
 * Возвращает 0 при успехе или -1 при ошибке.
 */
int sched_setparam(pid_t pid, const struct sched_param *param);

/*
 * sched_getscheduler - Получает текущую политику планирования процесса.
 * pid - Идентификатор процесса (или 0 для текущего).
 * Возвращает политику (или -1 при ошибке).
 */
int sched_getscheduler(pid_t pid);

/*
 * sched_getparam - Получает текущие параметры планирования процесса.
 * pid - Идентификатор процесса (или 0 для текущего).
 * param - Указатель на структуру для хранения параметров.
 * Возвращает 0 при успехе или -1 при ошибке.
 */
int sched_getparam(pid_t pid, struct sched_param *param);

/*
 * sched_yield - Освобождает процессор, позволяя другим процессам выполнить свои задачи.
 * Возвращает 0 при успехе или -1 при ошибке.
 */
int sched_yield(void);

/*
 * sched_rr_get_interval - Получает интервал времени планирования для процесса с политикой SCHED_RR.
 * pid - Идентификатор процесса (или 0 для текущего).
 * tp - Указатель на структуру timespec для хранения интервала.
 * Возвращает 0 при успехе или -1 при ошибке.
 */
int sched_rr_get_interval(pid_t pid, struct timespec *tp);

#define _GNU_SOURCE

/*
 * sched_setaffinity - Устанавливает привязку процесса к набору процессоров.
 * pid - Идентификатор процесса (или 0 для текущего).
 * len - Размер структуры cpu_set_t.
 * set - Указатель на структуру cpu_set_t с маской процессоров.
 * Возвращает 0 при успехе или -1 при ошибке.
 */
int sched_setaffinity(pid_t pid, size_t len, cpu_set_t *set);

/*
 * CPU_ZERO - Очищает все процессоры в наборе.
 * set - Указатель на структуру cpu_set_t.
 */
void CPU_ZERO(cpu_set_t *set);

/*
 * CPU_SET - Добавляет процессор в набор.
 * cpu - Идентификатор процессора.
 * set - Указатель на структуру cpu_set_t.
 */
void CPU_SET(int cpu, cpu_set_t *set);

/*
 * CPU_CLR - Удаляет процессор из набора.
 * cpu - Идентификатор процессора.
 * set - Указатель на структуру cpu_set_t.
 */
void CPU_CLR(int cpu, cpu_set_t *set);

/*
 * CPU_ISSET - Проверяет, включён ли процессор в набор.
 * cpu - Идентификатор процессора.
 * set - Указатель на структуру cpu_set_t.
 * Возвращает ненулевое значение, если процессор включён, или 0 в противном случае.
 */
int CPU_ISSET(int cpu, cpu_set_t *set);

/*
 * sched_getaffinity - Получает привязку процесса к набору процессоров.
 * pid - Идентификатор процесса (или 0 для текущего).
 * len - Размер структуры cpu_set_t.
 * set - Указатель на структуру cpu_set_t для хранения маски процессоров.
 * Возвращает 0 при успехе или -1 при ошибке.
 */
int sched_getaffinity(pid_t pid, size_t len, cpu_set_t *set);

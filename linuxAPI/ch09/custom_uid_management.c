/*
Нужно написать код, который управляет идентификацией пользователя в процессе с помощью системных вызовов setuid(), seteuid(), setreuid(), и setresuid().
 Эти вызовы управляют реальным (real), действующим (effective), и сохранённым (saved) идентификаторами пользователя, которые определяют права доступа процесса.

Вопрос просит создать две группы операций: real=X effective=Y saved=Y

Временное отключение и восстановление привилегий (как в set-user-ID-программах, где есть разные уровни доступа).

Нужно переключиться на реальный идентификатор пользователя (отключить привилегии), а затем вернуться к сохранённому идентификатору (восстановить привилегии).
Полный сброс привилегий.

Установить как действующий, так и сохранённый идентификаторы равными реальному идентификатору пользователя, чтобы процесс больше не мог восстановить привилегии.

Немного теории:
real ID — идентификатор пользователя, запустившего процесс.
effective ID — определяет права доступа процесса в данный момент.
saved ID — сохранённое значение effective ID, чтобы можно было вернуться к привилегиям позже.



Решение по пунктам:
1. Временное отключение и восстановление привилегий

Для временного отключения привилегий можно сделать:
- Переключаем действующий ID на значение реального ID (отключаем привилегии).
- Когда нужно восстановить, действующий ID переключаем обратно на значение сохранённого ID.

 2. Полный сброс привилегий
Чтобы полностью сбросить привилегии, нужно:
-Установить действующий и сохранённый ID равными реальному ID.
-После этого вернуть привилегии уже нельзя, так как сохранённого значения больше нет.
 */

#include <unistd.h>
#include <stdio.h>

/*1 Для временного отключения привилегий */
void toggle_privileges() {
    uid_t real_uid = getuid();    // Получаем реальный ID
    uid_t effective_uid = geteuid(); // Получаем действующий ID

    // Отключение привилегий: устанавливаем effective ID равным реальному
    if (seteuid(real_uid) == -1) {
        perror("Error dropping privileges");
        return;
    }

    printf("Privileges dropped. Effective UID: %d\n", geteuid());

    // Восстановление привилегий: устанавливаем effective ID равным сохраненному
    if (seteuid(effective_uid) == -1) {
        perror("Error restoring privileges");
        return;
    }

    printf("Privileges restored. Effective UID: %d\n", geteuid());
}

/* 2. Полный сброс привилегий */
void drop_privileges_permanently() {
    uid_t real_uid = getuid();  // Получаем реальный ID

    // Сбрасываем привилегии: устанавливаем real, effective и saved ID равными real UID
    if (setuid(real_uid) == -1) {
        perror("Error permanently dropping privileges");
        return;
    }

    printf("Privileges permanently dropped. Effective UID: %d\n", geteuid());
}

/*
toggle_privileges: временно отключает и возвращает привилегии.
drop_privileges_permanently: полностью сбрасывает привилегии, без возможности восстановления.
Эти функции используют seteuid() для временного изменения привилегий и setuid() для полного сброса.
 */

/* Повторите предыдущее упражнение для процесса выполнения set-user-ID-root-
программы, у которой следующий исходный набор идентификаторов процесса:
real=X effective=0 saved=0 */

/* Теперь процесс является set-user-ID-root-программой, где:

Реальный ID = X (не привилегированный пользователь).
Действующий ID = 0 (root-права).
Сохранённый ID = 0 (root-права).
Нам снова нужно:

Временно отключать и восстанавливать привилегии root.
Полностью сбросить root-привилегии, чтобы процесс больше не имел привилегий root.
Задачи и решения
1. Временное отключение и восстановление привилегий root
Здесь мы временно отключаем root-привилегии, меняя действующий ID на реальный ID (X), а затем восстанавливаем root, возвращая действующий ID к сохранённому ID (0).
2. Полный сброс привилегий root
Для полного сброса root-привилегий нужно:

Установить реальный, действующий, и сохранённый ID равными реальному ID (X).
После этого процесс уже не сможет восстановить привилегии root.
 */

#include <unistd.h>
#include <stdio.h>

// Временное отключение и восстановление root-привилегий
void toggle_root_privileges() {
    uid_t real_uid = getuid();       // Получаем реальный ID (X)
    uid_t effective_uid = geteuid();  // Получаем действующий ID (0)

    // Отключаем привилегии root: effective ID равен реальному ID
    if (seteuid(real_uid) == -1) {
        perror("Error dropping root privileges");
        return;
    }

    printf("Root privileges dropped. Effective UID: %d\n", geteuid());

    // Восстанавливаем привилегии root: effective ID снова равен сохраненному (0)
    if (seteuid(effective_uid) == -1) {
        perror("Error restoring root privileges");
        return;
    }

    printf("Root privileges restored. Effective UID: %d\n", geteuid());
}

// Полный сброс привилегий root
void drop_root_privileges_permanently() {
    uid_t real_uid = getuid();  // Получаем реальный ID (X)

    // Устанавливаем все идентификаторы равными real UID (X), сбрасывая привилегии
    if (setuid(real_uid) == -1) {
        perror("Error permanently dropping root privileges");
        return;
    }

    printf("Root privileges permanently dropped. Effective UID: %d\n", geteuid());
}

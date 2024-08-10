/*
 Observer (Наблюдатель)
Паттерн "Наблюдатель" позволяет объектам оповещать другие объекты об изменениях своего состояния.
В C это может быть реализовано через массив указателей на функции, которые будут вызываться при изменении состояния.
*/

#include <stdio.h>

#define MAX_OBSERVERS 10  // Максимальное количество наблюдателей

// Определяем тип функции-наблюдателя
typedef void (*Observer)();

Observer observers[MAX_OBSERVERS];  // Массив для хранения наблюдателей
int observer_count = 0;  // Счётчик количества наблюдателей

// Функция для добавления нового наблюдателя
void add_observer(Observer obs) {
    if (observer_count < MAX_OBSERVERS) {
        observers[observer_count++] = obs;  // Добавляем наблюдателя в массив
    }
}

// Функция для уведомления всех наблюдателей
void notify_observers() {
    for (int i = 0; i < observer_count; i++) {
        observers[i]();  // Вызов функции каждого наблюдателя
    }
}

// Первый наблюдатель
void observer1() {
    printf("Observer 1 notified!\n");
}

// Второй наблюдатель
void observer2() {
    printf("Observer 2 notified!\n");
}

int main() {
    add_observer(observer1);  // Добавляем первого наблюдателя
    add_observer(observer2);  // Добавляем второго наблюдателя
    notify_observers();  // Уведомляем всех наблюдателей
    return 0;
}

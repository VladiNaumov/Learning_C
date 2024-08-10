/*
Strategy (Стратегия)
Этот паттерн позволяет изменять алгоритмы или поведение программы во время выполнения, заменяя один алгоритм другим.
В C это может быть реализовано через указатели на функции.
*/

#include <stdio.h>

// Определяем тип функции, которая будет использоваться в качестве стратегии
typedef void (*StrategyFunc)();

// Первая стратегия
void strategy1() {
    printf("Executing Strategy 1\n");
}

// Вторая стратегия
void strategy2() {
    printf("Executing Strategy 2\n");
}

// Функция для выполнения стратегии
void execute_strategy(StrategyFunc strategy) {
    strategy();  // Вызов переданной функции (стратегии)
}

int main() {
    execute_strategy(strategy1);  // Выполнение первой стратегии
    execute_strategy(strategy2);  // Выполнение второй стратегии
    return 0;
}

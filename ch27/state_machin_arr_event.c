/*
 * Если необходимо полностью исключить использование if и switch,
 * можно реализовать конечный автомат в C, используя исключительно таблицы переходов.
 * Вместо условий можно использовать двумерный массив, где каждому состоянию и событию соответствует новое состояние.
 * */

#include <stdio.h>

// Определение состояний
typedef enum {
    START,
    STATE1,
    STATE2,
    END,
    NUM_STATES // количество состояний
} State;

// Определение событий
typedef enum {
    EVENT1,
    EVENT2,
    EVENT3,
    NUM_EVENTS // количество событий
} Event;

// Таблица переходов состояний
State transitionTable[NUM_STATES][NUM_EVENTS] = {
        // START
        [START][EVENT1] = STATE1,
        [START][EVENT2] = STATE2,
        [START][EVENT3] = START,  // переход в то же состояние, если событие не обрабатывается

        // STATE1
        [STATE1][EVENT1] = STATE1,
        [STATE1][EVENT2] = STATE1,
        [STATE1][EVENT3] = END,

        // STATE2
        [STATE2][EVENT1] = STATE2,
        [STATE2][EVENT2] = STATE2,
        [STATE2][EVENT3] = END,

        // END
        [END][EVENT1] = END,
        [END][EVENT2] = END,
        [END][EVENT3] = END,
};

// Главная функция обработки событий
State handleEvent(State currentState, Event event) {
    return transitionTable[currentState][event];
}

int main() {
    State currentState = START;

    // Пример последовательности событий
    currentState = handleEvent(currentState, EVENT1);
    printf("Current state: %d\n", currentState); // STATE1

    currentState = handleEvent(currentState, EVENT3);
    printf("Current state: %d\n", currentState); // END

    return 0;
}

/*
 * Объяснение:
Состояния и события: Определены как enum, что позволяет легко работать с индексами массива.
Таблица переходов: Это двумерный массив, где строки представляют текущее состояние, а столбцы — событие. Элемент массива хранит следующее состояние.
Метод handleEvent: Обрабатывает событие, возвращая следующее состояние, соответствующее текущему состоянию и событию.
 * */
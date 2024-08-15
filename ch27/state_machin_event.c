#include <stdio.h>

/*
 * реализации конечного автомата на языке C, используя указатели на функции.
 * В этом примере конечный автомат реализован без использования if и switch, где переходы состояний
 * управляются через таблицу указателей на функции.
 * */

// Определение состояний
typedef enum {
    START,
    STATE1,
    STATE2,
    END,
    NUM_STATES // Количество состояний
} State;

// Определение событий
typedef enum {
    EVENT1,
    EVENT2,
    EVENT3,
    NUM_EVENTS // Количество событий
} Event;

// Прототипы функций переходов состояний
State transitionStart(Event event);
State transitionState1(Event event);
State transitionState2(Event event);
State transitionEnd(Event event);

// Таблица переходов состояний ( State (*transitionTable[NUM_STATES])(Event) представляет собой объявление массива указателей на функции)
State (*transitionTable[NUM_STATES])(Event) = {
        transitionStart,  // START
        transitionState1, // STATE1
        transitionState2, // STATE2
        transitionEnd     // END
};

/*
 * Разбор структуры
State:

Это тип возвращаемого значения функции.
 В данном случае функция, на которую указывает каждый элемент массива, возвращает значение типа State.
(*transitionTable[NUM_STATES]):

transitionTable — это имя массива.
[NUM_STATES] — размер массива, определенный как количество состояний (NUM_STATES).
 Это указывает, что transitionTable — это массив с NUM_STATES элементами.
(* ... ) — означает, что элементы массива являются указателями на функции.
(Event):

Это список параметров функции, на которую указывает указатель. В данном случае функция принимает один параметр типа Event.
 * */

// Функции переходов состояний
State transitionStart(Event event) {
    // Переключаемся на состояние в зависимости от события
    return (event == EVENT1) ? STATE1 :
           (event == EVENT2) ? STATE2 : START;
}

State transitionState1(Event event) {
    // Переключаемся на состояние в зависимости от события
    return (event == EVENT3) ? END : STATE1;
}

State transitionState2(Event event) {
    // Переключаемся на состояние в зависимости от события
    return (event == EVENT3) ? END : STATE2;
}

State transitionEnd(Event event) {
    // В состоянии END не происходит никаких изменений
    return END;
}

// Функция обработки события
State handleEvent(State currentState, Event event) {
    return transitionTable[currentState](event);
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
Состояния и события: Определены с помощью enum. NUM_STATES и NUM_EVENTS определяют количество состояний и событий соответственно.
Функции переходов: Каждое состояние имеет свою функцию, которая принимает событие и возвращает новое состояние. Используются тернарные операторы для определения следующего состояния на основе события.
Таблица переходов: Это массив указателей на функции, где каждый элемент соответствует функции перехода для конкретного состояния.
Функция handleEvent: Вызывает соответствующую функцию перехода из таблицы переходов, передавая текущее состояние и событие, и возвращает новое состояние.
 * */
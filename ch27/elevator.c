#include <stdio.h>

/*
 * Сценарий лифта
    У нас есть лифт, который может находиться в следующих состояниях:

    Idle (Простой): Лифт стоит на месте, никуда не движется.
    Moving Up (Движется вверх): Лифт поднимается на этаж выше.
    Moving Down (Движется вниз): Лифт опускается на этаж ниже.
    Door Open (Дверь открыта): Лифт открыл двери на этаже.
    Лифт может получать следующие события:

    Call Up (Вызов на этаж и движение вверх).
    Call Down (Вызов на этаж и движение вниз).
    Arrived (Лифт прибыл на этаж).
    Open Door (Открыть дверь).
    Close Door (Закрыть дверь).
 * */

// Определение состояний лифта


typedef enum {
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    DOOR_OPEN
} LiftState;

// Определение событий лифта
typedef enum {
    CALL_UP,
    CALL_DOWN,
    ARRIVED,
    OPEN_DOOR,
    CLOSE_DOOR
} LiftEvent;

// Определение структуры для лифта
typedef struct {
    LiftState state;     // Текущее состояние лифта
    int currentFloor;    // Текущий этаж
    int targetFloor;     // Целевой этаж
} Lift;

// Функция для обработки событий
LiftState process_lift_event(Lift *lift, LiftEvent event) {
    switch (lift->state) {
        case IDLE:
            if (event == CALL_UP) {
                lift->state = MOVING_UP;
                printf("Лифт движется вверх к этажу %d.\n", lift->targetFloor);
            } else if (event == CALL_DOWN) {
                lift->state = MOVING_DOWN;
                printf("Лифт движется вниз к этажу %d.\n", lift->targetFloor);
            }
            break;

        case MOVING_UP:
            if (event == ARRIVED) {
                lift->state = DOOR_OPEN;
                printf("Лифт прибыл на этаж %d и открывает двери.\n", lift->currentFloor);
            }
            break;

        case MOVING_DOWN:
            if (event == ARRIVED) {
                lift->state = DOOR_OPEN;
                printf("Лифт прибыл на этаж %d и открывает двери.\n", lift->currentFloor);
            }
            break;

        case DOOR_OPEN:
            if (event == CLOSE_DOOR) {
                lift->state = IDLE;
                printf("Лифт закрывает двери и находится в режиме ожидания.\n");
            }
            break;
    }
    return lift->state;
}

int main() {
    // Инициализация лифта
    Lift myLift = {IDLE, 0, 0};  // Начальное состояние: лифт стоит на 0 этаже

    // Пример последовательности событий
    myLift.targetFloor = 5;
    process_lift_event(&myLift, CALL_UP);  // Вызов лифта на 5 этаж
    myLift.currentFloor = 5;
    process_lift_event(&myLift, ARRIVED);  // Лифт прибыл на 5 этаж
    process_lift_event(&myLift, CLOSE_DOOR);  // Лифт закрывает двери

    myLift.targetFloor = 1;
    process_lift_event(&myLift, CALL_DOWN);  // Вызов лифта на 1 этаж
    myLift.currentFloor = 1;
    process_lift_event(&myLift, ARRIVED);  // Лифт прибыл на 1 этаж
    process_lift_event(&myLift, CLOSE_DOOR);  // Лифт закрывает двери

    return 0;
}

/*
 * Объяснение:
    Состояния лифта:

    IDLE: Лифт в состоянии ожидания, не движется.
    MOVING_UP: Лифт движется вверх.
    MOVING_DOWN: Лифт движется вниз.
    DOOR_OPEN: Лифт остановился и открыл двери.
    События лифта:

    CALL_UP: Вызов лифта для движения вверх.
    CALL_DOWN: Вызов лифта для движения вниз.
    ARRIVED: Лифт прибыл на целевой этаж.
    OPEN_DOOR: Команда на открытие двери (в данном примере не используется напрямую, так как двери открываются автоматически при прибытии на этаж).
    CLOSE_DOOR: Команда на закрытие двери.
    Структура Lift:

    Содержит текущее состояние лифта (state), текущий этаж (currentFloor), и целевой этаж (targetFloor).
    Функция process_lift_event:

    Обрабатывает текущее состояние лифта и полученное событие, изменяя состояние лифта в зависимости от логики автомата.
    Использование в main:

    Инициализация лифта и обработка событий: сначала лифт вызывается на 5 этаж, затем на 1 этаж.
    */
#include <stdio.h>

/*
 * Пример: Турникет в метро
Представьте себе турникет в метро. Он работает как конечный автомат и имеет следующие состояния и переходы:

Состояния:
Закрыт (Closed): Турникет закрыт, и человек не может пройти.
Открыт (Open): Турникет открыт, и человек может пройти.
Входные символы:
Вставка жетона/карты (InsertToken): Человек вставляет жетон или карту.
Проход (Pass): Человек проходит через турникет.
Неверный жетон/карта (InvalidToken): Человек пытается использовать недействительный жетон или карту.
Функции переходов:
Закрыт (Closed):
Вставка жетона/карты (InsertToken) → переходит в состояние Открыт (Open).
Неверный жетон/карта (InvalidToken) → остаётся в состоянии Закрыт (Closed).
Открыт (Open):
Проход (Pass) → возвращается в состояние Закрыт (Closed).
Никаких других событий нет, турникет ждёт прохода.
Начальное состояние:
Турникет начинается в состоянии Закрыт (Closed).
Конечное состояние:
Нет явного конечного состояния, турникет продолжает работать в цикле: закрыт → открыт → закрыт.
Как это работает:
Начальное состояние: Турникет закрыт.
Человек подходит и вставляет жетон. Турникет открывается.
Человек проходит через турникет, и он снова закрывается.
Пример в жизни:
Шаг 1: Турникет закрыт.
Шаг 2: Вы вставляете жетон (событие InsertToken). Турникет открывается.
Шаг 3: Вы проходите через турникет (событие Pass). Турникет снова закрывается.
 *
 * */

// Перечисление состояний турникета
typedef enum {
    STATE_CLOSED,  // Турникет закрыт
    STATE_OPEN     // Турникет открыт
} State;

// Перечисление событий
typedef enum {
    EVENT_INSERT_TOKEN,    // Вставка жетона/карты
    EVENT_PASS,            // Проход через турникет
    EVENT_INVALID_TOKEN    // Неверный жетон/карта
} EventType;

// Структура для описания состояния
typedef struct {
    State currentState;  // Текущее состояние турникета
    char *stateName;     // Название состояния (например, "CLOSED", "OPEN")
} TurnstileState;

// Функция перехода состояний  (функция тип TurnstileStat  return currentState)
TurnstileState handle_event(TurnstileState currentState, EventType event) {
    switch (currentState.currentState) {
        case STATE_CLOSED:
            if (event == EVENT_INSERT_TOKEN) {
                printf("The turnstile is open.\n");
                TurnstileState newState = {STATE_OPEN, "OPEN"};
                return newState;
            } else if (event == EVENT_INVALID_TOKEN) {
                printf("Invalid token/card. Turnstile remains closed.\n");
                return currentState;
            }
            break;
        case STATE_OPEN:
            if (event == EVENT_PASS) {
                printf("Passage permitted. Turnstile closed..\n");
                TurnstileState newState = {STATE_CLOSED, "CLOSED"};
                return newState;
            }
            break;
        default:
            return currentState;
    }
    return currentState;  // Если событие не распознано, остаёмся в том же состоянии
}

int main() {
    // Начальное состояние турникета
    TurnstileState currentState = {STATE_CLOSED, "CLOSED"};

    // Симуляция работы турникета
    currentState = handle_event(currentState, EVENT_INSERT_TOKEN);  // Вставка жетона
    printf("Current status: %s\n", currentState.stateName);

    currentState = handle_event(currentState, EVENT_PASS);          // Проход через турникет
    printf("Current status:: %s\n", currentState.stateName);

    currentState = handle_event(currentState, EVENT_INVALID_TOKEN); // Попытка вставить неверный жетон
    printf("Current status:: %s\n", currentState.stateName);

    return 0;
}


/*
 * Перечисления (enum) для состояний и событий:

State — представляет возможные состояния турникета (закрыт или открыт).
EventType — описывает возможные события (вставка жетона, проход, неверный жетон).
Структура TurnstileState:

Включает в себя текущее состояние (как значение enum) и строку с названием состояния. Это полезно для более читаемых выводов и управления состояниями.
Функция handle_event:

На основе текущего состояния и события, функция определяет, в какое состояние должен перейти турникет.
Она возвращает новую структуру TurnstileState, что позволяет гибко управлять состояниями и их атрибутами.
Основная программа (main):

В этой функции симулируется работа турникета: вставка жетона, проход и попытка вставки неверного жетона.
Выводится текущее состояние после каждого события, что помогает проследить за изменениями.


 Заключение
Для простых автоматов: Используйте enum для состояний и событий, как в приведённом выше примере.
Для сложных автоматов: Используйте структуры и таблицы переходов для хранения состояний, переходов и дополнительных данных.
Для объектно-ориентированных систем: Представляйте состояния и события как объекты или классы, что позволяет инкапсулировать логику.

 * */



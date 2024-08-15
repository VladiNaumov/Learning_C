/*
 * Сценарий системы управления доступом
    Состояния:
    Locked (Заблокировано): Система закрыта, требуется карта доступа и PIN-код для разблокировки.
    Card Scanned (Карта отсканирована): Карта доступа успешно считана, ожидается ввод PIN-кода.
    Unlocked (Разблокировано): Доступ разрешен, система открыта.
    Access Denied (Доступ запрещен): Неправильный ввод PIN-кода, доступ запрещен, возвращаемся в состояние "Заблокировано".
    События:
    Scan Card (Сканирование карты): Пользователь приложил карту доступа.
    Enter PIN (Ввод PIN-кода): Пользователь ввел PIN-код.
    Timeout (Тайм-аут): Время на ввод PIN-кода истекло.
Lock (Блокировка): Система закрывается и возвращается в состояние "Заблокировано".
 * */

#include <stdio.h>
#include <string.h>

// Определение состояний системы доступа
typedef enum {
    LOCKED,
    CARD_SCANNED,
    UNLOCKED,
    ACCESS_DENIED
} AccessState;

// Определение событий системы доступа
typedef enum {
    SCAN_CARD,
    ENTER_PIN,
    TIMEOUT,
    LOCK
} AccessEvent;

// Определение структуры системы доступа
typedef struct {
    AccessState state;       // Текущее состояние системы
    const char* correctPIN;  // Правильный PIN-код
} AccessControlSystem;

// Функция обработки событий
AccessState process_access_event(AccessControlSystem* system, AccessEvent event, const char* enteredPIN) {
    switch (system->state) {
        case LOCKED:
            if (event == SCAN_CARD) {
                system->state = CARD_SCANNED;
                printf("Карта отсканирована. Введите PIN-код.\n");
            }
            break;

        case CARD_SCANNED:
            if (event == ENTER_PIN) {
                if (strcmp(system->correctPIN, enteredPIN) == 0) {
                    system->state = UNLOCKED;
                    printf("Правильный PIN-код. Доступ разрешен.\n");
                } else {
                    system->state = ACCESS_DENIED;
                    printf("Неправильный PIN-код. Доступ запрещен.\n");
                }
            } else if (event == TIMEOUT) {
                system->state = LOCKED;
                printf("Время истекло. Система заблокирована.\n");
            }
            break;

        case UNLOCKED:
            if (event == LOCK) {
                system->state = LOCKED;
                printf("Система заблокирована.\n");
            }
            break;

        case ACCESS_DENIED:
            if (event == LOCK) {
                system->state = LOCKED;
                printf("Система заблокирована.\n");
            }
            break;
    }
    return system->state;
}

int main() {
    // Инициализация системы доступа с правильным PIN-кодом "1234"
    AccessControlSystem system = {LOCKED, "1234"};

    // Пример последовательности событий
    process_access_event(&system, SCAN_CARD, NULL);           // Сканирование карты
    process_access_event(&system, ENTER_PIN, "1111");         // Ввод неправильного PIN-кода
    process_access_event(&system, LOCK, NULL);                // Блокировка системы

    process_access_event(&system, SCAN_CARD, NULL);           // Сканирование карты снова
    process_access_event(&system, ENTER_PIN, "1234");         // Ввод правильного PIN-кода
    process_access_event(&system, LOCK, NULL);                // Блокировка системы после использования

    return 0;
}

/*
 * Объяснение:
    Состояния системы доступа:

    LOCKED: Система заблокирована, ждет сканирования карты.
    CARD_SCANNED: Карта отсканирована, система ожидает ввода PIN-кода.
    UNLOCKED: Доступ разрешен, система разблокирована.
    ACCESS_DENIED: Введен неправильный PIN-код, доступ запрещен, система возвращается в заблокированное состояние.
    События системы доступа:

    SCAN_CARD: Карта отсканирована.
    ENTER_PIN: Ввод PIN-кода.
    TIMEOUT: Время на ввод PIN-кода истекло.
    LOCK: Система возвращается в заблокированное состояние.
    Структура AccessControlSystem:

    Содержит текущее состояние системы (state) и правильный PIN-код (correctPIN).
    Функция process_access_event:

    Обрабатывает текущее состояние системы и полученное событие. В зависимости от состояния и события система переходит в новое состояние, либо остается в текущем.
    Использование в main:

    В main мы моделируем последовательность событий: сначала карта сканируется, затем вводится неправильный PIN-код, затем система блокируется. После этого карта сканируется снова, вводится правильный PIN-код, и система снова блокируется.
 * */
/*
Adapter (Адаптер)
Адаптер используется для преобразования интерфейса одного класса в интерфейс, ожидаемый клиентом.
В C это можно реализовать с помощью функции, которая преобразует один тип данных в другой или изменяет вызовы функций.
*/

#include <stdio.h>

// Определение старого интерфейса
typedef struct {
    int old_interface;
} OldSystem;

// Старая функция, работающая с старым интерфейсом
void old_function(OldSystem* system) {
    printf("Old function: %d\n", system->old_interface);
}

// Определение нового интерфейса
typedef struct {
    int new_interface;
} NewSystem;

// Адаптер, преобразующий новый интерфейс в старый
void adapter(NewSystem* system) {
    OldSystem old_system = {system->new_interface};  // Преобразование нового интерфейса в старый
    old_function(&old_system);  // Вызов старой функции с преобразованным интерфейсом
}

int main() {
    NewSystem new_system = {42};  // Создаем новый интерфейс
    adapter(&new_system);  // Используем адаптер для вызова старой функции
    return 0;
}

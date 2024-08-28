#include <stdio.h>
#include "person.h"
#include "group.h"

// Основная функция, которая управляет всей логикой программы
void runProgram() {
    struct Group *group = createGroup();
    if (group == NULL) {
        return; // Не удалось создать группу, завершение работы функции
    }

    struct Person *person1 = createPerson("John Doe", 30, "Male");
    struct Person *person2 = createPerson("Jane Smith", 25, "Female");
    struct Person *person3 = createPerson("Alice Johnson", 28, "Female");

    if (person1 == NULL || person2 == NULL || person3 == NULL) {
        freeGroup(group); // Освобождение памяти для группы при ошибке
        return;
    }

    addPersonToGroup(group, person1);
    addPersonToGroup(group, person2);
    addPersonToGroup(group, person3);

    printGroup(group);

    freeGroup(group); // Освобождение памяти, выделенной для группы и ее содержимого
}

int main() {
    runProgram(); // Вызов основной функции программы
    return 0;
}

/* В данном коде мы храним не указатели на структуры, а сами структуры Person в массиве. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 50

typedef struct {
    char name[NAME_LENGTH];
    int age;
    char address[100];
} Person;

void addPerson(Person **people, int *count, int *capacity) {
    if (*count >= *capacity) {
        *capacity *= 2;
        *people = realloc(*people, *capacity * sizeof(Person));
        if (*people == NULL) {
            printf("Ошибка выделения памяти!\n");
            exit(1);
        }
    }

    printf("Введите имя: ");
    scanf("%s", (*people)[*count].name);

    printf("Введите возраст: ");
    scanf("%d", &(*people)[*count].age);

    printf("Введите адрес: ");
    scanf("%s", (*people)[*count].address);

    (*count)++;
    printf("Человек добавлен!\n");
}

void listPeople(Person *people, int count) {
    if (count == 0) {
        printf("Картотека пуста.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("ID: %d\n", i + 1);
        printf("Имя: %s\n", people[i].name);
        printf("Возраст: %d\n", people[i].age);
        printf("Адрес: %s\n", people[i].address);
        printf("-------------------\n");
    }
}

void deletePerson(Person *people, int *count) {
    if (*count == 0) {
        printf("Картотека пуста.\n");
        return;
    }

    int id;
    printf("Введите ID человека для удаления: ");
    scanf("%d", &id);

    if (id < 1 || id > *count) {
        printf("Неверный ID!\n");
        return;
    }

    // Смещение всех людей на одну позицию влево, начиная с удалённого
    for (int i = id - 1; i < *count - 1; i++) {
        people[i] = people[i + 1];
    }

    (*count)--;
    printf("Человек удалён!\n");
}

void editPerson(Person *people, int count) {
    if (count == 0) {
        printf("Картотека пуста.\n");
        return;
    }

    int id;
    printf("Введите ID человека для редактирования: ");
    scanf("%d", &id);

    if (id < 1 || id > count) {
        printf("Неверный ID!\n");
        return;
    }

    id--;  // Преобразуем в индекс массива

    printf("Редактирование человека ID %d:\n", id + 1);
    printf("Текущее имя: %s\n", people[id].name);
    printf("Введите новое имя: ");
    scanf("%s", people[id].name);

    printf("Текущий возраст: %d\n", people[id].age);
    printf("Введите новый возраст: ");
    scanf("%d", &people[id].age);

    printf("Текущий адрес: %s\n", people[id].address);
    printf("Введите новый адрес: ");
    scanf("%s", people[id].address);

    printf("Данные человека обновлены!\n");
}

int main() {
    int capacity = 2;  // Начальная ёмкость массива
    Person *people = malloc(capacity * sizeof(Person));
    if (people == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    int count = 0;
    int choice;

    while (1) {
        printf("\nМеню:\n");
        printf("1. Добавить нового человека\n");
        printf("2. Показать всех людей\n");
        printf("3. Удалить человека\n");
        printf("4. Изменить данные человека\n");
        printf("5. Выйти\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPerson(&people, &count, &capacity);
                break;
            case 2:
                listPeople(people, count);
                break;
            case 3:
                deletePerson(people, &count);
                break;
            case 4:
                editPerson(people, count);
                break;
            case 5:
                printf("Выход...\n");
                free(people);  // Освобождение выделенной памяти
                return 0;
            default:
                printf("Неверный выбор!\n");
        }
    }

    return 0;
}

/*
Динамическое выделение памяти:

Переменная capacity задаёт начальный размер картотеки (например, 2).
Функция malloc выделяет память для начального массива людей.
Функция realloc удваивает размер массива при необходимости (если достигнута текущая ёмкость).
Добавление нового человека:

Перед добавлением проверяется, достаточно ли памяти. Если массив заполнен, realloc увеличивает его размер.
Завершение программы:

В конце программы, перед выходом, память, выделенная под картотеку, освобождается с помощью free.


В данном коде мы храним не указатели на структуры, а сами структуры Person в массиве.
В массиве Person *people, где people — это указатель на первый элемент массива структур Person, каждая ячейка массива представляет собой саму структуру Person, а не указатель на неё.

Объяснение:
Массив структур:

В коде массив people хранит элементы типа Person, а каждый элемент — это конкретная структура Person, которая содержит такие поля, как name, age, и address.
Массив выделяется динамически с помощью malloc и увеличивается с помощью realloc, чтобы вместить больше структур, когда нужно добавить новых людей.
Что такое Person *people?:

Person *people — это указатель на массив структур Person. Если обратиться к элементу этого массива, например, people[i], то мы получаем саму структуру Person, а не указатель на структуру.
Реализация:

Вызов *people = realloc(*people, *capacity * sizeof(Person)); в функции addPerson означает, что мы изменяем размер выделенной памяти под массив структур Person, а не под массив указателей на структуры.
Важное отличие:
Если бы в массиве хранились указатели на структуры (например, Person **people), то для каждого элемента массива нужно было бы выделять отдельную память для самой структуры Person, а затем сохранять указатель на эту память в массиве.
В данном же коде хранится сразу массив структур.
*/
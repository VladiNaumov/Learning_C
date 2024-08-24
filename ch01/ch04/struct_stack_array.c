#include <stdio.h>

typedef struct person
{
    char* name;   // Имя человека
    int age;      // Возраст человека
} person;

// Функция для инициализации массива структур
void initialize_people(person arr[], char* names[], int ages[], int size)
{
    for (int i = 0; i < size; ++i) {
        arr[i].name = names[i];
        arr[i].age = ages[i];
    }
}

// Функция для вывода значений массива структур
void print_people(person arr[], int size)
{
    for (int i = 0; i < size; ++i) {
        printf("Person[%d]: address=%p \t name=%s \t age=%d \n", i, (void*)&arr[i], arr[i].name, arr[i].age);
    }
}

// Основная функция, которая инициализирует и обрабатывает массив структур
int array()
{
    char* names[] = {"Bob", "Lisp", "Kate", "Tom"};
    int ages[] = {30, 25, 28, 22};
    int size = sizeof(names) / sizeof(names[0]);

    person arr[size];  // Массив структур

    initialize_people(arr, names, ages, size);  // Инициализация
    print_people(arr, size);  // Вывод

    return 0;
}

int main(void)
{
    return array();
}

/*

Структура person:

Теперь структура называется person и имеет два поля:
name: указатель на строку (char*), представляющую имя человека.
age: целочисленное поле (int), представляющее возраст человека.
Функция initialize_people:

Эта функция инициализирует массив структур arr, присваивая каждому элементу имя и возраст из массивов names и ages.
Функция print_people:

Эта функция выводит на консоль содержимое массива структур, включая адрес каждой структуры, её имя и возраст.
Функция array:

В этой функции создаётся массив строк names и массив целых чисел ages, а затем создаётся массив структур arr, который инициализируется и выводится с помощью функций initialize_people и print_people.
*/
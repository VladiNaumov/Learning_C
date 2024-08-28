/* передача структуры в функцию по указателю */
/*
Указатели на структуру как параметры
При использовании структур в качестве параметров в функции следует учитывать, что при вызове функции для структуры,
также как и для параметров типа int или char, выделяется память, в которую помещаются значения элементов структуры.
То есть структура в функцию передается по значению, а это значит, что переданную в функцию структуру мы изменить не можем.

Если необходимо уменьшить выделение памяти (особенно если структура большая) или
иметь возможность изменять изначальную структуру в функции, то можно передавать в функцию указатель на структуру:
*/

#include <stdio.h>
  
struct person{
    char* name;
    int age;
};
 
void change_person(struct person* user){
    user->age = user->age + 1;
}

int main(void){
    struct person bob = {"Bob", 22};
    printf("Before change. %s : %d \n", bob.name, bob.age);
    change_person(&bob);
    printf("After change. %s : %d \n", bob.name, bob.age);
    return 0;
}

/*
В этом примере функция change_person принимает указатель на структуру person и увеличивает на единицу значение элемента age.

Для проверки в функции main выводим данные объекта person на консоль до и после вызова функции change_person.

Before change. Bob : 22
After change. Bob : 23

*/

/* ----------------- Struct pointer ---------------------------- */

#include <stdio.h>

struct person {
    int age;
    char *name[20];
};

int main(void){
    struct person kate = {31, "Kate"};
    // указатель на переменную kate
    struct person *p_kate = &kate;

    // получаем значение элемента name
    char *name = p_kate->name;
    // получаем значение элемента age
    int age = (*p_kate).age;

    printf("name = %s \t age = %d \n", name, age);

    // изменим элемент age в структуре
    p_kate->age = 32;
    printf("name = %s \t age = %d \n", kate.name, kate.age);
    return 0;
}

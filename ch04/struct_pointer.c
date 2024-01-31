#include <stdio.h>
 
struct person 
{
    int age;
    char name[20];
};
 
int main(void)
{
    struct person kate = {31, "Kate"};
    // указатель на переменную kate
    struct person * p_kate = &kate;
     
    // получаем значение элемента name
    // получаем значение элемента age
    char * name = p_kate->name;
    int age = (*p_kate).age;
     
    printf("name = %s \t age = %d \n", name, age);
     
    // изменим элемент age в структуре
    p_kate->age = 32;
    printf("name = %s \t age = %d \n", kate.name, kate.age);
    return 0;
}
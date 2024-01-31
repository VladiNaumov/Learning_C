#include <stdio.h>
 
typedef struct
{
    int age;
    int salary;
    
}person;


void demo(){

    printf("\n HELLO \n");
}

 
int main(void)
{
    person tom = {0};
    tom.age = 22;
    tom.salary = 100;

    person alex = {33,1000};


    demo();

   printf("Age: %d \t salary: %d \n", tom.age, tom.salary);
   printf("Age: %d \t salary: %d \n", alex.age, alex.salary);


    return 0;
}


/*                              */
#include <stdio.h>


struct company
{
    char* name;
    char* country;
};
struct phone
{
    char* title;
    int price;
    struct company manufacturer;
};

int main(void)
{
    struct phone realmeC31;
    realmeC31.title = "Realme C31";
    realmeC31.price = 7900;

    // устанавливаем значения для вложенной структуры
    realmeC31.manufacturer.name = "Realme";
    realmeC31.manufacturer.country = "China";

    // При инициализации структуры сразу можно инициализировать и вложенную структуру:
    // struct phone realmeC31= {"Realme C31", 7900, {"Realme", "China"}};

    printf("Phone: %s \n", realmeC31.title);
    printf("Price: %d \n", realmeC31.price);
    printf("Manufacturer: %s \n", realmeC31.manufacturer.name);
    return 0;

}
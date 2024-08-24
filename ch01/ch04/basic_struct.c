#include <stdio.h>
 
typedef struct {
    int age;
    int salary;
}person;


void demo(){
    printf("\n HELLO \n");
}

 
int main(void){
    person tom = {0};
    tom.age = 22;
    tom.salary = 100;
    person alex = {33,1000};

    demo();

   printf("Age: %d \t salary: %d \n", tom.age, tom.salary);
   printf("Age: %d \t salary: %d \n", alex.age, alex.salary);

    return 0;
}


/*   ------------------------   */

#include <stdio.h>


struct company{
    char* name;
    char* country;
};

struct phone{
    char* title;
    int price;
    struct company manufacturer;
};

int main(void){
    struct phone realmeC31;
    realmeC31.title = "Realme C31";
    realmeC31.price = 7900;

    // устанавливаем значения для вложенной структуры
    realmeC31.manufacturer.name = "Realme";
    realmeC31.manufacturer.country = "China";

    printf("Phone: %s \n", realmeC31.title);
    printf("Price: %d \n", realmeC31.price);
    printf("Manufacturer: %s \n", realmeC31.manufacturer.name);
    return 0;

}

/* ---------------------------------------------------- */

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

/* ---------------------------------------------------------- */

#include <stdio.h>
 
struct person {
    int age;
    char name[20];
};
 
int main(void){
    struct person people[] = { {23, "Tom"}, {32, "Bob"}, {26, "Alice"}, {41, "Sam"}};
    int n = sizeof(people)/sizeof(people[0]);
    for(struct person *p=people; p < people+n; p++){
        printf("Name:%s \t Age: %d \n", p->name, p->age);
    }
    return 0;
}

/*--------------------------------------------------------------------*/


#include <stdio.h>

typedef struct{
    char name;
    int age;
} person;

void print_person(person user){
    printf("Name: %s \n", user.name);
    printf("Age: %d \n", user.age);
}

int main(void){
    person tom = {.name = "Tom", .age=38 };
    print_person(tom);
    return 0;
}

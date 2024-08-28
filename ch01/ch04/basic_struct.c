#include <stdio.h>

/*--------------------- basic Struct -----------------------*/


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

/* ---------------------------------- */
struct person{
    char* name;
    int age;
};

struct person create_person(char* name, int age){
    struct person user;
    user.name = name;
    user.age = age;
    return user;
}

int main(void){
    struct person bob = {"Bob", 22};
    printf("Name: %s \n", bob.name);
    printf("Age: %d \n", bob.age);
    return 0;
}

/* ---------------------------   */

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

/* ---------------массив struct People -------------------- */

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



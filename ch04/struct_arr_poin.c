#include <stdio.h>
 
struct person 
{
    int age;
    char name[20];
};
 
int main(void)
{
    struct person people[] = { {23, "Tom"}, {32, "Bob"}, {26, "Alice"}, {41, "Sam"}};
     
    int n = sizeof(people)/sizeof(people[0]);
     
    for(struct person *p=people; p < people+n; p++)
    {
        printf("Name:%s \t Age: %d \n", p->name, p->age);
    }
    return 0;
}

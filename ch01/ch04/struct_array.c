#include <stdio.h>


typedef struct
{
    char* value;
    struct node* next;

}node;


int array(){

    node bob;
    node lisp;
    node kate;
    node tom;

    node* p1 = &bob;
    node* p2 = &lisp;
    node* p3 = &kate;
    node* p4 = &tom;


    node arr[] = {*p1, *p2, *p3, *p4};

    //printf("arr[0] = %s \n", *arr);

    for (int i = 0; i < 4; ++i) {

        void* address = arr + i;
        node value = *(arr + i);
        printf("array[%d]: address=%p \t name=%s \n", i, address, value);


    }

    return 0;

}

int main(void)
{

   array();


}
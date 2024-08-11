#include <stdio.h>

int array_(){

    int array[5] = {1, 2, 3, 4, 5};

    for(int i = 0; i < 5; i++)
    {
        void* address = array + i;  // получаем адрес i-го элемента массива
        int value = *(array + i);   // получаем значение i-го элемента массива
        printf("array[%d]: address=%p \t value=%d \n", i, address, value);
    }
    return 0;
}

void array(){

 int arr[] = {100, 200, 4, 9, 0, 10};

    int ptr_a = *(arr + 3); // 9

    int size = sizeof(arr) / sizeof(*arr);

    for (int i = 0; i < size; ++i )
    {
        printf(" %d ", *(arr+i));

    }

    printf(" %d ", ptr_a);
}

void array_two(){

    int arr[] = {100, 200, 4, 9, 0, 10};

    for (int i = 0; i < 6; ++i )
    {
        printf(" %d ", *(arr+i));

    }
}

int array_three()
{
    int A[2] = {40, 20};

     int *ptr1 = A;
     int *ptr2 = A;

     // Change the value of the first element to 13
     *A = 13;

     // Change the value of the second element to 17
     *(A+1) = 17;

    printf("ptr1 %d ptr2 %d\n", *ptr1, *ptr2 );
    return 0;
}

int array_foo(){
    int A[2] = {40, 20};

    // A -> (int *) ptr to A[0] element, &A -> (int (*)[]) -> ptr to whole Array.
    int *ptr = A;
    printf("ptr -> A[1] = %d\n", *(ptr + 1)); // A[1] => 20.

    //Illegal usage of A.
    // int a_2 = ++A;  //expected lvalue.

    //But with ptr you can do this.
    int b_2 = *++ptr; //Now ptr contains address of A[1]. (b_2 = A[1]);

    int (*ptr2)[2] = &A; //ptr to array, not to literal element.

    //*ptr2 => get array.
    //**ptr2 => get first element of array.
    //*ptr2 + 1 => get address of second element of array.
    printf("ptr2 -> A[1] = %d\n", *( *ptr2 + 1) );

    return 0;
}

int array_double(){
    double point[3] = {0.0, 1.0, 2.0};
    double *ptr = point;

    /* prints x 0.0, y 1.0 z 2.0 */
    printf("x %f y %f z %f\n", ptr[0], ptr[1], ptr[2]);

    // printf("length of point is %s\n", length(point));

    return 0;
}


int pointer_array_char(){

    char *fruit[] = {"apricot", "apple", "banana", "lemon", "orange"};
    for(int i=0; i < 5; i++)
    {
        printf("%s \n", fruit[i]);
    }
    return 0;
}

int pointer_const(){

    int A[2] = {100, 200};
    const int *a0 = A;
    printf("content of a0 = %d\n", *a0);

    //*a0 *= 10; //error: cannot change constant value.

    a0 = (A + 1); // A[1]
    printf("content of a0 = %d\n", *a0); //prints: A[1]

    return 0;

     /*
        Чтобы запретить менять адрес (значение переменной) указателя,
        надо добавить слово const ПОСЛЕ звёздочки. Кроме того, можно добавить ключевые слова const перед и после '*',
        чтобы сделать переменную фиксированной ещё сильнее, например так:


        // Переменная с постоянным адресом и постоянным содержимым.
        const int *const ptr = A; // constant address with constant content

        // Переменная с постоянным адресом (содержимое можно менять)
        int *const ptr2 = A; // constant address only.

        // Переменная с постоянным содержимым, но с изменяемым адресом (значение справа)
        const int *ptr3 = A; // constant content only (can change address (rvalue))
*/

}




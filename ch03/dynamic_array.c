#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *mas = NULL;
    mas = malloc(sizeof(int) * 2);

    *mas = 737;
    *(mas +1) = 333;

    mas = realloc (mas,sizeof(int) * 10 );

     *(mas +9) = 323;

    printf("%d\n", mas[0]);
    printf("%d\n", mas[1]);
     printf("%d\n", mas[9]);

    free(mas);
    mas = NULL;
    return 0;
}
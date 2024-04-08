
/*
Все определенные в программе данные, например, переменные, хранятся в памяти по определенному адресу.
И указатели позволяют напрямую обращаться к этим адресам и благодаря этому манипулировать данными.
Указатели представляют собой объекты, и они (указатели) хранят адреса других объектов такие как: переменных, констант, указателей или функций.
*/

int pointer(){

    int x = 123;
    int *ip = &x;                   // указатель хранит адрес объекта int
    void *vp = ip;                  // void-указатель получает адрес из указателя ip
    printf("vp: %p\n", vp);         // получаем адрес, который хранится в указателе vp
    printf("ip: %p\n", (void*)ip);  // преобразование к типу void* - получаем адрес из указателя ip

    return 0;

}

int pointer(){

    int *pointer;
    int value2 = 10;
    pointer = &value2;
    printf("Value from pointer: %d\n", *pointer);

    printf("Size of int pointer: %zu\n", sizeof (int*)); /* size 8 bytes */
    printf("Size of int variable: %zu\n", sizeof (int)); /* size 4 bytes */
    printf("Size of char pointer: %zu\n", sizeof (char*)); /* size 8 bytes */
    printf("Size of char variable: %zu\n", sizeof (char)); /* size 1 bytes */
    printf("Size of short pointer: %zu\n", sizeof (short*)); /* size 8 bytes */
    printf("Size of short variable: %zu\n", sizeof (short)); /* size 2 bytes */
    return 0;


}

int pointer(){

    char c = 'N';  // определяем переменную
    int d = 10;     // определяем переменную
    short s = 2;    // определяем переменную

    char *pc = &c;          // получаем адрес переменной с типа char
    int *pd = &d;           // получаем адрес переменной d типа int
    short *ps = &s;         // получаем адрес переменной s типа short

    printf("Variable c: address=%p \t value=%s \n", (void*) pc, *pc);
    printf("Variable d: address=%p \t value=%d \n", (void*) pd, *pd);
    printf("Variable s: address=%p \t value=%hd \n", (void*) ps, *ps);

    return 0;

}

int pointer(){

    int a = 10;

    int *pa = &a;
    int *pb = pa;

    *pa = 25;

    printf("Value on pointer pa: %d \n", *pa);  // 25
    printf("Value on pointer pb: %d \n", *pb);  // 25
    printf("Value of variable a: %d \n", a);    // 25

    return 0;
}

int pointer(){
    int a = 123;
    int *p1 = &a;

    //Теперь p2 хранит тот же адрес, что и p1.
    int *p2 = &a;

    *p1 -= 3; // a = 123 - 3.
    printf("*p2 = %d\n", *p2); //Выведет 120

    return 0;
}


int pointer(){
    int n = 10;

    int *ptr = &n;
    printf("address=%p \t value=%d \n", (void*)ptr, *ptr);

    ptr++;
    printf("address=%p \t value=%d \n", (void*)ptr, *ptr);

    ptr--;
    printf("address=%p \t value=%d \n", (void*)ptr, *ptr);

    return 0;

    /*
     *  address=0060FEA8	value=10
        address=0060FEAC	value=6356652
        address=0060FEA8	value=10
    */
}

int pointer(){

    /*
        Указатели на указатели
        Кроме обычных указателей в языке Си мы можем создавать указатели на другие указатели.
        Если указатель хранит адрес переменной, то указатель на указатель хранит адрес указателя,
        на который он указывает. Такие ситуации еще называются многоуровневой адресацией.

        Например:

    */

    int  x = 22;
    int *px = &x;       // указатель px хранит адрес переменной x
    int **ppx = &px;    // указатель ppx хранит адрес указателя px

    printf("Address of px: %p \n", (void *)ppx);
    printf("Address of x: %p \n", (void *)*ppx);
    printf("Value of x: %d \n", **ppx);
    return 0;


    /*
    Здесь указатель ppx хранит адрес указателя px.
    Поэтому через выражение *ppx можно получить значение, которое хранится в указателе px - адрес переменной x.
    А через выражение **ppx можно получить значение по адресу из px, то есть значение переменной x.
    */

    /*
    смотрим  в каких адресах памяти находятся разные структуры данных (int global, int, const int, int, main)
    */

    #include <stdio.h>

    int global;
    int Global = 0;
    const int GLOBAL = 0xdeabdbeef;

    int main (int argc, char *argv[])
    {

    int i;

    printf(" -: %x\n", &global);
    printf(" -: %x\n", &Global);
    printf(" -: %x\n", &GLOBAL);
    printf(" -: %x\n", &i);
    printf(" -: %x\n", &argc);
    printf(" -: %x\n", &argv);
    printf(" -: %x\n", &main);

    exit(0);

    }


}

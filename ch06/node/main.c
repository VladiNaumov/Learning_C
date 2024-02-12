#include"node.h"


int main(void)
{

    // функция выделения памяти
    struct node* bob = node_malloc();
    struct node* lisp = node_malloc();
    struct node* kate = node_malloc();
    struct node* tom = node_malloc();

    // создание экземпляра структуры
    node_init(bob, "BOB");
    node_init(lisp,"LISP");
    node_init(kate,"KATE");
    node_init(tom,"TOM");

    node_linc(bob, lisp);
    node_linc(lisp, 0);


    getAddress(tom);
    getAddress(lisp);



   node_linc(kate, lisp);
   node_linc(tom, kate);



    for (int var = 0; var < 3; ++var) {

        struct node* ext = node_malloc();
        node_init(ext, "LEGION");

        node_linc(ext, lisp);

        addArray_(ext, var);


        node_linc(ext, lisp);

    }

    getArr(3);


    // устанавливаем указатель на первую структуру в цепочке
    iterator(bob);

    node_destroy(bob);
    node_destroy(lisp);
    node_destroy(kate);
    node_destroy(tom);

   /*
    * не поддерживается стандартом ISO C99
    free(bob);
    free(lisp);
    free(kate);
    free(tom);
   */


    return 0;

}

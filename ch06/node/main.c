#include"node.h"


int main(void)
{

    // создаём "hip" и помещаем структуру в её
    struct node* bob = node_malloc();
    struct node* lisp = node_malloc();
    struct node* kate = node_malloc();
    struct node* tom = node_malloc();

     getAddress(bob);

    // создание экземпляра структуры
    node_new(bob, "BOB");
    node_new(lisp,"LISP");
    node_new(kate,"KATE");
    node_new(tom,"TOM");

    getAddress(bob);

    node_linc(bob, lisp);
    node_linc(lisp, 0);


    getAddress(bob);
    getAddress(lisp);



   node_linc(kate, lisp);
   node_linc(tom, kate);


   // устанавливаем указатель на первую структуру в цепочке
   iterator(bob);


    for (int var = 0; var < 3; ++var) {

        struct node* ext = node_malloc();
        new_node(ext, "LEGION");

        addArray_(ext, var);


       // node_linc(ext, lisp);

    }

    getArr(3);




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

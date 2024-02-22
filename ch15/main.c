 // Реализация простого стека

 #include <stdio.h>
 #include <stdlib.h>
 #include "stack.h"

 // выводит инструкцию по использованию программы
 void instructions( void )
 {
     puts( "Enter choice:\n"
          "1 to push a value on the stack\n"
          "2 to pop a value off the stack\n"
          "3 to end program" );
 }

 void menu(){
     StackNodePtr stackPtr = NULL; // указатель на вершину стека
     unsigned int choice;          // пункт меню, выбранный пользователем
     int value;                    // целое число, введенное пользователем

     instructions(); // вывести меню

     printf( "%s", "? " );
     scanf( "%u", &choice );

     // пока пользователь не выберет пункт меню 3
     while ( choice != 3 ) {

         switch ( choice ) {
         // втолкнуть значение в стек
         case 1:
             printf( "%s", "Enter an integer: " );
             scanf( "%d", &value );
             push( &stackPtr, value );
             printStack( stackPtr );
             break;
         // вытолкнуть значение со стека
         case 2:
             // если стек не пуст
             if ( !isEmpty( stackPtr ) ) {
                 printf( "The popped value is %d.\n", pop( &stackPtr ) );
             }
             printStack( stackPtr );
             break;
         default:
             puts( "Invalid choice.\n" );
             instructions();
             break;
         }

         printf( "%s", "? " );
         scanf( "%u", &choice );
     }

     puts( "End of run." );
 }

 // выполнение программы начинается с функции main
 int main( void ){
     menu();
 }

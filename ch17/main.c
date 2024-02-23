  // Реализация простой очереди
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


// display program instructions to user
void instructions( void )
{
    printf ( "Enter your choice:\n"
           "   1 to add an item to the queue\n"
           "   2 to remove an item from the queue\n"
           "   3 to end\n" );
}

void menu(){

    QueueNodePtr headPtr = NULL; // инициализировать headPtr
    QueueNodePtr tailPtr = NULL; // инициализировать tailPtr
    unsigned int choice;         // пункт меню, выбранный пользователем
    char item;                   // символ, введенный пользователем

    instructions();// вывести меню


    printf( "%s", "? " );
    scanf( "%u", &choice );

    // пока пользователь не выберет пункт меню 3
    while ( choice != 3 ) {

        switch( choice ) {
        // поставить значение в очередь
        case 1:
            printf( "%s", "Enter a character: " );
            scanf( "\n%c", &item );
            enqueue( &headPtr, &tailPtr, item );
            printQueue( headPtr );
            break;

            // удалить значение из очереди
        case 2:
            // если очередь не пуста
            if ( !isEmpty( headPtr ) ) {
                item = dequeue( &headPtr, &tailPtr );
                printf( "%c has been dequeued.\n", item );
            }

            printQueue( headPtr );
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
 int main( void )
 {
     menu();


 }



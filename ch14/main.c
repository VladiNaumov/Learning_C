// Вставка узлов в список и удаление их из списка
#include <stdio.h>
#include <stdlib.h>
#include "node.h"

void menu(){

    ListNodePtr startPtr = NULL; // изначально список пуст
    unsigned int choice;         // выбор пользователя
    char item;                   // символ, введенный пользователем

    instructions(); // вывести меню

    printf( "%s", "? " );
    scanf( "%u", &choice );

    // выполнять цикл, пока пользователь не выберет пункт 3

    while ( choice != 3 ) {
        switch ( choice ) {

        case 1:
            printf( "%s", "Enter a character: " );
            scanf( "\n%c", &item );
            insert( &startPtr, item ); // вставить элемент в список
            printList( startPtr );
            break;

        case 2: // удалить элемент

            // если список не пуст
            if ( !isEmpty( startPtr ) ) {
                printf( "%s", "Enter character to be deleted: " );
                scanf( "\n%c", &item );

                // если символ найден, удалить его из списка
                if ( delete( &startPtr, item ) ) { // удалить элемент
                    printf( "%c deleted.\n", item );
                    printList( startPtr );
                }
                else {
                    printf( "%c not found.\n\n", item );
                }
            }
            else {
                puts( "List is empty.\n" );
            }

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

// выводит инструкцию по работе с программой
void instructions( void )
{
    // puts / fputs() – вывод одной строки в стандартный поток вывода (на консоль).
    puts( "Enter your choice:\n"
         "   1 to insert an element into the list.\n"
         "   2 to delete an element from the list.\n"
         "   3 to end." );


}

int main( void )
{
    menu();

}






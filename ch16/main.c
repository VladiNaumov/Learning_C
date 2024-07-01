#include "node.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{

    struct Node* head = NULL;
    int arr[] = {1,2,3,4,5,6,7,8,9,10};
    //Создаём список из массива
    fromArray(&head, arr, 10);

    printLinkedList(head);

    //Вставляем узел со значением 333 после 4-го элемента (станет пятым)
    insert(head, 4, 333);
    printLinkedList(head);

    pushBack(head, 11);
    pushBack(head, 12);
    pushBack(head, 13);
    pushBack(head, 14);
    printLinkedList(head);

    printf("%d\n", pop(&head));
    printf("%d\n", popBack(&head));

    printLinkedList(head);
    //Удаляем пятый элемент (индексация с нуля)
    deleteNth(&head, 4);
    printLinkedList(head);
    deleteList(&head);

   // getch(); ISO 99


}

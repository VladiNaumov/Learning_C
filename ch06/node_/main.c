#include "node.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    struct Node* head = NULL;

    for (int var = 0; var < 1000; ++var) {
        push(&head, "100");
    }
    push(&head, "1");
    push(&head, "1");
    push(&head, "1");
    push(&head, "1");
    push(&head, "1");
    push(&head, "1");


    iterator(head);

    node_destroy(head);

    return 0;

}

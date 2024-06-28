#include <stdio.h>
#include <stdlib.h>

/* Односвязный список состоит из узлов. Каждый узел содержит значение и указатель на следующий узел,
 * поэтому представим его в качестве структуры.Чтобы не писать каждый раз struct мы определили новый тип */

typedef struct Node{
    int value;
    struct Node *next;
} Node;

// указатель на первый элемент списка. Вначале списка нет и указатель ссылается на NULL.
struct Node *head = NULL;

/* Для добавления нового узла необходимо

1) Выделить под него память.
2) Задать ему значение
3) Сделать так, чтобы он ссылался на предыдущий элемент (или на NULL, если его не было)
4) Перекинуть указатель head на новый узел.

*/

// добавления нового узла
void push(Node **head, int data) {
    Node *tmp = (Node*) malloc(sizeof(Node));
    tmp->value = data;
    tmp->next = (*head);
    (*head) = tmp;

    /*
    Node *tmp = (Node*) malloc(sizeof(Node));
    (*tmp).value = data;
    (*tmp).next = (*head);
    (*head) = tmp;
    */
}


// она удаляет элемент, на который указывает head и возвращает его значение.
int pop(Node **head) {
    Node* prev = NULL;
    int val;
    if (head == NULL) {
        exit(-1);
    }
    prev = (*head);
    val = prev->value;
    (*head) = (*head)->next;
    free(prev);
    return val;
}

//перебор всех элементов списка
Node* getNth(Node* head, int n) {
    int counter = 0;
    while (counter < n && head) {
        head = head->next;
        counter++;
    }
    return head;
}

// Для нахождение последнего элемента
Node* getLast(Node *head) {
    if (head == NULL) {
        return NULL;
    }
    while (head->next) {
        head = head->next;
    }
    return head;
}

// pushBack (её ещё принято называть shift или enqueue), которая добавляет новый элемент в конец списка
void pushBack(Node *head, int value) {
    Node *last = getLast(head);
    Node *tmp = (Node*) malloc(sizeof(Node));
    tmp->value = value;
    tmp->next = NULL;
    last->next = tmp;
}


/* popBack (unshift, или dequeue), которая удаляет последний элемент списка и возвращает его значение.
Будем использовать два указателя. Один – текущий узел, второй – предыдущий.*/
int popBack(Node **head) {
    Node *pFwd = NULL;  //текущий узел
    Node *pBwd = NULL;  //предыдущий узел
    //Получили NULL
    if (!head) {
        exit(-1);
    }
    //Список пуст
    if (!(*head)) {
        exit(-1);
    }

    pFwd = *head;
    while (pFwd->next) {
        pBwd = pFwd;
        pFwd = pFwd->next;
    }

    if (pBwd == NULL) {
        free(*head);
        *head = NULL;
    } else {
        free(pFwd->next);
        pBwd->next = NULL;
    }

    return 0;
}

/* функцию insert, которая вставляет на n-е место новое значение.
Для вставки, сначала нужно будет пройти до нужного узла, потом создать новый элемент и поменять указатели.
Если мы вставляем в конец, то указатель next нового узла будет указывать на NULL, иначе на следующий элемент.*/
void insert(Node *head, unsigned n, int val) {
    unsigned i = 0;
    Node *tmp = NULL;
    //Находим нужный элемент. Если вышли за пределы списка, то выходим из цикла,
    //ошибка выбрасываться не будет, произойдёт вставка в конец
    while (i < n && head->next) {
        head = head->next;
        i++;
    }
    tmp = (Node*) malloc(sizeof(Node));
    tmp->value = val;
    //Если это не последний элемент, то next перекидываем на следующий узел
    if (head->next) {
        tmp->next = head->next;
        //иначе на NULL
    } else {
        tmp->next = NULL;
    }
    head->next = tmp;
}

/* Функция удаления элемента списка похожа на вставку. Сначала получаем указатель на элемент, стоящий до удаляемого,
потом перекидываем ссылку на следующий элемент за удаляемым, потом удаляем элемент.*/

int deleteNth(Node **head, int n) {
    if (n == 0) {
        return pop(head);
    } else {
        Node *prev = getNth(*head, n-1);
        Node *elm  = prev->next;
        int val = elm->value;

        prev->next = elm->next;
        free(elm);
        return val;
    }
}

// удаление
void deleteList(Node **head) {
    Node* prev = NULL;
    while ((*head)->next) {
        prev = (*head);
        (*head) = (*head)->next;
        free(prev);
    }
    free(*head);
}


//  Первая - создать список из массива.
void fromArray(Node **head, int *arr, size_t size) {
    size_t i = size - 1;
    if (arr == NULL || size == 0) {
        return;
    }
    do {
        push(head, arr[i]);
    } while(i--!=0);
}

/* И обратная функция, которая возвратит массив элементов, хранящихся в списке.
Так как мы будем создавать массив динамически, то сначала определим его размер, а только потом запихнём туда значения. */

int* toArray(const Node *head) {

    // определим его размер массивa
    //int leng = length(head); ISO 99

    int leng = 20;
    int *values = (int*) malloc(leng*sizeof(int));
    while (head) {
        values[--leng] = head->value;
        head = head->next;
    }
    return values;
}

//функция, которая будет печатать содержимое списка
void printLinkedList(const Node *head) {
    while (head) {
        printf("%d ", head->value);
        head = head->next;
    }
    printf("\n");
}



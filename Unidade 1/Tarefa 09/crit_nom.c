#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;

void insert(Node** head, int value) {
    Node* new_node = malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = *head;
    *head = new_node;
}

void print_list(Node* head, int id) {
    printf("Lista %d: ", id);
    while (head) {
        printf("%d -> ", head->value);
        head = head->next;
    }
    printf("NULL\n");
}

int main() {
    int N = 1000;
    Node* list1 = NULL;
    Node* list2 = NULL;

    srand(time(NULL));

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < N; i++) {
            #pragma omp task
            {
                int num = rand() % 1000;
                int list_choice = rand() % 2;

                if (list_choice == 0) {
                    #pragma omp critical(list1)
                    insert(&list1, num);
                } else {
                    #pragma omp critical(list2)
                    insert(&list2, num);
                }
            }
        }
    }

    print_list(list1, 1);
    print_list(list2, 2);

    return 0;
}

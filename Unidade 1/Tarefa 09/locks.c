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

// Função para liberar a memória das listas
void free_list(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    int N = 1000;
    int M;

    printf("Digite o número de listas: ");
    scanf("%d", &M);

    Node** lists = malloc(M * sizeof(Node*));
    omp_lock_t* locks = malloc(M * sizeof(omp_lock_t));

    for (int i = 0; i < M; i++) {
        lists[i] = NULL;
        omp_init_lock(&locks[i]);
    }

    srand(time(NULL));

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int num = rand() % 1000;
        int list_choice = rand() % M;

        omp_set_lock(&locks[list_choice]);
        insert(&lists[list_choice], num);
        omp_unset_lock(&locks[list_choice]);
    }

    // Imprime as listas
    for (int i = 0; i < M; i++) {
        print_list(lists[i], i);
        free_list(lists[i]);  // Libera a memória da lista
        omp_destroy_lock(&locks[i]);
    }

    free(locks);
    free(lists);

    return 0;
}

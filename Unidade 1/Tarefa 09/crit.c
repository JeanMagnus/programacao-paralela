#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>  // Para a função gettimeofday

// Estrutura de um nó
typedef struct Node {
    int value;
    struct Node* next;
} Node;

// Estrutura da lista
typedef struct {
    Node* head;
} LinkedList;

// Inserção no início da lista
void insert(LinkedList* list, int value) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = list->head;
    list->head = newNode;
}

// Impressão da lista
void print_list(const char* name, LinkedList* list) {
    printf("%s: ", name);
    Node* current = list->head;
    while (current) {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
}

int main() {
    const int N = 1000;
    LinkedList list1 = {NULL};
    LinkedList list2 = {NULL};

    srand(time(NULL));

    // Variáveis para medir o tempo
    struct timeval start, end;

    // Medir o tempo de início
    gettimeofday(&start, NULL);

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < N; i++) {
            #pragma omp task firstprivate(i)
            {
                int value = rand() % 1000;
                int choice = rand() % 2;

                if (choice == 0) {
                    #pragma omp critical(list1_insert)
                    insert(&list1, value);
                } else {
                    #pragma omp critical(list2_insert)
                    insert(&list2, value);
                }
            }
        }
        // A barreira implícita aqui garante que todas as tarefas terminem
    }

    // Medir o tempo de término
    gettimeofday(&end, NULL);

    // Calculando o tempo total
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    // Imprimindo as listas
    print_list("Lista 1", &list1);
    print_list("Lista 2", &list2);

    // Exibindo o tempo total de execução
    printf("Tempo total de execução: %.6f segundos\n", elapsed_time);

    return 0;
}

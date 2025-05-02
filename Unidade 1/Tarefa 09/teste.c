#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

// Estrutura de um nó
typedef struct Node {
    int value;
    struct Node* next;
} Node;

// Estrutura da lista
typedef struct {
    Node* head;
} LinkedList;

// Função para inserir um valor na lista
void insert(LinkedList* list, int value) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = list->head;
    list->head = newNode;
}

// Função para imprimir a lista
void print_list(const char* name, LinkedList* list) {
    printf("%s: ", name);
    Node* current = list->head;
    while (current) {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
}

// Função para contar os nós de uma lista
int count_nodes(LinkedList* list) {
    int count = 0;
    Node* current = list->head;
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

// Função para calcular a diferença de tempo
long long get_time_difference(struct timeval start, struct timeval end) {
    return ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
}

int main() {
    int N, M;

    // Leitura do número de listas e de inserções
    printf("Digite o número de listas: ");
    scanf("%d", &M);
    printf("Digite o número de inserções: ");
    scanf("%d", &N);

    // Alocando as listas
    LinkedList* lists = (LinkedList*) malloc(M * sizeof(LinkedList));
    for (int i = 0; i < M; i++) {
        lists[i].head = NULL;  // Inicializando cada lista como vazia
    }

    srand(time(NULL));

    struct timeval start, end;
    gettimeofday(&start, NULL); // Início do tempo

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < N; i++) {
            #pragma omp task firstprivate(i)
            {
                int value = rand() % 1000;
                int choice = rand() % M; // Escolhe uma lista aleatória

                // Inserção na lista escolhida de forma concorrente e segura
                #pragma omp critical
                insert(&lists[choice], value);
            }
        }
    }

    gettimeofday(&end, NULL); // Fim do tempo

    // Imprimindo as listas
    for (int i = 0; i < M; i++) {
        print_list("Lista", &lists[i]);
    }

    // Validando o número total de inserções
    int total_insertions = 0;
    for (int i = 0; i < M; i++) {
        total_insertions += count_nodes(&lists[i]);
    }
    printf("Total de inserções realizadas: %d\n", total_insertions);

    long long time_taken = get_time_difference(start, end);
    printf("Tempo total de execução: %lld microssegundos\n", time_taken);

    // Liberando a memória
    for (int i = 0; i < M; i++) {
        Node* current = lists[i].head;
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(lists); // Libera o vetor de listas

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

typedef struct Node {
    char filename[100];
    struct Node* next;
} Node;

// Função para criar um novo nó
Node* create_node(const char* name) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->filename, name);
    new_node->next = NULL;
    return new_node;
}

// Função para adicionar nó ao final da lista
void append_node(Node** head, const char* name) {
    Node* new_node = create_node(name);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

// Função para liberar a lista
void free_list(Node* head) {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Node* head = NULL;
    // Criando alguns arquivos fictícios
    append_node(&head, "arquivo1.txt");
    append_node(&head, "arquivo2.txt");
    append_node(&head, "arquivo3.txt");
    append_node(&head, "arquivo4.txt");
    append_node(&head, "arquivo5.txt");
    append_node(&head, "arquivo6.txt");
    append_node(&head, "arquivo7.txt");
    append_node(&head, "arquivo8.txt");
    append_node(&head, "arquivo9.txt");
    append_node(&head, "arquivo10.txt");

    Node* current = head;

    #pragma omp parallel
    {
        #pragma omp single
        {
            while (current != NULL) {
                Node* node = current;
                current = current->next;

                // Cria uma tarefa para cada nó
                #pragma omp task firstprivate(node)
                {
                    printf("Processando %s na thread %d\n", node->filename, omp_get_thread_num());
                }
            }
        }
    }

    free_list(head);
    return 0;
}


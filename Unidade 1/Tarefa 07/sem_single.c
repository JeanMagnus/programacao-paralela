#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define NUM_ARQUIVOS 20
#define TAM_NOME 50

typedef struct no {
    char nome_arquivo[TAM_NOME];
    struct no* prox;
} No;

No* criar_no(const char* nome) {
    No* novo = (No*)malloc(sizeof(No));
    strncpy(novo->nome_arquivo, nome, TAM_NOME);
    novo->prox = NULL;
    return novo;
}

void inserir_no(No** inicio, const char* nome) {
    No* novo = criar_no(nome);
    if (*inicio == NULL) {
        *inicio = novo;
    } else {
        No* atual = *inicio;
        while (atual->prox != NULL)
            atual = atual->prox;
        atual->prox = novo;
    }
}

void processar_no(No* no) {
    printf("Arquivo: %s | Thread: %d\n", no->nome_arquivo, omp_get_thread_num());
}

int main() {
    No* lista = NULL;

    for (int i = 0; i < NUM_ARQUIVOS; i++) {
        char nome[TAM_NOME];
        snprintf(nome, TAM_NOME, "arquivo_%d.txt", i + 1);
        inserir_no(&lista, nome);
    }

    No* atual = lista;

    // VERSÃO PROPOSITALMENTE QUEBRADA
    #pragma omp parallel
    {
        while (atual != NULL) {
            #pragma omp task
            {
                processar_no(atual);
            }
            atual = atual->prox;
        }
    }

    // Liberação da memória
    atual = lista;
    while (atual != NULL) {
        No* temp = atual;
        atual = atual->prox;
        free(temp);
    }

    return 0;
}

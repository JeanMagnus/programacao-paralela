#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Definição dos tamanhos da matriz e do número de repetições para maior precisão
#define MIN_SIZE 100
#define MAX_SIZE 2000
#define STEP 100
#define REPEATS 10

// Multiplicação matriz-vetor com acesso por linhas (linha externa, coluna interna)
void matrix_vector_mult_row_major(double *matrix, double *vector, double *result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = 0; // Inicializa o elemento do vetor resultado
        for (int j = 0; j < size; j++) {
            result[i] += matrix[i * size + j] * vector[j]; // Soma os produtos da linha da matriz com o vetor
        }
    }
}

// Multiplicação matriz-vetor com acesso por colunas (coluna externa, linha interna)
void matrix_vector_mult_col_major(double *matrix, double *vector, double *result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = 0; // Inicializa o vetor resultado
    }
    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++) {
            result[i] += matrix[i * size + j] * vector[j]; // Soma os produtos acessando a matriz por colunas
        }
    }
}

// Preenche a matriz com valores aleatórios entre 0 e 1
void fill_matrix(double *matrix, int size) {
    for (int i = 0; i < size * size; i++) {
        matrix[i] = (double)rand() / RAND_MAX;
    }
}

// Preenche o vetor com valores aleatórios entre 0 e 1
void fill_vector(double *vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = (double)rand() / RAND_MAX;
    }
}

// Retorna o tempo atual em segundos usando gettimeofday()
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main() {
    srand(42); // Inicializa o gerador de números aleatórios com uma seed fixa
    
    // Cabeçalho da tabela de resultados
    printf("===========================================\n");
    printf("| Matrix Size | Row-Major (s) | Col-Major (s) |\n");
    printf("===========================================\n");
    
    // Loop sobre diferentes tamanhos de matriz
    for (int size = MIN_SIZE; size <= MAX_SIZE; size += STEP) {
        // Alocação dinâmica de memória para matriz, vetor e vetor resultado
        double *matrix = (double *)malloc(size * size * sizeof(double));
        double *vector = (double *)malloc(size * sizeof(double));
        double *result = (double *)malloc(size * sizeof(double));
        
        fill_matrix(matrix, size); // Preenche a matriz com valores aleatórios
        fill_vector(vector, size); // Preenche o vetor com valores aleatórios
        
        double row_major_time = 0.0, col_major_time = 0.0;
        
        // Mede o tempo da multiplicação no formato Row-Major
        for (int r = 0; r < REPEATS; r++) {
            double start = get_time();
            matrix_vector_mult_row_major(matrix, vector, result, size);
            row_major_time += get_time() - start;
        }
        row_major_time /= REPEATS; // Calcula a média dos tempos
        
        // Mede o tempo da multiplicação no formato Col-Major
        for (int r = 0; r < REPEATS; r++) {
            double start = get_time();
            matrix_vector_mult_col_major(matrix, vector, result, size);
            col_major_time += get_time() - start;
        }
        col_major_time /= REPEATS; // Calcula a média dos tempos
        
        // Imprime os resultados formatados na tabela
        printf("| %11d | %13.6f | %13.6f |\n", size, row_major_time, col_major_time);
        
        // Libera a memória alocada
        free(matrix);
        free(vector);
        free(result);
    }
    printf("===========================================\n");
    return 0;
    // teste
    
}

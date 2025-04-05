#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

#define N 100000000  // Tamanho dos vetores
#define M 500        // Tamanho das matrizes

double timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main() {
    int threads_list[] = {1, 2, 4, 6, 8, 16, 32};
    int n_threads = sizeof(threads_list) / sizeof(threads_list[0]);

    printf("Nº Threads | Tempo CPU (s) | Tempo Memória (s) | Soma Final\n");
    printf("------------------------------------------------------------\n");

    for (int t = 0; t < n_threads; t++) {
        int threads = threads_list[t];
        omp_set_num_threads(threads);

        // --------- Memory-bound: soma de vetores ----------
        float *a = malloc(N * sizeof(float));
        float *b = malloc(N * sizeof(float));
        float *c = malloc(N * sizeof(float));

        for (int i = 0; i < N; i++) {
            a[i] = 1.0f;
            b[i] = 2.0f;
        }

        double start_mem = timestamp();

        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            c[i] = a[i] + b[i];
        }

        double end_mem = timestamp();
        double tempo_mem = end_mem - start_mem;

        free(a);
        free(b);
        free(c);

        // --------- CPU-bound: multiplicação de matrizes ----------
        double **A = malloc(M * sizeof(double *));
        double **B = malloc(M * sizeof(double *));
        double **C = malloc(M * sizeof(double *));
        for (int i = 0; i < M; i++) {
            A[i] = malloc(M * sizeof(double));
            B[i] = malloc(M * sizeof(double));
            C[i] = malloc(M * sizeof(double));
        }

        for (int i = 0; i < M; i++)
            for (int j = 0; j < M; j++) {
                A[i][j] = (i + j) % 100;
                B[i][j] = (i - j + 100) % 100;
                C[i][j] = 0.0;
            }

        double start_cpu = timestamp();

        #pragma omp parallel for
        for (int i = 0; i < M; i++)
            for (int j = 0; j < M; j++)
                for (int k = 0; k < M; k++)
                    C[i][j] += A[i][k] * B[k][j];

        double end_cpu = timestamp();
        double tempo_cpu = end_cpu - start_cpu;

        double soma_final = 0.0;
        for (int i = 0; i < M; i++)
            for (int j = 0; j < M; j++)
                soma_final += C[i][j];

        for (int i = 0; i < M; i++) {
            free(A[i]);
            free(B[i]);
            free(C[i]);
        }
        free(A); free(B); free(C);

        // --------- Saída formatada ----------
        printf("%10d | %13.6f | %17.6f | %10.2f\n",
               threads, tempo_cpu, tempo_mem, soma_final);
    }

    return 0;
}
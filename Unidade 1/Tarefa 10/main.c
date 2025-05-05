#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

// Função para medir o tempo com gettimeofday
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main() {
    int total_pontos = 1000000;
    int num_threads = omp_get_max_threads();
    int *acertos = (int*) calloc(num_threads, sizeof(int));

    // Versão 1 - rand_r com #pragma omp critical
    double start_v1 = get_time();
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        unsigned int seed = id;

        #pragma omp for
        for (int i = 0; i < total_pontos; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            if (x * x + y * y <= 1.0) {
                #pragma omp critical
                acertos[id]++;
            }
        }
    }

    int total_acertos_v1 = 0;
    for (int i = 0; i < num_threads; i++) {
        total_acertos_v1 += acertos[i];
    }
    double pi_v1 = 4.0 * total_acertos_v1 / total_pontos;
    double end_v1 = get_time();
    printf("Versão 1 (rand_r + critical): pi = %f | tempo = %f s\n", pi_v1, end_v1 - start_v1);

    // Reset
    for (int i = 0; i < num_threads; i++) acertos[i] = 0;

    // Versão 2 - rand_r com #pragma omp atomic
    double start_v2 = get_time();
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        unsigned int seed = id;

        #pragma omp for
        for (int i = 0; i < total_pontos; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            if (x * x + y * y <= 1.0) {
                #pragma omp atomic
                acertos[id]++;
            }
        }
    }

    int total_acertos_v2 = 0;
    for (int i = 0; i < num_threads; i++) {
        total_acertos_v2 += acertos[i];
    }
    double pi_v2 = 4.0 * total_acertos_v2 / total_pontos;
    double end_v2 = get_time();
    printf("Versão 2 (rand_r + atomic): pi = %f | tempo = %f s\n", pi_v2, end_v2 - start_v2);

    // Reset
    for (int i = 0; i < num_threads; i++) acertos[i] = 0;

    // Versão 3 - rand_r com reduction
    double start_v3 = get_time();
    int total_acertos_v3 = 0;
    #pragma omp parallel
    {
        unsigned int seed = omp_get_thread_num();

        #pragma omp for reduction(+:total_acertos_v3)
        for (int i = 0; i < total_pontos; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            if (x * x + y * y <= 1.0) {
                total_acertos_v3++;
            }
        }
    }

    double pi_v3 = 4.0 * total_acertos_v3 / total_pontos;
    double end_v3 = get_time();
    printf("Versão 3 (rand_r + reduction): pi = %f | tempo = %f s\n", pi_v3, end_v3 - start_v3);

    free(acertos);
    return 0;
}

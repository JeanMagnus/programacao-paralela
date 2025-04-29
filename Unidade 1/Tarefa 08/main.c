#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main() {
    int total_pontos = 1000000;
    int num_threads = omp_get_max_threads();
    int *acertos = (int*) calloc(num_threads, sizeof(int));

    // Versão 1 - Usando #pragma omp critical para acumular os acertos
    double start_time_v1 = get_time();
    #pragma omp parallel
    {
        int id = omp_get_thread_num(); // Cada thread descobre seu id
        unsigned int seed = id;         // Semente diferente por thread

        #pragma omp for
        for (int i = 0; i < total_pontos; i++) {
            double x = (double)rand() / RAND_MAX; // Usando rand()
            double y = (double)rand() / RAND_MAX; // Usando rand()

            if (x*x + y*y <= 1.0) {
                #pragma omp critical
                acertos[id]++;
            }
        }
    }

    int total_acertos_v1 = 0;
    for (int i = 0; i < num_threads; i++) {
        total_acertos_v1 += acertos[i];
    }
    double pi_estimado_v1 = 4.0 * total_acertos_v1 / total_pontos;
    double end_time_v1 = get_time();
    printf("Versão 1 (com rand()) - Estimativa de pi: %f\n", pi_estimado_v1);
    printf("Tempo de execução da Versão 1 com rand(): %f segundos\n", end_time_v1 - start_time_v1);

    // Resetando o vetor para a segunda versão
    for (int i = 0; i < num_threads; i++) {
        acertos[i] = 0;
    }

    // Versão 2 - Usando um vetor compartilhado para cada thread escrever seus acertos
    double start_time_v2 = get_time();
    #pragma omp parallel
    {
        int id = omp_get_thread_num(); // Cada thread descobre seu id
        unsigned int seed = id;         // Semente diferente por thread

        #pragma omp for
        for (int i = 0; i < total_pontos; i++) {
            double x = (double)rand() / RAND_MAX; // Usando rand()
            double y = (double)rand() / RAND_MAX; // Usando rand()

            if (x*x + y*y <= 1.0) {
                acertos[id]++;
            }
        }
    }

    int total_acertos_v2 = 0;
    for (int i = 0; i < num_threads; i++) {
        total_acertos_v2 += acertos[i];
    }
    double pi_estimado_v2 = 4.0 * total_acertos_v2 / total_pontos;
    double end_time_v2 = get_time();
    printf("Versão 2 (sem #pragma omp critical e com rand()) - Estimativa de pi: %f\n", pi_estimado_v2);
    printf("Tempo de execução da Versão 2 com rand(): %f segundos\n", end_time_v2 - start_time_v2);

    // Resetando o vetor para a comparação com rand_r()
    for (int i = 0; i < num_threads; i++) {
        acertos[i] = 0;
    }

    // Versão 1 - Usando rand_r()
    double start_time_v1_rand_r = get_time();
    #pragma omp parallel
    {
        int id = omp_get_thread_num(); // Cada thread descobre seu id
        unsigned int seed = id;         // Semente diferente por thread

        #pragma omp for
        for (int i = 0; i < total_pontos; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX; // Usando rand_r()
            double y = (double)rand_r(&seed) / RAND_MAX; // Usando rand_r()

            if (x*x + y*y <= 1.0) {
                #pragma omp critical
                acertos[id]++;
            }
        }
    }

    int total_acertos_v1_rand_r = 0;
    for (int i = 0; i < num_threads; i++) {
        total_acertos_v1_rand_r += acertos[i];
    }
    double pi_estimado_v1_rand_r = 4.0 * total_acertos_v1_rand_r / total_pontos;
    double end_time_v1_rand_r = get_time();
    printf("Versão 1 (com rand_r()) - Estimativa de pi: %f\n", pi_estimado_v1_rand_r);
    printf("Tempo de execução da Versão 1 com rand_r(): %f segundos\n", end_time_v1_rand_r - start_time_v1_rand_r);

    // Resetando o vetor para a segunda versão
    for (int i = 0; i < num_threads; i++) {
        acertos[i] = 0;
    }

    // Versão 2 - Usando rand_r()
    double start_time_v2_rand_r = get_time();
    #pragma omp parallel
    {
        int id = omp_get_thread_num(); // Cada thread descobre seu id
        unsigned int seed = id;         // Semente diferente por thread

        #pragma omp for
        for (int i = 0; i < total_pontos; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX; // Usando rand_r()
            double y = (double)rand_r(&seed) / RAND_MAX; // Usando rand_r()

            if (x*x + y*y <= 1.0) {
                acertos[id]++;
            }
        }
    }

    int total_acertos_v2_rand_r = 0;
    for (int i = 0; i < num_threads; i++) {
        total_acertos_v2_rand_r += acertos[i];
    }
    double pi_estimado_v2_rand_r = 4.0 * total_acertos_v2_rand_r / total_pontos;
    double end_time_v2_rand_r = get_time();
    printf("Versão 2 (sem #pragma omp critical e com rand_r()) - Estimativa de pi: %f\n", pi_estimado_v2_rand_r);
    printf("Tempo de execução da Versão 2 com rand_r(): %f segundos\n", end_time_v2_rand_r - start_time_v2_rand_r);

    free(acertos); // Libera a memória alocada
    return 0;
}

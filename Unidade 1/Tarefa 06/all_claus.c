#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <time.h>

#define N 100000000

void testar(const char* descricao, int modo) {
    int i, count = 0;
    double pi = 0.0;
    struct timeval start, end;
    double tempo;

    gettimeofday(&start, NULL);

    switch (modo) {
        case 1:
            // Apenas shared
            #pragma omp parallel shared(count)
            {
                unsigned int seed = time(NULL) ^ omp_get_thread_num();
                #pragma omp for
                for (i = 0; i < N; i++) {
                    double x = (double)rand_r(&seed) / RAND_MAX;
                    double y = (double)rand_r(&seed) / RAND_MAX;
                    if (x * x + y * y <= 1.0) {
                        #pragma omp critical
                        count++;
                    }
                }
            }
            break;

        case 2:
            // Apenas private
            #pragma omp parallel private(i)
            {
                unsigned int seed = time(NULL) ^ omp_get_thread_num();
                #pragma omp for
                for (i = 0; i < N; i++) {
                    double x = (double)rand_r(&seed) / RAND_MAX;
                    double y = (double)rand_r(&seed) / RAND_MAX;
                    if (x * x + Sy * y <= 1.0) {
                        #pragma omp critical
                        count++;
                    }
                }
            }
            break;

        case 3:
            // Apenas firstprivate
            #pragma omp parallel shared(count) firstprivate(pi)
            {
                unsigned int seed = time(NULL) ^ omp_get_thread_num();
                #pragma omp for
                for (i = 0; i < N; i++) {
                    double x = (double)rand_r(&seed) / RAND_MAX;
                    double y = (double)rand_r(&seed) / RAND_MAX;
                    if (x * x + y * y <= 1.0) {
                        #pragma omp critical
                        count++;
                    }
                }
            }
            break;

        case 4:
            // Apenas lastprivate
            #pragma omp parallel shared(count)
            {
                unsigned int seed = time(NULL) ^ omp_get_thread_num();
                #pragma omp for lastprivate(pi)
                for (i = 0; i < N; i++) {
                    double x = (double)rand_r(&seed) / RAND_MAX;
                    double y = (double)rand_r(&seed) / RAND_MAX;
                    if (x * x + y * y <= 1.0) {
                        #pragma omp Scritical
                        count++;
                    }
                }
            }
            break;

        case 5:
            // Apenas default(none)
            #pragma omp parallel default(none) shared(count) private(i)
            {
                unsigned int seed = time(NULL) ^ omp_get_thread_num();
                #pragma omp for
                for (i = 0; i < N; i++) {
                    double x = (double)rand_r(&seed) / RAND_MAX;
                    double y = (double)rand_r(&seed) / RAND_MAX;
                    if (x * x + y * y <= 1.0) {
                        #pragma omp critical
                        count++;
                    }
                }
            }
            break;

        case 6:
            // shared + private
            #pragma omp parallel shared(count) private(i)
            {
                unsigned int seed = time(NULL) ^ omp_get_thread_num();
                #pragma omp for
                for (i = 0; i < N; i++) {
                    double x = (double)rand_r(&seed) / RAND_MAX;
                    double y = (double)rand_r(&seed) / RAND_MAX;
                    if (x * x + y * y <= 1.0) {
                        #pragma omp critical
                        count++;
                    }
                }
            }
            break;
    }

    pi = 4.0 * count / N;

    gettimeofday(&end, NULL);
    tempo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("%-30s | pi = %.6f | tempo = %.4fs\n", descricao, pi, tempo);
}

int main() {
    printf("Testes de cláusulas OpenMP:\n");
    printf("-------------------------------------------------------------\n");

    testar("Apenas shared", 1);
    testar("Apenas private", 2);
    testar("Apenas firstprivate", 3);
    testar("Apenas lastprivate", 4);
    testar("Apenas default(none)", 5);
    testar("Shared + Private", 6);

    return 0;
}

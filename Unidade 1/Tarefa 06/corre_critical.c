#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main() {
    int i, N = 1000000;
    int count = 0;

    #pragma omp parallel
    {
        unsigned int seed = time(NULL) ^ omp_get_thread_num(); // semente única por thread

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

    double pi = 4.0 * count / N;
    printf("Estimativa de pi: %f\n", pi);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main() {
    int i, N = 1000000;
    int count = 0;
    double x, y;

    srand(time(NULL)); // semente do gerador de números aleatórios

    #pragma omp parallel for
    for (i = 0; i < N; i++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            count++;
        }
    }

    double pi = 4.0 * count / N;
    printf("Estimativa de pi: %f\n", pi);

    return 0;
}

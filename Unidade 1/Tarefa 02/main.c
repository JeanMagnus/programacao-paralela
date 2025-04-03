#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 1000000

double array[SIZE];

// Função para calcular o tempo decorrido
double get_time_elapsed(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

double initialize_array() {
    for (int i = 0; i < SIZE; i++) {
        array[i] = i * 0.5;
    }
    return array[SIZE - 1]; // Evita otimização
}

double sum_sequential() {
    double sum = 0.0;
    for (int i = 0; i < SIZE; i++) {
        sum += array[i];
    }
    printf("Resultado da soma sequencial: %f\n", sum); // Evita otimização
    return sum;
}

double sum_unrolled() {
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    for (int i = 0; i < SIZE; i += 4) {
        sum1 += array[i];
        sum2 += array[i + 1];
        sum3 += array[i + 2];
        sum4 += array[i + 3];
    }
    double total_sum = sum1 + sum2 + sum3 + sum4;
    printf("Resultado da soma desenrolada: %f\n", total_sum); // Evita otimização
    return total_sum;
}

int main() {
    struct timeval start, end;
    double elapsed_time;

    // Tempo de inicialização
    gettimeofday(&start, NULL);
    initialize_array();
    gettimeofday(&end, NULL);
    elapsed_time = get_time_elapsed(start, end);
    printf("Tempo de inicializaçao: %f s\n", elapsed_time);

    // Soma sequencial
    gettimeofday(&start, NULL);
    sum_sequential();
    gettimeofday(&end, NULL);
    elapsed_time = get_time_elapsed(start, end);
    printf("Soma sequencial: %f s\n", elapsed_time);

    // Soma desenrolada
    gettimeofday(&start, NULL);
    sum_unrolled();
    gettimeofday(&end, NULL);
    elapsed_time = get_time_elapsed(start, end);
    printf("Soma desenrolada: %f s\n", elapsed_time);

    return 0;
}

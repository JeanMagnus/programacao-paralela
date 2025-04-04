#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Função para calcular uma aproximação de pi usando a série de Leibniz
double calcular_pi(long num_iteracoes) {
    double pi_aproximado = 0.0;
    for (long k = 0; k < num_iteracoes; k++) {
        pi_aproximado += (k % 2 == 0 ? 1.0 : -1.0) / (2.0 * k + 1.0);
    }
    return 4.0 * pi_aproximado;
}

int main() {
    // Lista de números de iterações a testar
    long iteracoes[] = {1000, 10000, 100000, 1000000, 10000000};
    int n_tests = sizeof(iteracoes) / sizeof(iteracoes[0]);

    printf("Aproximacao de PI usando a serie de Leibniz\n\n");
    printf("%12s%20s%20s%20s\n", "Iteracoes", "PI Aproximado", "Erro Absoluto", "Tempo (ms)");

    for (int i = 0; i < n_tests; i++) {
        long n = iteracoes[i];
        
        struct timeval inicio, fim;
        gettimeofday(&inicio, NULL); // Tempo de início

        double pi_calc = calcular_pi(n);

        gettimeofday(&fim, NULL); // Tempo de fim

        // Calcula o tempo em milissegundos
        double tempo_execucao = (fim.tv_sec - inicio.tv_sec) * 1000.0;
        tempo_execucao += (fim.tv_usec - inicio.tv_usec) / 1000.0;

        double erro = fabs(M_PI - pi_calc);

        printf("%12ld%20.15f%20.15f%20.3f\n", n, pi_calc, erro, tempo_execucao);
    }

    return 0;
}

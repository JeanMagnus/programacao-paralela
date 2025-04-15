#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

// Verifica se um número é primo
int eh_primo(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

// Calcula tempo decorrido em microsegundos
double tempo_execucao(struct timeval start, struct timeval end) {
    return (double)(end.tv_sec - start.tv_sec) * 1e6 + (double)(end.tv_usec - start.tv_usec);
}

int main() {
    int n;
    printf("Digite o valor de n: ");
    scanf("%d", &n);

    // ================= SEQUENCIAL =================
    struct timeval start_seq, end_seq;
    int total_primos_seq = 0;

    gettimeofday(&start_seq, NULL);
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) {
            total_primos_seq++;
        }
    }
    gettimeofday(&end_seq, NULL);

    // ================= PARALELO =================
    struct timeval start_par, end_par;
    int total_primos_par = 0;

    int num_threads = omp_get_max_threads();  // Sem omp single
    int* contadores_locais = calloc(num_threads, sizeof(int));

    gettimeofday(&start_par, NULL);

    #pragma omp parallel for
    for (int i = 2; i <= n; i++) {
        int tid = omp_get_thread_num();
        if (eh_primo(i)) {
            contadores_locais[tid]++;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        total_primos_par += contadores_locais[i];
    }

    gettimeofday(&end_par, NULL);
    free(contadores_locais);

    // ================= RESULTADOS =================
    double tempo_seq = tempo_execucao(start_seq, end_seq) / 1e6;
    double tempo_par = tempo_execucao(start_par, end_par) / 1e6;

    printf("\n===== RESULTADOS =====\n");
    printf("Total de primos (Sequencial): %d\n", total_primos_seq);
    printf("Tempo (Sequencial): %.6f segundos\n", tempo_seq);

    printf("Total de primos (Paralelo):   %d\n", total_primos_par);
    printf("Tempo (Paralelo):   %.6f segundos\n", tempo_par);

    return 0;
}

#include <stdio.h>
#include <time.h> 
#include <pthread.h>

#define OBJETIVO 100000000

void executar_sequencial() {
    long long contador = 0;
    
    for (long long i = 0; i < OBJETIVO; i++) {
        contador++;
    }
    
    printf("Versao Sequencial -> Contador final: %lld\n", contador);
}

int main() {
    struct timespec inicio, fim;
    double tempo_execucao;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    executar_sequencial();

    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo_execucao = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("Tempo de execucao: %f segundos\n", tempo_execucao);

    return 0;
}

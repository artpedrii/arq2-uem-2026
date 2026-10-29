#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define N 1024

double A[N][N];
double B[N][N];
double C[N][N];

void carregar_matriz(const char *nome_arquivo, double matriz[N][N]) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: arquivo %s nao encontrado.\n", nome_arquivo);
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(arquivo, "%lf", &matriz[i][j]);
        }
    }
    fclose(arquivo);
}

//versão sequencial
void multiplicar_sequencial() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0; 
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    //tipagem
    struct timespec inicio, fim;
    double tempo_sequencial;
    //carregando as matrizes
    carregar_matriz("A_512.txt", A);
    carregar_matriz("B_512.txt", B);
    //calcula tempo sequencial
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    multiplicar_sequencial();
    clock_gettime(CLOCK_MONOTONIC, &fim);
    tempo_sequencial = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("Tempo sequencial (%dx%d): %f segundos\n", N, N, tempo_sequencial);
    //**PARALELA *//
    return 0;
}
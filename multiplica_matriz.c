#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define N 512

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

//versão paralela
typedef struct {
    int id_thread;
    int linha_inicio;
    int linha_fim;
} InfoThread;

void* multiplicar_paralelizada(void* arg) {
    InfoThread *info = (InfoThread*) arg;
    for (int i = info->linha_inicio; i < info->linha_fim; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
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
    // **VERSÃO PARALELA DA MULTIPLICAÇÃO** //
    int num_threads = 4; 
    pthread_t threads[8];
    InfoThread info[8];
    int linhas_por_thread = N / num_threads;
    // Inicia o cronômetro para a versão paralela
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    // 1. Cria as threads distribuindo as fatias de linhas
    for (int i = 0; i < num_threads; i++) {
        info[i].id_thread = i;
        info[i].linha_inicio = i * linhas_por_thread;
        info[i].linha_fim = (i == num_threads - 1) ? N : (i + 1) * linhas_por_thread;

        pthread_create(&threads[i], NULL, multiplicar_paralelizada, (void*)&info[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_paralelo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("Tempo paralelo (%d threads, %dx%d): %f segundos\n", num_threads, N, N, tempo_paralelo);
    return 0;
}
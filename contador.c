//imports
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define OBJETIVO 100000000

//implementação sequencial 
void executar_sequencial() {
    long long contador = 0;
    for (long long i = 0; i < OBJETIVO; i++) {
        contador++;
    }
    printf("Versao Sequencial -> Contador final: %lld\n", contador);
}

//implementação paralela A
long long contador_compartilhado = 0;
pthread_mutex_t cadeado; 

void* contar_com_mutex(void* arg) {
    long long incrementos_desta_thread = *(long long*)arg; 
    for (long long i = 0; i < incrementos_desta_thread; i++) {
        pthread_mutex_lock(&cadeado);
        contador_compartilhado++;
        pthread_mutex_unlock(&cadeado);
    }
    return NULL;
}

//implementação paralela B

int main() {
    //criação de variáveis e tipagem
    struct timespec inicio, fim;
    double tempo_execucao_sequencial;
    double tempo_execucao_paralelaA;
    int numero_threads = 8;

    //**SEQUENCIAL**//
    //inicia o cálculo do tempo da versão sequencial
    clock_gettime(CLOCK_MONOTONIC, &inicio); 
    executar_sequencial();
    clock_gettime(CLOCK_MONOTONIC, &fim);
    //finaliza o cálculo do tempo da versão sequencial
    tempo_execucao_sequencial = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9; //calcula e divide por 1 bilhão para termos os segundos normais.
    printf("Tempo de execucao da versão sequencial: %f segundos\n", tempo_execucao_sequencial); //imprime o tempo da versão sequencial.

    //**PARALELA A**//
    //inicializa o MUTEX
    pthread_mutex_init(&cadeado, NULL);
    //calcula a fatia de cada thread
    long long incrementos_por_thread = OBJETIVO / numero_threads;
    //cria espaços para as threads
    pthread_t threads[8];
    //inicia o cálculo do tempo da versão paralela A
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    for (int i = 0; i < numero_threads; i++) {
        pthread_create(&threads[i], NULL, contar_com_mutex, &incrementos_por_thread);
    }
    for (int i = 0; i < numero_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);
    //finaliza o cálculo do tempo da versão paralela A
    tempo_execucao_paralelaA = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9; //calculo do tempo em segundos da versão paralela A.
    printf("Tempo de execucao da versão paralela A (com %d threads): %f segundos\n", numero_threads, tempo_execucao_paralelaA); //imprime o tempo da versão paralela A.

    return 0;
}

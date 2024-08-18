#ifndef MEMORIA_H
#define MEMORIA_H


#include "./processo.h"

// TODO: Diminuir tamanho da memoria para testes de algoritmo de troca de processos
// first fit, next fit, best fit e worst fit
// TODO: Implementar métricas

// Existem três parâmetros de desempenho que a sua simulação deve calcular para todas as quatro técnicas: 
// - número médio de fragmentos externos;
// - tempo médio de alocação considerando o número médio de nós percorridos na alocação;
// - número de vezes que um processo teve que ser levado para o "disco", para liberar espaço na memória;


// Definição da estrutura Memoria, que representa a memória principal do sistema
typedef struct {
    int memoriaPrincipal[TAM_MEMORIA];  // Array que armazena os dados na memória principal
    int tamanho;                        // Tamanho atual da memória (número de elementos armazenados)
} Memoria;

typedef struct 
{
    int bitmap[TAM_MEMORIA];
    int tamanho;
} MapaDeBits;

typedef struct
{
    int id;
    int quantidadeVariaveis;
    ProcessosNaMemoria *proximo;
}ProcessosNaMemoria;

typedef struct
{
    ProcessosNaMemoria *primeiro;
    ProcessosNaMemoria *ultimo;
    int tamanho;
}ProcessosNaMemoriaLista;

int alocarMemoriaFirstFit(MapaDeBits *mapa, ProcessosNaMemoriaLista *lista, int tamanho, int idProcesso);
void printMemoria(Memoria *memoria);
void swapParaDisco(Memoria *memoria, MapaDeBits *mapaDeBits, int idProcesso);
void recuperarDoDisco(Memoria *memoria, MapaDeBits *mapaDeBits, int idProcesso);
void iniciarMapaDeBits(MapaDeBits *mapaDeBits);
void iniciarMemoria(Memoria *memoria);
void atualizarMapaDeBits(MapaDeBits *mapa, int inicio, int tamanho, int valor);
void iniciarProcessosNaMemoriaLista(ProcessosNaMemoriaLista *processosNaMemoriaLista);
int localizarBlocoLivre(MapaDeBits *mapa, int tamanho, int *inicio);
void adicionarProcesso(ProcessosNaMemoriaLista *lista, int id);
void removerProcesso(ProcessosNaMemoriaLista *lista, int id);
void desalocarMemoriaFirstFit(ProcessosNaMemoriaLista *lista, MapaDeBits *mapa, int tamanho);

#endif // MEMORIA_H


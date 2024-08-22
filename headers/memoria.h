#ifndef MEMORIA_H
#define MEMORIA_H


#include "./tabela_de_processos.h"

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
} Memoria;

typedef struct 
{
    int bitmap[TAM_MEMORIA];
    int tamanho;
} MapaDeBits;

typedef struct ProcessosNaMemoria
{
    int id;
    int quantidadeVariaveis;
    struct ProcessosNaMemoria *proximo;
}ProcessosNaMemoria;

typedef struct
{
    ProcessosNaMemoria *primeiro;
    ProcessosNaMemoria *ultimo;
    int tamanho;
}ProcessosNaMemoriaLista;

void alocarMemoriaFirstFit(Memoria *memoria, MapaDeBits *mapa, ProcessosNaMemoriaLista *lista, int tamanho, ProcessoSimulado *processo, tabelaProcessos *tabela);
void desalocarMemoriaFirstFit(Memoria *memoria, ProcessosNaMemoriaLista *lista, MapaDeBits *mapa, int tamanho, tabelaProcessos *tabela);
void printMemoriaPreenchida(Memoria *memoria, MapaDeBits *mapa);
void swapParaDisco(Memoria *memoria, MapaDeBits *mapaDeBits, ProcessoSimulado *processo);
void recuperarDoDisco(Memoria *memoria, MapaDeBits *mapaDeBits, ProcessoSimulado *processo);
void iniciarMapaDeBits(MapaDeBits *mapaDeBits);
void atualizarMapaDeBits(MapaDeBits *mapa, int inicio, int tamanho, int valor);
void iniciarProcessosNaMemoriaLista(ProcessosNaMemoriaLista *processosNaMemoriaLista);
int localizarBlocoLivre(MapaDeBits *mapa, int tamanho, int *inicio);
void adicionarProcesso(ProcessosNaMemoriaLista *lista, int id);
void removerProcesso(ProcessosNaMemoriaLista *lista, int id);
int isProcessoNaMemoria(ProcessosNaMemoriaLista *lista, int id);

#endif // MEMORIA_H


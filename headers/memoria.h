#ifndef MEMORIA_H
#define MEMORIA_H


#include "./tabela_de_processos.h"
#include "./Desempenho.h"
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
} MapaDeBits;

void alocarMemoriaFirstFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *proceso, tabelaProcessos *tabela, Desempenho * desempenho);
int desalocarMemoriaFirstFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int tamanho, tabelaProcessos *tabela ,Desempenho * desempenho);
void alocarMemoriaNextFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *processo, tabelaProcessos *tabela, int *ultimaPosicaoAlocacao, Desempenho * desempenho);
int desalocarMemoriaNextFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int tamanho, tabelaProcessos *tabela, int *ultimaPosicaoAlocacao, Desempenho * desempenho);
int localizarBlocoLivreNextFit(MapaDeBits *mapa, int tamanho, int *inicio, int ultimaPosicaoAlocacao, Desempenho * desempenho);
void alocarMemoriaWorstFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *processo, tabelaProcessos *tabela, Desempenho * desempenho);
int desalocarMemoriaWorstFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int idProcesso, tabelaProcessos *tabela, Desempenho * desempenho);
void alocarMemoriaBestFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *processo, tabelaProcessos *tabela, Desempenho * desempenho);
int desalocarMemoriaBestFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int idProcesso, tabelaProcessos *tabela, Desempenho * desempenho);
void printMemoriaPreenchida(Memoria *memoria, MapaDeBits *mapa);
void swapParaDisco(Memoria *memoria, MapaDeBits *mapaDeBits, ProcessoSimulado *processo, Desempenho *desempenho);
void recuperarDoDisco(Memoria *memoria, MapaDeBits *mapaDeBits, ProcessoSimulado *processo);
void iniciarMapaDeBits(MapaDeBits *mapaDeBits);
void atualizarMapa(MapaDeBits *mapa, int inicio, int tamanho, int valor);
void iniciarProcessosNaMemoriaLista(FilaDinamica *lista);
int localizarBlocoLivre(MapaDeBits *mapa, int tamanho, int *inicio,Desempenho * desempenho);
void adicionarProcesso(FilaDinamica *lista, int id, int tamanho);
void removerProcesso(FilaDinamica *lista, int id);
int isProcessoNaMemoria(FilaDinamica *lista, int id);
void printMapaDeBits(MapaDeBits *mapa);
int localizarBlocoLivreWorstFit(MapaDeBits *mapa, int tamanho, int *inicio, Desempenho *desempenho);

#endif // MEMORIA_H


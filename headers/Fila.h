#ifndef FILADINAMICA_H
#define FILADINAMICA_H

#include "./tempo.h"

// Estrutura que representa um nó na fila dinâmica.
typedef struct node
{
    int dado;              // Valor armazenado no nó.
    struct node *proximo;  // Ponteiro para o próximo nó na fila.
} Node;

// Estrutura que representa a fila dinâmica em si.
typedef struct
{
    Node *frente;          // Ponteiro para o primeiro nó (frente) da fila.
    Node *tras;            // Ponteiro para o último nó (trás) da fila.
    int tamanho;           // Número de elementos atualmente na fila.
} FilaDinamica;

// Funções para manipulação da fila dinâmica
//---------------------------------------------------------------------------------------

// Inicializa a fila dinâmica, configurando os ponteiros e o tamanho.
void inicializarFilaDinamica(FilaDinamica *fila);

// Verifica se a fila dinâmica está vazia. Retorna 1 se vazia, caso contrário, 0.
int isFilaDinamicaVazia(FilaDinamica *fila);

// Adiciona um novo item ao final da fila.
void enfileirarDinamica(FilaDinamica *fila, int item);

// Remove e retorna o item da frente da fila.
int desenfileirarDinamica(FilaDinamica *fila);

// Retorna o valor armazenado na frente da fila sem removê-lo.
int frenteFilaDinamica(FilaDinamica *fila);

// Libera toda a memória associada aos nós da fila.
void liberarFilaDinamica(FilaDinamica *fila);

// Imprime todos os elementos da fila.
void imprimirFilaDinamica(FilaDinamica *fila);

void removerNo(FilaDinamica *fila, Node *no);
//---------------------------------------------------------------------------------------

#endif

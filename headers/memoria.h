#ifndef MEMORIA_H
#define MEMORIA_H

#include "./instrucao.h"

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

void alocarMemoriaProcesso(Memoria *memoria, int numVariaveis);
void printMemoria(Memoria *memoria);

#endif // MEMORIA_H


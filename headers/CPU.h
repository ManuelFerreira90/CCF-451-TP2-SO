#ifndef CPU_H
#define CPU_H

#include "./processo.h"

// Estrutura que representa uma CPU simulada.
typedef struct CPU
{
    int contadorPrograma; // Contador de programa: armazena o endereço da próxima instrução a ser executada.
    int *memoriaVect;     // Ponteiro para o vetor de memória alocado dinamicamente.
    Tempo fatiaTempo;     // Tempo máximo que um processo pode usar a CPU antes de ser interrompido.
    Tempo tempoUsado;     // Tempo já utilizado pelo processo em execução.
    int quantidadeInteiros; // Quantidade de inteiros armazenados na memória.
    ProcessoSimulado *processoEmExecucao; // Ponteiro para o processo que está atualmente em execução na CPU.
} CPU;

// Funções associadas à CPU
//---------------------------------------------------------------------------------------

// Inicializa os atributos de uma estrutura CPU.
void iniciarCPU(CPU *cpu);

// Imprime o estado atual da CPU em um formato legível.
void imprimeCPU(CPU cpu, int index);

// Imprime a borda da tabela de estado da CPU.
void printTableBorder();

//---------------------------------------------------------------------------------------

#endif

#ifndef PROCESSO_H
#define PROCESSO_H

#include "./Fila.h"

// Enumeração que define os possíveis estados de um processo.
typedef enum
{
    Bloqueado = 0, // O processo está bloqueado, aguardando um evento para continuar.
    Pronto = 1,    // O processo está pronto para ser executado pela CPU.
    Execucao = 2,  // O processo está atualmente em execução na CPU.
} Estados;

// Estrutura que representa um processo simulado no sistema.
typedef struct
{
    pid_t ID_Processo;          // Identificador único do processo.
    pid_t ID_Processo_Pai;      // Identificador do processo pai.
    int PC;                     // Contador de programa (Program Counter), indicando a próxima instrução a ser executada.
    Estados EstadosProcesso;    // Estado atual do processo (e.g., Bloqueado, Pronto, Execução).
    int prioridade;             // Prioridade do processo, usada quando o escalonamento é baseado em prioridade.
    int *memoria;               // Ponteiro para a memória alocada para o processo.
    int quantidadeInteiros;     // Quantidade de inteiros alocados na memória do processo.
    char *conjuntoInstrucoes;   // Conjunto de instruções que o processo deve executar.
    Tempo tempoInicio;          // Tempo em que o processo foi iniciado.
    Tempo tempoCPU;             // Tempo total de CPU utilizado pelo processo.
    Tempo tempoBloqueado;       // Tempo total que o processo passou bloqueado.
    int quantum;                // Tempo máximo que o processo pode usar a CPU antes de ser interrompido.
    int quantidadeInstrucao;    // Quantidade de instruções que o processo possui.
} ProcessoSimulado;

// Declarações de funções para manipulação de processos simulados
//---------------------------------------------------------------------------------------

// Inicializa um novo processo com base nas instruções e parâmetros fornecidos.
ProcessoSimulado *inicializaProcesso(char *conjuntoInstrucoes, int quantidadeInstrucao, int PID_pai, int id, int algoritmoEscalonamento);

// Cria um novo processo a partir de um processo pai, herdando suas características.
ProcessoSimulado *criarNovoProcessoAPartirdoPai(ProcessoSimulado *processoPai, int ID_processo);

// Imprime as informações detalhadas de um processo simulado.
void imprimirProcesso(ProcessoSimulado *processo);

// Retorna String para representar a prioridade do processo
char* getInfoPrioridade(int prioridade);

//---------------------------------------------------------------------------------------

#endif // PROCESSO_H



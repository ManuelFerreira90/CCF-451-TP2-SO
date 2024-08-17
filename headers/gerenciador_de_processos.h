#ifndef GERENCIADOR_PROCESSOS_H
#define GERENCIADOR_PROCESSOS_H

#include "./tabela_de_processos.h"

// Estrutura que agrupa filas de processos prontos e bloqueados, organizadas por prioridades.
typedef struct
{
    FilaDinamica filasProntos[NUM_PRIORIDADES];   // Filas de processos prontos, uma para cada nível de prioridade.
    FilaDinamica filasBloqueados[NUM_PRIORIDADES]; // Filas de processos bloqueados, uma para cada nível de prioridade.
} EstruturaFilaPrioridades;

// Estrutura para armazenar filas de processos prontos e bloqueados no algoritmo Round Robin.
typedef struct
{
    FilaDinamica filaPronto;   // Fila de processos prontos para execução.
    FilaDinamica filaBloqueado; // Fila de processos bloqueados.
} EstruturaRoundRobin;

// União que permite escolher entre diferentes estruturas de escalonamento (fila de prioridades ou Round Robin).
typedef union
{
    EstruturaFilaPrioridades filaPrioridades; // Estrutura para escalonamento por prioridades.
    EstruturaRoundRobin roundRobin;            // Estrutura para escalonamento Round Robin.
} EstruturaEscalonamento;

// Estrutura principal que gerencia todos os processos e recursos de uma simulação de sistema operacional.
typedef struct
{
    tabelaProcessos TabelaProcessos;               // Tabela que armazena todos os processos simulados.
    EstruturaEscalonamento EstruturaEscalonamento; // Estrutura de escalonamento selecionada (prioridades ou Round Robin).
    Tempo tempoAtual;                              // Tempo atual da simulação.
    CPU *cpus;                                     // Ponteiro para um array de CPUs simuladas.
    int quantidadeCPUs;                            // Número de CPUs disponíveis na simulação.
    int algoritmoEscalonamento;                    // Algoritmo de escalonamento em uso: 0 para fila de prioridades, 1 para Round Robin.
    int *processosEmExecucao;                      // Array que indica quais processos estão em execução em cada CPU.
    Tempo tempoMedio;                              // Tempo médio de execução dos processos.
    int processosTerminados;                       // Contador de processos que já terminaram a execução.
} GerenciadorProcessos;

// Declarações de funções para gerenciar processos na simulação
//---------------------------------------------------------------------------------------

// Inicializa o gerenciador de processos com os parâmetros fornecidos.
void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada, int PID_Pai, int numsCPUs, int escalonador);

// Adiciona um processo à fila de prioridade de prontos.
void adicionarProcessoProntoFilaDePrioridade(GerenciadorProcessos *gerenciador, int processoIndex);

// Adiciona um processo à fila de prioridade de bloqueados.
void adicionarProcessoBloqueadoFilaDePrioridade(GerenciadorProcessos *gerenciador, int processoIndex);

// Processa um comando de instrução para um processo específico.
void processarComando(GerenciadorProcessos *gerenciador, Instrucao instrucao, int indexCPU);

// Executa o escalonador de processos com base em fila de prioridades.
void escalonadorFilaDePrioridades(GerenciadorProcessos *gerenciador);

// Coloca um processo na CPU conforme a fila de prioridades.
void colocaProcessoNaCPUFilaDePrioridades(GerenciadorProcessos *gerenciador, int cpuIndex);

// Realiza a execução de um processo na CPU.
void executandoProcessoCPU(GerenciadorProcessos *gerenciador);

// Realiza a troca de contexto entre processos.
int trocaDeContexto(GerenciadorProcessos *gerenciador, int i);

// Atualiza os dados de um processo em execução na CPU.
void atualizaDadosProcesso(CPU *cpu);

// Imprime as instruções de um processo simulado.
void imprimirInstrucoes(ProcessoSimulado *processo);

// Inicializa o escalonador Round Robin.
void iniciarRoundRobin(GerenciadorProcessos *gerenciador);

// Inicializa o escalonador por fila de prioridades.
void iniciarFilaDePrioridades(GerenciadorProcessos *gerenciador);

// Processa uma linha específica de um arquivo de entrada.
Instrucao processarLinhaEspecifica(char *caminhoArquivo, int numeroLinha);

// Imprime o tempo médio dos processos ao final da simulação.
void imprimirTempoMedioProcessos(GerenciadorProcessos gerenciador);

// Imprime uma instrução de forma simplificada.
void printInstrucaoSimplificada(Instrucao instrucao, int cpuIndex, int processoId);

// Inicializa o vetor de memória de uma CPU.
void iniciarVetorMemoria(CPU *cpu);

// Comandos de controle de simulação (Exemplos: B, D, N, V, A, S, F, R, T)
void comandoB(GerenciadorProcessos *gerenciador, int indexCPU, int valor);
void comandoD(CPU *cpu, int index);
void comandoN(CPU *cpu, int valor);
void comandoV(CPU *cpu, int index, int valor);
void comandoA(CPU *cpu, int index, int valor);
void comandoS(CPU *cpu, int index, int valor);
void comandoF(GerenciadorProcessos *gerenciador, int index, int valor);
void comandoR(CPU *cpu, Instrucao instrucao);
void comandoT(GerenciadorProcessos *gerenciador, int indexCPU);

// Funções auxiliares para leitura e manipulação de arquivos e dados.
void lerArquivo(char *arquivoEntrada);
void lerTerminal(char *retorno);
void remove_char(char *str, char garbage);

// Incrementa o tempo de uso da CPU.
void incrementarTempoCPU(GerenciadorProcessos *gerenciador);

// Funções de troca de contexto específicas para cada algoritmo de escalonamento.
void trocaDeContextoFilaDePrioridade(GerenciadorProcessos *gerenciador);
void escalonadorRoundRobin(GerenciadorProcessos *gerenciador);
void trocaDeContextoRoundRobin(GerenciadorProcessos *gerenciador);
void colocaProcessoNaCPURoundRobin(GerenciadorProcessos *gerenciador, int cpuIndex);

// Imprime informações detalhadas sobre todos os processos e filas.
void imprimirTodosProcessos(GerenciadorProcessos *gerenciador);
void imprimirFilas(GerenciadorProcessos *gerenciador);

// Retorna o tempo de fatia de tempo para um processo com base na prioridade.
int getFatiaTempoPrioridade(int prioridade);
void verificarBloqueadosFilaDePrioridades(GerenciadorProcessos *gerenciador);
void verificarBloqueadosRoundRobin(GerenciadorProcessos *gerenciador);
//---------------------------------------------------------------------------------------

#endif // GERENCIADOR_PROCESSOS_H

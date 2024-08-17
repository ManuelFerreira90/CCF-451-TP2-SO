#ifndef TABELA_DE_PROCESSOS_H
#define TABELA_DE_PROCESSOS_H

#include "./CPU.h"

// Estrutura que representa uma tabela de processos no sistema.
typedef struct tabelaProcessos
{
    ProcessoSimulado *listaProcessos[MAX_PROCESSOS]; // Lista de ponteiros para processos simulados.
    int primeiroProcessoIndex;                       // Índice do primeiro processo na tabela.
    int ultimoProcessoIndex;                         // Índice do último processo na tabela.
} tabelaProcessos;

// Funções de gerenciamento da tabela de processos
// Declarações de funções e estruturas
//---------------------------------------------------------------------------------------

// Verifica se a tabela de processos está vazia.
// Retorna 1 se a tabela estiver vazia, 0 caso contrário.
int isTabelaProcessosVazia(tabelaProcessos *tabela);

// Verifica se a tabela de processos está cheia.
// Retorna 1 se a tabela estiver cheia, 0 caso contrário.
int isTabelaProcessosCheia(tabelaProcessos *tabela);

// Retorna o tamanho atual da tabela de processos (número de processos presentes).
int getTamanhoTabelaProcessos(tabelaProcessos *tabela);

// Inicializa a tabela de processos, configurando os índices e limpando a lista de processos.
void inicializarTabelaProcessos(tabelaProcessos *tabela);

// Insere um novo processo na tabela de processos.
// Adiciona o processo no final da tabela, se houver espaço disponível.
void inserirTabelaProcessos(ProcessoSimulado *processo, tabelaProcessos *tabela);

// Remove um processo da tabela de processos com base no seu ID.
// Ajusta os índices e a lista de processos após a remoção.
void retirarTabelaProcessos(tabelaProcessos *tabela, int ID_Processo);

// Obtém os índices dos processos com um determinado estado.
// Armazena os índices em uma lista e retorna o tamanho da lista.
int *getIndicesEstadoTabelaProcessos(tabelaProcessos *tabela, Estados estado, int *tamanhoLista);

// Imprime as informações de um processo simulado.
// Exibe detalhes do processo com base no algoritmo de escalonamento utilizado.
void imprimeProcesso(ProcessoSimulado *processo, int algoritmoEscalonamento);

// Imprime todos os processos na tabela de processos.
// Exibe detalhes de cada processo com base no algoritmo de escalonamento utilizado.
void imprimeTabelaProcessos(tabelaProcessos *tabela, int algoritmoEscalonamento);

// Converte um estado de processo em uma string correspondente.
// Retorna a string representando o estado.
const char *estadoToString(Estados estado);

// Obtém um processo da tabela de processos com base no índice fornecido.
// Retorna o ponteiro para o processo simulado correspondente.
ProcessoSimulado *getProcesso(tabelaProcessos *tabela, int indice);

//---------------------------------------------------------------------------------------

#endif // TABELA_DE_PROCESSOS_H

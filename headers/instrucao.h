#ifndef INSTRUCAO_H
#define INSTRUCAO_H

// Definições de constantes usadas no sistema de gerenciamento de processos.
#define MAX_PROCESSOS 5000  // Número máximo de processos que o sistema pode gerenciar.
#define TAMANHO_MEMORIA 100 // Tamanho da memória alocada para cada processo.
#define TABLE_WIDTH 40      // Largura da tabela usada para exibição de dados.
#define MAX_CMD_LEN 100     // Tamanho máximo permitido para comandos de entrada.
#define NUM_PRIORIDADES 4   // Número de níveis de prioridade para o escalonador de processos.
#define NUM_CPUs 4          // Número de CPUs disponíveis para execução dos processos.
#define QUANTUM 5           // Quantum de tempo utilizado no escalonador Round Robin.

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>    // Incluído para manipulação de arquivos com open()
#include <sys/wait.h> // Incluído para manipulação de processos com wait()
#include <errno.h>    // Incluído para tratamento de erros do sistema

typedef enum
{
    prioridade0 = 1,    // Prioridade mais alta
    prioridade1 = 2,   // Prioridade alta
    prioridade2 = 4,  // Prioridade média
    prioridade3 = 8  // Prioridade mais baixa
} fatiaTempoFilaDePrioridade;

// Estrutura que define uma instrução a ser executada em um processo.
typedef struct Instrucao
{
    char comando;              // Comando que define a ação a ser executada (e.g., 'R', 'A').
    int index;                 // Índice ou posição associado ao comando.
    int valor;                 // Valor adicional necessário para executar o comando.
    char arquivo[MAX_CMD_LEN]; // Nome do arquivo associado à instrução, se necessário.
    int sucesso;               // Flag que indica se a instrução foi executada com sucesso (1 para sucesso, 0 para falha).
} Instrucao;

#endif // INSTRUCAO_H

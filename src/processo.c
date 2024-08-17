/*
Estado Bloqueado = 0
Estado Pronto = 1
Estado Execução = 2
*/
#include "../headers/processo.h"

// Função para inicializar um novo processo simulado
// Recebe o conjunto de instruções, quantidade de instruções, PID do processo pai, ID do processo e o algoritmo de escalonamento
// Retorna um ponteiro para o novo processo simulado.
ProcessoSimulado *inicializaProcesso(char *conjuntoInstrucoes, int quantidadeInstrucao, int PID_Pai, int ID, int algoritmoEscalonamento)
{
    ProcessoSimulado *processo;

    // Aloca memória para um novo processo simulado
    processo = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    processo->PC = 0;                   // Inicializa o contador de programa
    processo->EstadosProcesso = Pronto; // Define o estado inicial do processo como "Pronto"

    // Define a prioridade do processo dependendo do algoritmo de escalonamento
    if (algoritmoEscalonamento == 0)
    {
        processo->prioridade = 0;
        processo->quantum = prioridade0;
    }
    else
    {
        processo->prioridade = -1;   // Se não for usado o escalonamento por prioridade
        processo->quantum = QUANTUM; // Define o quantum de tempo
    }

    processo->memoria = NULL;            // Inicialmente, a memória do processo é NULL
    processo->ID_Processo = ID;          // Atribui o ID ao processo
    processo->ID_Processo_Pai = PID_Pai; // Atribui o ID do processo pai
    processo->tempoInicio.valor = 0;     // Inicializa o tempo de início
    processo->tempoCPU.valor = 0;        // Inicializa o tempo de CPU
    processo->quantidadeInteiros = 0;    // Inicializa a quantidade de inteiros

    processo->quantidadeInstrucao = quantidadeInstrucao; // Define a quantidade de instruções
    processo->conjuntoInstrucoes = conjuntoInstrucoes;   // Atribui o conjunto de instruções

    return processo; // Retorna o processo simulado recém-criado
}

// Função para criar um novo processo a partir de um processo pai
// Recebe o processo pai e o ID do novo processo
// Retorna um ponteiro para o novo processo simulado.
ProcessoSimulado *criarNovoProcessoAPartirdoPai(ProcessoSimulado *processoPai, int ID_processo)
{
    // Aloca memória para o novo processo simulado
    ProcessoSimulado *novoProcesso = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    if (!novoProcesso)
    {
        // Caso ocorra um erro de alocação, o programa poderia ser encerrado aqui
        // fprintf(stderr, "Erro ao alocar memória para novo processo.\n");
        return NULL;
    }

    novoProcesso->ID_Processo = ID_processo;                  // Define o ID do novo processo
    novoProcesso->ID_Processo_Pai = processoPai->ID_Processo; // Define o ID do processo pai
    novoProcesso->PC = processoPai->PC + 1;                   // O novo processo continua após a instrução F do processo pai
    novoProcesso->EstadosProcesso = Pronto;                   // Define o estado inicial do novo processo como "Pronto"
    novoProcesso->prioridade = processoPai->prioridade;       // Herda a prioridade do processo pai

    // Aloca memória para o vetor de inteiros do novo processo e copia os dados do processo pai
    novoProcesso->memoria = (int *)malloc(sizeof(int) * processoPai->quantidadeInteiros);
    if (!novoProcesso->memoria)
    {
        // fprintf(stderr, "Erro ao alocar memória para vetor de valores inteiros do novo processo.\n");
        free(novoProcesso);
    }
    memcpy(novoProcesso->memoria, processoPai->memoria, sizeof(int) * processoPai->quantidadeInteiros);
    novoProcesso->quantidadeInteiros = processoPai->quantidadeInteiros;

    // Duplica o conjunto de instruções do processo pai para o novo processo
    novoProcesso->conjuntoInstrucoes = strdup(processoPai->conjuntoInstrucoes);
    if (!novoProcesso->conjuntoInstrucoes)
    {
        // fprintf(stderr, "Erro ao alocar memória para conjunto de instruções do novo processo.\n");
        free(novoProcesso->memoria);
        free(novoProcesso);
    }
    novoProcesso->quantidadeInstrucao = processoPai->quantidadeInstrucao;

    // Define o tempo de início e inicializa o tempo de CPU
    novoProcesso->tempoInicio = processoPai->tempoCPU;
    novoProcesso->tempoCPU.valor = 0;

    return novoProcesso; // Retorna o novo processo simulado
}

// Função para imprimir os detalhes de um processo simulado
// Exibe informações como ID, estado, prioridade, tempo de CPU, etc.
void imprimirProcesso(ProcessoSimulado *processo)
{
    char *estado;
    switch (processo->EstadosProcesso)
    {
    case Bloqueado:
        estado = "Bloqueado";
        break;
    case Pronto:
        estado = "Pronto";
        break;
    case Execucao:
        estado = "Execução";
        break;
    default:
        estado = "Desconhecido";
        break;
    }

    printf("=============================================\n");
    printf("|              PROCESSO                     |\n");
    printf("=============================================\n");
    printf("| %-20s | %-18d |\n", "ID", processo->ID_Processo);                         // Exibe o ID do processo
    printf("| %-20s | %-18d |\n", "ID do processo pai", processo->ID_Processo_Pai);     // Exibe o ID do processo pai
    printf("| %-20s | %-18d |\n", "PC", processo->PC);                                  // Exibe o contador de programa
    printf("| %-20s | %-18s |\n", "Estado", estado);                                    // Exibe o estado do processo
    if (processo->prioridade != -1)                                                     // Verifica se o escalonamento por prioridade foi usado
    {
        printf("| %-20s | %-18s |\n", "Prioridade", getInfoPrioridade(processo->prioridade));              // Exibe a prioridade do processo
    }
    printf("| %-21s | %-18d |\n", "Tempo de início", processo->tempoInicio.valor);      // Exibe o tempo de início
    printf("| %-20s | %-18d |\n", "Tempo de CPU", processo->tempoCPU.valor);            // Exibe o tempo de CPU
    printf("| %-22s | %-18d |\n", "Qtd de instruções", processo->quantidadeInstrucao);  // Exibe a quantidade de instruções
    printf("| %-22s | %-18s |\n", "Conj. de instruções", processo->conjuntoInstrucoes); // Exibe o conjunto de instruções
    printf("| %-20s | %-18d |\n", "Qtd de inteiros", processo->quantidadeInteiros);     // Exibe a quantidade de inteiros
    printf("| %-21s |", "Memória");

    // Exibe a memória associada ao processo
    for (int i = 0; i < processo->quantidadeInteiros; i++)
    {
        if (i > 0)
            printf(" ");
        printf(" %d", processo->memoria[i]);
    }

    printf("\n=============================================\n");
}

char* getInfoPrioridade(int prioridade)
{
    char* prioridadeString;
    switch (prioridade)
    {
    case 0:
        prioridadeString = "Muito Alta";
        break;
    case 1:
        prioridadeString = "Alta";
        break;
    case 2:
        prioridadeString = "Média";
        break;
    case 3:
        prioridadeString = "Baixa";
        break;
    default:
        break;
    }

    return prioridadeString;
}

#include "../headers/tabela_de_processos.h"

// Verifica se a tabela de processos está vazia
int isTabelaProcessosVazia(tabelaProcessos *tabela)
{
    return tabela->ultimoProcessoIndex == 0;
}

// Verifica se a tabela de processos está cheia
int isTabelaProcessosCheia(tabelaProcessos *tabela)
{
    return tabela->ultimoProcessoIndex >= MAX_PROCESSOS;
}

// Retorna o tamanho atual da tabela de processos
int getTamanhoTabelaProcessos(tabelaProcessos *tabela)
{
    return tabela->ultimoProcessoIndex;
}

// Inicializa a tabela de processos, configurando os índices inicial e final
void inicializarTabelaProcessos(tabelaProcessos *tabela)
{
    tabela->primeiroProcessoIndex = 0;
    tabela->ultimoProcessoIndex = tabela->primeiroProcessoIndex;
}

// Insere um novo processo na tabela, se esta não estiver cheia
void inserirTabelaProcessos(ProcessoSimulado *processo, tabelaProcessos *tabela)
{
    if (!isTabelaProcessosCheia(tabela))
    {
        tabela->listaProcessos[tabela->ultimoProcessoIndex] = processo;
        tabela->ultimoProcessoIndex++;
    }
    else
    {
        // Se a tabela estiver cheia, uma mensagem de erro pode ser exibida
    }
}

// Remove um processo da tabela pelo seu ID, liberando a memória associada
void retirarTabelaProcessos(tabelaProcessos *tabela, int ID_Processo)
{
    int foundIndex = -1;

    // Procura o processo com o ID especificado na tabela
    for (int i = tabela->primeiroProcessoIndex; i <= tabela->ultimoProcessoIndex; i++)
    {
        if (tabela->listaProcessos[i]->ID_Processo == ID_Processo)
        {
            foundIndex = i;
            break;
        }
    }

    // Se o processo não for encontrado, a função retorna
    if (foundIndex == -1)
    {
        return;
    }

    // Libera a memória alocada para o processo removido
    free(tabela->listaProcessos[foundIndex]->memoria);
    free(tabela->listaProcessos[foundIndex]);

    // Desloca os processos seguintes para preencher o espaço vazio
    for (int i = foundIndex; i < tabela->ultimoProcessoIndex; i++)
    {
        tabela->listaProcessos[i] = tabela->listaProcessos[i + 1];
    }

    // Limpa a referência ao último processo na tabela e ajusta o índice
    tabela->listaProcessos[tabela->ultimoProcessoIndex] = NULL;
    tabela->ultimoProcessoIndex--;

    // Ajusta o índice do primeiro processo se a tabela estiver vazia
    if (tabela->primeiroProcessoIndex > tabela->ultimoProcessoIndex)
    {
        tabela->primeiroProcessoIndex = tabela->ultimoProcessoIndex = -1;
    }
}

// Retorna uma lista de índices de processos que estão em um determinado estado
int *getIndicesEstadoTabelaProcessos(tabelaProcessos *tabela, Estados estado, int *tamanhoLista)
{
    int tamanhoTabela = getTamanhoTabelaProcessos(tabela);
    int *lista_indices = (int *)malloc(sizeof(int) * tamanhoTabela);
    int indice_lista = 0;
    for (int x = 0; x < tamanhoTabela; x++)
    {
        if (tabela->listaProcessos[x]->EstadosProcesso == estado)
        {
            lista_indices[indice_lista] = x;
            indice_lista++;
        }
    }
    *tamanhoLista = indice_lista;
    return lista_indices;
}

// Retorna o processo associado ao ID fornecido, se existir
ProcessoSimulado *getProcesso(tabelaProcessos *tabela, int ID_Processo)
{
    for (int i = tabela->primeiroProcessoIndex; i <= tabela->ultimoProcessoIndex; i++)
    {
        if (tabela->listaProcessos[i]->ID_Processo == ID_Processo)
        {
            return tabela->listaProcessos[i];
        }
    }
    return NULL;
}

// Função auxiliar para converter o estado do processo em uma string
const char *estadoToString(Estados estado)
{
    switch (estado)
    {
    case Bloqueado:
        return "Bloqueado";
    case Pronto:
        return "Pronto";
    case Execucao:
        return "Execução";
    default:
        return "Desconhecido";
    }
}

// Imprime as informações de um processo, considerando o algoritmo de escalonamento
void imprimeProcesso(ProcessoSimulado *processo, int algoritmoEscalonamento)
{
    if (processo != NULL)
    {
        if (algoritmoEscalonamento == 0)
        {
            printf("| %-11d | %-14d | %-10d | %-12s | %-10s |\n",
                   processo->ID_Processo,
                   processo->ID_Processo_Pai,
                   processo->PC,
                   estadoToString(processo->EstadosProcesso),
                   getInfoPrioridade(processo->prioridade));
        }
        else if (algoritmoEscalonamento == 1)
        {
            printf("| %-11d | %-14d | %-10d | %-12s |\n",
                   processo->ID_Processo,
                   processo->ID_Processo_Pai,
                   processo->PC,
                   estadoToString(processo->EstadosProcesso));
        }
    }
}

// Imprime a tabela de processos, formatada de acordo com o algoritmo de escalonamento
void imprimeTabelaProcessos(tabelaProcessos *tabela, int algoritmoEscalonamento)
{

    if (algoritmoEscalonamento == 0)
    {
        printf("========================================================================\n");
        printf("|                            Tabela De Processos                       |\n");
        printf("========================================================================\n");
        printf("+-------------+----------------+------------+-------------+------------+\n");
        printf("| ID Processo | ID Processo Pai| PC         | Estado      | Prioridade |\n");
        printf("+-------------+----------------+------------+-------------+------------+\n");

        for (int i = tabela->primeiroProcessoIndex; i <= tabela->ultimoProcessoIndex; i++)
        {
            if (tabela->listaProcessos[i] != NULL)
            {
                imprimeProcesso(tabela->listaProcessos[i], algoritmoEscalonamento);
            }
        }
        printf("+-------------+----------------+------------+-------------+------------+\n");
    }
    else if (algoritmoEscalonamento == 1)
    {
        printf("===========================================================\n");
        printf("|                  Tabela De Processos                    |\n");
        printf("===========================================================\n");
        printf("+-------------+----------------+------------+-------------+\n");
        printf("| ID Processo | ID Processo Pai| PC         | Estado      |\n");
        printf("+-------------+----------------+------------+-------------+\n");

        for (int i = tabela->primeiroProcessoIndex; i <= tabela->ultimoProcessoIndex; i++)
        {
            if (tabela->listaProcessos[i] != NULL)
            {
                imprimeProcesso(tabela->listaProcessos[i], algoritmoEscalonamento);
            }
        }
        printf("+-------------+----------------+------------+-------------+\n");
    }
}

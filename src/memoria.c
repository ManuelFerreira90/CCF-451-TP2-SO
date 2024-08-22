#include "../headers/memoria.h"

void iniciarMapaDeBits(MapaDeBits *mapaDeBits)
{
    for (int i = 0; i < TAM_MEMORIA; i++)
    {
        mapaDeBits->bitmap[i] = 0;
    }

    mapaDeBits->tamanho = TAM_MEMORIA;
}

void iniciarProcessosNaMemoriaLista(ProcessosNaMemoriaLista *processosNaMemoriaLista)
{
    processosNaMemoriaLista->primeiro = NULL;
    processosNaMemoriaLista->ultimo = NULL;
    processosNaMemoriaLista->tamanho = 0;
}

void printMapaDeBits(MapaDeBits *mapa)
{
    printf("┌─────────────────────────────────────────────┐\n");
    printf("│                 Mapa de Bits                │\n");
    printf("├─────────────────────────────────────────────┤\n");
    printf("│ Indice │ Valor                              │\n");
    printf("├─────────────────────────────────────────────┤\n");

    for (int i = 0; i < 10; i++)
    {
        printf("│ %6d │ %-32d │\n", i, mapa->bitmap[i]);
    }

    printf("└─────────────────────────────────────────────┘\n");
}

void alocarMemoriaFirstFit(Memoria *memoria, MapaDeBits *mapa, ProcessosNaMemoriaLista *lista, int tamanho, ProcessoSimulado *proceso, tabelaProcessos *tabela)
{
    int inicio;
    if (localizarBlocoLivre(mapa, tamanho, &inicio))
    {
        // Alocando memória para o processo
        atualizarMapaDeBits(mapa, inicio, tamanho, 1);
        printMapaDeBits(mapa);

        // Adicionando o processo à lista de processos na memória
        adicionarProcesso(lista, proceso->ID_Processo);

        proceso->regBase = inicio;
        proceso->regLimite = inicio + tamanho;

        printf("Processo %d alocado na memória\n", proceso->ID_Processo);
        printf("Registrador Base: %d\n", proceso->regBase);
        printf("Registrador Limite: %d\n", proceso->regLimite);
        return;
    }
    else
    {
        // Não há espaço suficiente, movendo um processo para o disco
        desalocarMemoriaFirstFit(memoria, lista, mapa, tamanho, tabela);

        // Tentando novamente a alocação após liberar espaço
        alocarMemoriaFirstFit(memoria, mapa, lista, tamanho, proceso->ID_Processo, tabela);
        return;
    }
}

void desalocarMemoriaFirstFit(Memoria *memoria, ProcessosNaMemoriaLista *lista, MapaDeBits *mapa, int tamanho, tabelaProcessos *tabela)
{
    ProcessosNaMemoria *atual = lista->primeiro;
    ProcessosNaMemoria *proximo = NULL;

    while (atual != NULL)
    {
        proximo = atual->proximo;
        if (tamanho <= atual->quantidadeVariaveis)
        {
            ProcessoSimulado *processo = getProcesso(tabela, atual->id);

            // Move o processo para o disco
            swapParaDisco(memoria, mapa, processo);

            // Desaloca memória para o processo
            atualizarMapaDeBits(mapa, processo->regBase, atual->quantidadeVariaveis, 0);

            // Remove o processo da lista de processos na memória
            removerProcesso(lista, atual->id);
            return;
        }
        atual = proximo;
    }
}

void atualizarMapaDeBits(MapaDeBits *mapa, int inicio, int tamanho, int valor)
{
    // Atualiza o mapa de bits para marcar a memória como ocupada (1) ou livre (0)
    for (int i = inicio; i < inicio + tamanho; i++)
    {
        mapa->bitmap[i] = valor;
    }
}

int localizarBlocoLivre(MapaDeBits *mapa, int tamanho, int *inicio)
{
    int consecutivos = 0;
    for (int i = 0; i < mapa->tamanho; i++)
    {
        //printf("Consecutivos: %d\n", consecutivos);
        if (mapa->bitmap[i] == 0)
        {
            consecutivos++;
            if (consecutivos == tamanho)
            {
                *inicio = i - tamanho + 1;
                printf("Espaço livre encontrado na posição %d\n", *inicio);
                return 1; // Encontrou espaço livre
            }
        }
        else
        {
            consecutivos = 0;
        }
    }
    printf("Não há espaço suficiente na memória\n");
    return 0; // Não encontrou espaço livre suficiente
}

int isProcessoNaMemoria(ProcessosNaMemoriaLista *lista, int id)
{
    ProcessosNaMemoria *atual = lista->primeiro;

    while (atual != NULL)
    {
        if (atual->id == id)
        {
            return 1;
        }
        atual = atual->proximo;
    }

    return 0;
}

void printMemoriaPreenchida(Memoria *memoria, MapaDeBits *mapa)
{
    printf("┌─────────────────────────────────────────────┐\n");
    printf("│             Memoria Preenchida              │\n");
    printf("├─────────────────────────────────────────────┤\n");
    printf("│ Indice │ Valor                              │\n");
    printf("├─────────────────────────────────────────────┤\n");

    for (int i = 0; i < mapa->tamanho; i++)
    {
        if (mapa->bitmap[i] == 1)  // Verifica se o bit está preenchido
        {
            printf("│ %6d │ %-32d │\n", i, memoria->memoriaPrincipal[i]);
        }
    }

    printf("└─────────────────────────────────────────────┘\n");
}

void swapParaDisco(Memoria *memoria, MapaDeBits *mapaDeBits, ProcessoSimulado *processo)
{

    FILE *disco = fopen(DISCO, "a");

    if (disco == NULL)
    {
        perror("Erro ao abrir arquivo de disco");
        return;
    }

    // Grava as variáveis do processo no disco, que estão localizadas na memória principal
    for (int i = processo->regBase; i < processo->regLimite; i++)
    {
        fprintf(disco, "%d ", memoria->memoriaPrincipal[i]);
    }

    // Finaliza a linha para o processo
    fprintf(disco, "\n");

    fclose(disco);

    // Atualizar mapa de bits
    atualizarMapaDeBits(&mapaDeBits, processo->regBase, processo->quantidadeInteiros, 0);
}

void recuperarDoDisco(Memoria *memoria, MapaDeBits *mapaDeBits, ProcessoSimulado *processo)
{
    FILE *disco = fopen("./disco/disco.txt", "r");

    if (disco == NULL)
    {
        perror("Erro ao abrir arquivo de disco");
        return;
    }

    int valor;
    int regBase = processo->regBase;
    int linhaAtual = 0;
    char buffer[1024];

    while (linhaAtual < processo->linhaDoDisco && fgets(buffer, sizeof(buffer), disco) != NULL)
    {
        linhaAtual++;
    }

    if (linhaAtual == processo->linhaDoDisco)
    {
        // Lê os valores da linha correspondente e armazena na memória principal
        for (int i = regBase; i < processo->regLimite; i++)
        {
            if (fscanf(disco, "%d", &valor) == 1)
            {
                memoria->memoriaPrincipal[i] = valor;
            }
        }
    }
    else
    {
        printf("Linha %d não encontrada no Disco\n", processo->linhaDoDisco);
    }

    fclose(disco);
}

void adicionarProcesso(ProcessosNaMemoriaLista *lista, int id)
{
    ProcessosNaMemoria *novoProcesso = (ProcessosNaMemoria *)malloc(sizeof(ProcessosNaMemoria));
    novoProcesso->id = id;
    novoProcesso->proximo = NULL;

    if (lista->tamanho == 0)
    {
        lista->primeiro = novoProcesso;
        lista->ultimo = novoProcesso;
    }
    else
    {
        lista->ultimo->proximo = novoProcesso;
        lista->ultimo = novoProcesso;
    }

    lista->tamanho++;
}

void removerProcesso(ProcessosNaMemoriaLista *lista, int id)
{
    ProcessosNaMemoria *atual = lista->primeiro;
    ProcessosNaMemoria *anterior = NULL;

    while (atual != NULL && atual->id != id)
    {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL)
        return; // Processo não encontrado

    if (anterior == NULL)
    {
        lista->primeiro = atual->proximo;
    }
    else
    {
        anterior->proximo = atual->proximo;
    }

    if (atual == lista->ultimo)
    {
        lista->ultimo = anterior;
    }

    free(atual);
    lista->tamanho--;
}

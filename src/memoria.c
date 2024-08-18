#include "../headers/memoria.h"

void iniciarMemoria(Memoria *memoria)
{
    memoria->tamanho = 0;
}

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

int alocarMemoriaFirstFit(MapaDeBits *mapa, ProcessosNaMemoriaLista *lista, int tamanho, int idProcesso)
{
    int inicio;
    if (localizarBlocoLivre(mapa, tamanho, &inicio))
    {
        // Aloca memória para o processo
        atualizarMapaDeBits(mapa, inicio, tamanho, 1);

        // Adicione o processo à lista de processos na memória
        adicionarProcesso(lista, idProcesso);

        return inicio;
    }
    else
    {
        // Não há espaço suficiente, mova um processo para o disco
        // Exemplo de chamada para mover um processo para o disco
        // moverParaDisco(&processo, mapa, memoria, nomeArquivo);

        // Tente novamente a alocação após liberar espaço
        alocarMemoriaFirstFit(mapa, lista, tamanho, idProcesso);
    }
}

void desalocarMemoriaFirstFit(ProcessosNaMemoriaLista *lista, MapaDeBits *mapa, int tamanho)
{
    ProcessosNaMemoria *atual = lista->primeiro;
    ProcessosNaMemoria *proximo = NULL;

    while (atual != NULL)
    {
        proximo = atual->proximo;
        if (tamanho <= atual->quantidadeVariaveis)
        {
            ProcessoSimulado *processo = getProcessoPorId(atual->id);
            // Desaloca memória para o processo
            atualizarMapaDeBits(mapa, processo->regBase, atual->quantidadeVariaveis, 0);

            // Remove o processo da lista de processos na memória
            removerProcesso(lista, atual->id);
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
        if (mapa->bitmap[i] == 0)
        {
            consecutivos++;
            if (consecutivos == tamanho)
            {
                *inicio = i - tamanho + 1;
                return 1; // Encontrou espaço livre
            }
        }
        else
        {
            consecutivos = 0;
        }
    }
    return 0; // Não encontrou espaço livre suficiente
}

void printMemoria(Memoria *memoria)
{
    printf("┌─────────────────────────────────────────────┐\n");
    printf("│             Memoria Estrutura               │\n");
    printf("├─────────────────────────────────────────────┤\n");
    printf("│ Tamanho: %-37d │\n", memoria->tamanho);
    printf("├─────────────────────────────────────────────┤\n");
    printf("│ Indice │ Valor                              │\n");
    printf("├─────────────────────────────────────────────┤\n");

    for (int i = 0; i < memoria->tamanho; i++)
    {
        printf("│ %6d │ %-32d │\n", i, memoria->memoriaPrincipal[i]);
    }

    printf("└─────────────────────────────────────────────┘\n");
}

void swapParaDisco(Memoria *memoria, MapaDeBits *mapaDeBits, int idProcesso)
{
    ProcessoSimulado *processo = getProcessoPorId(idProcesso);

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

void recuperarDoDisco(Memoria *memoria, MapaDeBits *mapaDeBits, int idProcesso)
{
    ProcessoSimulado *processo = getProcessoPorId(idProcesso);

    FILE *disco = fopen(DISCO, "r");

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


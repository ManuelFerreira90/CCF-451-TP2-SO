#include "../headers/memoria.h"

// TODO: processo maior que a memória

void iniciarMapaDeBits(MapaDeBits *mapaDeBits)
{
    for (int i = 0; i < TAM_MEMORIA; i++)
    {
        mapaDeBits->bitmap[i] = 0;
    }
}

void printMapaDeBits(MapaDeBits *mapa)
{
    printf("┌─────────────────────────────────────────────┐\n");
    printf("│                 Mapa de Bits                │\n");
    printf("├─────────────────────────────────────────────┤\n");
    printf("│ Indice │ Valor                              │\n");
    printf("├─────────────────────────────────────────────┤\n");

    for (int i = 0; i < TAM_MEMORIA; i++)
    {
        printf("│ %6d │ %-32d │\n", i, mapa->bitmap[i]);
    }

    printf("└─────────────────────────────────────────────┘\n");
}

void alocarMemoriaWorstFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *processo, tabelaProcessos *tabela) {
    int maiorInicio = -1;
    int maiorTamanho = 0;

    int inicio = 0;
    int blocoLivreAtual = 0;
    
    // Percorre o mapa de bits para encontrar o maior bloco livre
    for (int i = 0; i < TAM_MEMORIA; i++) {
        if (mapa->bitmap[i] == 0) {
            if (blocoLivreAtual == 0) {
                inicio = i; // Marca o início de um bloco livre
            }
            blocoLivreAtual++;
        } else {
            if (blocoLivreAtual > maiorTamanho) {
                maiorTamanho = blocoLivreAtual;
                maiorInicio = inicio;
            }
            blocoLivreAtual = 0; // Reinicia o bloco livre atual
        }
    }

    // Considera o último bloco livre no final da memória
    if (blocoLivreAtual > maiorTamanho) {
        maiorTamanho = blocoLivreAtual;
        maiorInicio = inicio;
    }

    // Verifica se encontrou um bloco grande o suficiente
    if (maiorInicio != -1 && maiorTamanho >= tamanho) {
        // Atualiza o mapa de bits e a memória
        atualizarMapa(mapa, inicio, tamanho, 1);
        printMapaDeBits(mapa);

        // Adiciona o processo à lista de processos na memória
        printf("Alocando memória para o processo %d, tamanho %d\n", processo->ID_Processo, processo->quantidadeInteiros);
        enfileirarDinamicaProcesso(lista, processo->ID_Processo, processo->quantidadeInteiros);

        // for (int i = maiorInicio; i < maiorInicio + tamanho; i++) {
        //     mapa->bitmap[i] = 1;
        //     memoria->memoriaPrincipal[i] = processo->ID_Processo; // Associa o bloco de memória ao ID do processo
        // }
        
        printf("Processo %d alocado na memória de %d a %d usando Worst Fit.\n", processo->ID_Processo, maiorInicio, maiorInicio + tamanho - 1);

        printf("\nFila depois de adicionar\n");
        imprimirFilaDinamica(lista);
        printf("\n\n");

        processo->regBase = maiorInicio;
        processo->regLimite = maiorInicio + tamanho;

    } else {
        printf("Memória insuficiente para alocar o processo %d usando Worst Fit.\n Desalocando processo...\n", processo->ID_Processo);

        int sucesso = desalocarMemoriaWorstFit(memoria, lista, mapa, tamanho, tabela);
        
        if(sucesso == -1)
        {
            printf("Não foi possível desalocar memória\n");
            return;
        }

        printf("\nFila depois de desalocar\n");
        imprimirFilaDinamica(lista);
        printf("\n\n");

        printf("Tentando alocar memória novamente\n");
        alocarMemoriaFirstFit(memoria, mapa, lista, tamanho, processo, tabela);
        return;
    }
}

int desalocarMemoriaWorstFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int idProcesso, tabelaProcessos *tabela) {
    // Node *atual = lista->frente;

    // while (atual != NULL) {
    //     if (atual->dado == idProcesso) {
    //         // Encontra o processo na memória
    //         int inicio = atual->dado;
    //         int tamanho = atual->quantidadeVariaveis;

    //         // Atualiza o mapa de bits e a memória
    //         for (int i = inicio; i < inicio + tamanho; i++) {
    //             mapa->bitmap[i] = 0;
    //             memoria->memoriaPrincipal[i] = 0;
    //         }

    //         // Remove o processo da lista
    //         // removerProcesso(lista, idProcesso);

    //         printf("Processo %d desalocado da memória.\n", idProcesso);
    //         return 1; // Indica que o desalocamento foi bem-sucedido
    //     }
    //     atual = atual->proximo;
    // }

    printf("Retirando primeiro proceso que encontrar\n");
    int id = desenfileirarDinamica(lista);

    printf("fila depois de remover 1 \n");
    imprimirFilaDinamica(lista);

    printf("\nID do processo retirado %d\n", id);
    if(id == -1)
    {
        printf("Não foi possível desalocar memória\n");
        return id;
    }

    printf("Desalocando memória para o processo %d\n", id);
    ProcessoSimulado *processo = getProcesso(tabela, id);

    imprimirFilaDinamica(lista);
    swapParaDisco(memoria, mapa, processo);

    // printf("Erro: Processo %d não encontrado na memória para desalocar.\n", idProcesso);
    // return 0; // Indica falha no desalocamento
    return id;
}

void alocarMemoriaFirstFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *proceso, tabelaProcessos *tabela)
{
    int inicio;
    printf("Alocando memória para o processo %d, tamanho %d\n", proceso->ID_Processo, proceso->quantidadeInteiros);
    if (localizarBlocoLivre(mapa, tamanho, &inicio))
    {
        // Alocando memória para o processo
        atualizarMapa(mapa, inicio, tamanho, 1);
        printMapaDeBits(mapa);

        // Adicionando o processo à lista de processos na memória
        printf("Alocando memória para o processo %d, tamanho %d\n", proceso->ID_Processo, proceso->quantidadeInteiros);
        enfileirarDinamicaProcesso(lista, proceso->ID_Processo, proceso->quantidadeInteiros);

        printf("\nFila depois de adicionar\n");
        imprimirFilaDinamica(lista);
        printf("\n\n");

        proceso->regBase = inicio;
        proceso->regLimite = inicio + tamanho;

        printf("Processo %d alocado na memória\n", proceso->ID_Processo);
        printf("Registrador Base: %d\n", proceso->regBase);
        printf("Registrador Limite: %d\n", proceso->regLimite);
        return;
    }
    else
    {
        printf("nao consegue ne moises\n");
        // Não há espaço suficiente, movendo um processo para o disco
        int sucesso = desalocarMemoriaFirstFit(memoria, lista, mapa, tamanho, tabela);
        
        if(sucesso == -1)
        {
            printf("Não foi possível desalocar memória\n");
            return;
        }

        printf("\nFila depois de desalocar\n");
        imprimirFilaDinamica(lista);
        printf("\n\n");

        printf("Tentando alocar memória novamente\n");
        alocarMemoriaFirstFit(memoria, mapa, lista, tamanho, proceso, tabela);
        return;
    }
}

int desalocarMemoriaFirstFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int tamanho, tabelaProcessos *tabela)
{
    // Node *atual = lista->frente;
    // Node *proximo = NULL;

    // int encontrou = 0;

    // while (atual != NULL)
    // {
    //     proximo = atual->proximo;
    //     if (tamanho <= atual->quantidadeVariaveis)
    //     {
    //         encontrou = 1;
    //         printf("Desalocando memória para o processo %d\n", atual->dado);
    //         ProcessoSimulado *processo = getProcesso(tabela, atual->dado);

    //         // Move o processo para o disco
    //         swapParaDisco(memoria, mapa, processo);
    //         // Desaloca memória para o processo
    //         atualizarMapaDeBits(mapa, processo->regBase, atual->quantidadeVariaveis, 0);
    //         printMapaDeBits(mapa);
    //         // Remove o processo da lista de processos na memória
    //         removerNoPorValor(lista, atual->dado);
    //         printf("fila depois de remover\n");
    //         imprimirFilaDinamica(lista);

    //         printf("Espaço liberado\n");
    //         return;
    //     }
    //     atual = proximo;
    // }

    // if (!encontrou)
    // {        imprimirFilaDinamica(lista);
        //printMapaDeBits(mapa);
    //     printf("Não foi possível encontrar um processo para desalocar\n");
    // }

    printf("Retirando primeiro proceso que encontrar\n");
    // atual = lista->frente;
    // proximo = atual->proximo;

    int id = desenfileirarDinamica(lista);

    printf("fila depois de remover 1 \n");
    //removerNoPorValor(lista, id);
    imprimirFilaDinamica(lista);

    printf("\nID do processo retirado %d\n", id);

    if(id == -1)
    {
        printf("Não foi possível desalocar memória\n");
        return id;
    }

    printf("Desalocando memória para o processo %d\n", id);
    ProcessoSimulado *processo = getProcesso(tabela, id);

    imprimirFilaDinamica(lista);
    swapParaDisco(memoria, mapa, processo);

    return id;
}

void atualizarMapa(MapaDeBits *mapa, int inicio, int tamanho, int valor)
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
    for (int i = 0; i < TAM_MEMORIA; i++)
    {
        // printf("Consecutivos: %d\n", consecutivos);
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

int isProcessoNaMemoria(FilaDinamica *lista, int id)
{
    Node *atual = lista->frente;

    while (atual != NULL)
    {
        if (atual->dado == id)
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

    for (int i = 0; i < TAM_MEMORIA; i++)
    {
        if (mapa->bitmap[i] == 1) // Verifica se o bit está preenchido
        {
            printf("│ %6d │ %-32d │\n", i, memoria->memoriaPrincipal[i]);
        }
    }

    printf("└─────────────────────────────────────────────┘\n");
}

void swapParaDisco(Memoria *memoria, MapaDeBits *mapaDeBits, ProcessoSimulado *processo)
{

    printf("Swap para disco\n");
    FILE *disco = fopen(DISCO, "r+"); // Abrir o arquivo para leitura e escrita

    if (disco == NULL)
    {
        perror("Erro ao abrir arquivo de disco");
        return;
    }

    // Pular para a linha específica no arquivo correspondente ao processo
    char buffer[1024];
    int linhaAtual = 0;

    printf("linha do processo no disco %d\n", processo->linhaDoDisco);

    // Encontra a linha correspondente ao processo no arquivo
    while (fgets(buffer, sizeof(buffer), disco) != NULL)
    {
        if (linhaAtual == processo->linhaDoDisco)
        {
            // Apaga a linha atual e reposiciona o ponteiro do arquivo para a posição inicial da linha
            printf("\n AA Linha do Disco que o process foi movido %d AA \n", linhaAtual);
            fseek(disco, -strlen(buffer), SEEK_CUR);
            break;
        }
        linhaAtual++;
    }

    // Verifica se a linha foi encontrada
    if (linhaAtual != processo->linhaDoDisco)
    {
        printf("Erro: Linha do processo não encontrada no disco.\n");
        fclose(disco);
        return;
    }

    // Grava as variáveis do processo no disco, que estão localizadas na memória principal
    for (int i = processo->regBase; i < processo->regLimite; i++)
    {
        printf("%d ", memoria->memoriaPrincipal[i]);
        fprintf(disco, "%d ", memoria->memoriaPrincipal[i]);
    }

    // Finaliza a linha para o processo
    fprintf(disco, "\n");

    fclose(disco);
    printf("\n");
    // Atualizar mapa de bits
    atualizarMapa(mapaDeBits, processo->regBase, processo->quantidadeInteiros, 0);
    printf("Processo %d movido para o disco\n", processo->ID_Processo);
}

void recuperarDoDisco(Memoria *memoria, MapaDeBits *mapaDeBits, ProcessoSimulado *processo)
{
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
    printf("Recuperando linha %d do Disco\n", processo->linhaDoDisco);
    if (linhaAtual == processo->linhaDoDisco)
    {
        // Lê os valores da linha correspondente e armazena na memória principal
        for (int i = regBase; i < processo->regLimite; i++)
        {
            if (fscanf(disco, "%d", &valor) == 1)
            {
                printf("\nvalor recuperado: %d\n", valor);
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

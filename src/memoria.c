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



void alocarMemoriaFirstFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *proceso, tabelaProcessos *tabela, Desempenho * desempenho)
{
    int inicio;
    if (localizarBlocoLivre(mapa, tamanho, &inicio))
    {
        // Alocando memória para o processo
        atualizarMapa(mapa, inicio, tamanho, 1);

        // Adicionando o processo à lista de processos na memória
        printf("Alocando memória para o processo %d, tamanho %d\n", proceso->ID_Processo, proceso->quantidadeInteiros);
        enfileirarDinamicaProcesso(lista, proceso->ID_Processo, proceso->quantidadeInteiros);
        printf("\n\n");

        proceso->regBase = inicio;
        proceso->regLimite = inicio + tamanho;

        printf("Registrador Base: %d\n", proceso->regBase);
        printf("Registrador Limite: %d\n", proceso->regLimite);
        return;
    }
    else
    {
        // Não há espaço suficiente, movendo um processo para o disco
        int sucesso = desalocarMemoriaFirstFit(memoria, lista, mapa, tamanho, tabela, desempenho);
        
        if(sucesso == -1)
        {
            printf("Não foi possível desalocar memória\n");
            return;
        }

        alocarMemoriaFirstFit(memoria, mapa, lista, tamanho, proceso, tabela,desempenho);
        return;
    }
}

int desalocarMemoriaFirstFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int tamanho, tabelaProcessos *tabela ,Desempenho * desempenho)
{
    int id = desenfileirarDinamica(lista);
    if(id == -1)
    {
        return id;
    }

    printf("Desalocando memória para o processo %d\n", id);
    ProcessoSimulado *processo = getProcesso(tabela, id);
    swapParaDisco(memoria, mapa, processo, desempenho);

    return id;
}

void alocarMemoriaNextFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *processo, tabelaProcessos *tabela, int *ultimaPosicaoAlocacao, Desempenho * desempenho)
{
    int inicio = -1;
    // Tentar encontrar um bloco livre começando da última posição de alocação
    if (localizarBlocoLivreNextFit(mapa, tamanho, &inicio, *ultimaPosicaoAlocacao))
    {
        // Alocando memória para o processo
        atualizarMapa(mapa, inicio, tamanho, 1);

        // Adicionando o processo à lista de processos na memória
        enfileirarDinamicaProcesso(lista, processo->ID_Processo, processo->quantidadeInteiros);

        processo->regBase = inicio;
        processo->regLimite = inicio + tamanho;
        *ultimaPosicaoAlocacao = processo->regLimite; // Atualizar a última posição de alocação

        printf("Alocando memória para o processo %d, tamanho %d\n", processo->ID_Processo, processo->quantidadeInteiros);
        printf("\n\n");
        printf("Registrador Base: %d\n", processo->regBase);
        printf("Registrador Limite: %d\n", processo->regLimite);
        return;
    }
    else
    {
        int sucesso = desalocarMemoriaNextFit(memoria, lista, mapa, tamanho, tabela, ultimaPosicaoAlocacao, desempenho);

        if (sucesso == -1)
        {
            return;
        }

        alocarMemoriaNextFit(memoria, mapa, lista, tamanho, processo, tabela, ultimaPosicaoAlocacao, desempenho);
        return;
    }
}
int desalocarMemoriaNextFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int tamanho, tabelaProcessos *tabela, int *ultimaPosicaoAlocacao, Desempenho * desempenho)
{
    // Desenfileira o primeiro processo da lista
    int id = desenfileirarDinamica(lista);

    if (id == -1)
    {
        return id;
    }

    printf("Desalocando memória para o processo %d\n", id);
    ProcessoSimulado *processo = getProcesso(tabela, id);

    if (processo == NULL)
    {
        printf("Processo não encontrado na tabela de processos.\n");
        return -1;
    }

    // Atualiza o mapa de bits, marcando o bloco como livre
    atualizarMapa(mapa, processo->regBase, processo->quantidadeInteiros, 0);

    // Se o bloco desalocado estava no final da memória, atualiza a última posição de alocação
    if (processo->regLimite == *ultimaPosicaoAlocacao)
    {
        *ultimaPosicaoAlocacao = processo->regBase;
    }

    // Transfere o processo para o disco, se necessário
    swapParaDisco(memoria, mapa, processo, desempenho);

    return id;
}

int localizarBlocoLivreNextFit(MapaDeBits *mapa, int tamanho, int *inicio, int ultimaPosicaoAlocacao){
    int encontrado = 0;
    int j = ultimaPosicaoAlocacao;
    int consecutivos = 0;

    for (int i = 0; i < TAM_MEMORIA * 2; i++)
    {
        if(j == TAM_MEMORIA)
        {
            consecutivos = 0;
            j = 0;
        }
        if(mapa->bitmap[j] == 0)
        {
            consecutivos++;
            if(consecutivos == tamanho)
            {
                encontrado = 1;
                *inicio = j - tamanho + 1;
                return 1;
            }
        }
        else
        {
            consecutivos = 0;
        }
        j++;
    }

    return encontrado;
}

void alocarMemoriaWorstFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *processo, tabelaProcessos *tabela, Desempenho * desempenho) {
    int inicio;

    if(localizarBlocoLivreWorstFit(mapa, tamanho, &inicio)){
        atualizarMapa(mapa, inicio, tamanho, 1);

        // Adiciona o processo à lista de processos na memória
        printf("Alocando memória para o processo %d, tamanho %d\n", processo->ID_Processo, processo->quantidadeInteiros);
        enfileirarDinamicaProcesso(lista, processo->ID_Processo, processo->quantidadeInteiros);
        processo->regBase = inicio;
        processo->regLimite = inicio + tamanho;

    } else {
        int sucesso = desalocarMemoriaWorstFit(memoria, lista, mapa, tamanho, tabela, desempenho);
        
        if(sucesso == -1)
        {
            return;
        }

        alocarMemoriaWorstFit(memoria, mapa, lista, tamanho, processo, tabela, desempenho);
        return;
    }
}

int desalocarMemoriaWorstFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int idProcesso, tabelaProcessos *tabela) {
    if (isFilaDinamicaVazia(lista)) {
        return -1;
    }

    Node *atual = lista->frente;
    Node *maiorProcessoNode = NULL;
    int maiorTamanho = 0;

    // Encontra o processo com maior tamanho
    while (atual != NULL) {
        ProcessoSimulado *processo = getProcesso(tabela, atual->dado);
        if (processo != NULL && processo->quantidadeInteiros > maiorTamanho) {
            maiorTamanho = processo->quantidadeInteiros;
            maiorProcessoNode = atual;
        }
        atual = atual->proximo;
    }

    if (maiorProcessoNode == NULL) {
        return -1; // Indica falha no desalocamento
    }

    int id = maiorProcessoNode->dado;
    printf("Desalocando memória para o processo maior: ID %d, tamanho %d.\n", id, maiorTamanho);

    ProcessoSimulado *processoParaDesalocar = getProcesso(tabela, id);
    if (processoParaDesalocar == NULL) {
        return -1;
    }

    atualizarMapa(mapa, processoParaDesalocar->regBase, maiorTamanho, 0);
    removerNoPorValor(lista, id);
    swapParaDisco(memoria, mapa, processoParaDesalocar, desempenho);
    return id; // Indica que o desalocamento foi bem-sucedido
}

int localizarBlocoLivreBestFit(MapaDeBits *mapa, int tamanho, int *inicio)
{
    int menorTamanho = 0;
    int consecutivos = 0;


    for (int i = 0; i < TAM_MEMORIA; i++)
    {
        if (mapa->bitmap[i] == 0 && i != TAM_MEMORIA - 1)
        {
            consecutivos++;
        }
        else 
        {
            if (consecutivos >= tamanho)
            {
                if (menorTamanho == 0 || consecutivos < menorTamanho)
                {

                    menorTamanho = consecutivos;
                    *inicio = i - consecutivos;
                }
            }
            consecutivos = 0;
        }
    }

    if (menorTamanho >= tamanho)
    {
        return 1; // Encontrou espaço livre
    }
    
    return 0; // Não encontrou espaço livre suficiente
}

void alocarMemoriaFirstFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *proceso, tabelaProcessos *tabela)
{
    int inicio;
    if(localizarBlocoLivreBestFit(mapa, tamanho, &inicio)){
        atualizarMapa(mapa, inicio, tamanho, 1);

        // Adiciona o processo à lista de processos na memória
        printf("Alocando memória para o processo %d, tamanho %d\n", processo->ID_Processo, processo->quantidadeInteiros);
        enfileirarDinamicaProcesso(lista, processo->ID_Processo, processo->quantidadeInteiros);

        processo->regBase = inicio;
        processo->regLimite = inicio + tamanho;

    } else {
        int sucesso = desalocarMemoriaBestFit(memoria, lista, mapa, tamanho, tabela, desempenho);
        
        if(sucesso == -1)
        {
            return;
        }
        alocarMemoriaBestFit(memoria, mapa, lista, tamanho, processo, tabela, desempenho);
        return;
    }
}

int desalocarMemoriaBestFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int idProcesso, tabelaProcessos *tabela, Desempenho * desempenho) {
    if (isFilaDinamicaVazia(lista)) {
        return -1;
    }

    // if (!encontrou)
    // {        imprimirFilaDinamica(lista);
        //printMapaDeBits(mapa);
    //     printf("Não foi possível encontrar um processo para desalocar\n");
    // }

    printf("Retirando primeiro proceso que encontrar\n");
    // atual = lista->frente;
    // proximo = atual->proximo;

    int id = desenfileirarDinamica(lista);

    if (menorProcessoNode == NULL) {
        return -1; // Indica falha no desalocamento
    }

    int id = menorProcessoNode->dado;
    printf("Desalocando memória para o processo menor: ID %d, tamanho %d.\n", id, menorTamanho);

    ProcessoSimulado *processoParaDesalocar = getProcesso(tabela, id);
    if (processoParaDesalocar == NULL) {
        return -1;
    }

    atualizarMapa(mapa, processoParaDesalocar->regBase, menorTamanho, 0);
    removerNoPorValor(lista, id);
    swapParaDisco(memoria, mapa, processoParaDesalocar, desempenho);
    return id; // Indica que o desalocamento foi bem-sucedido
}

void atualizarMapa(MapaDeBits *mapa, int inicio, int tamanho, int valor)
{
    // Atualiza o mapa de bits para marcar a memória como ocupada (1) ou livre (0)
    for (int i = inicio; i < inicio + tamanho; i++)
    {
        mapa->bitmap[i] = valor;
    }
}

int localizarBlocoLivreWorstFit(MapaDeBits *mapa, int tamanho, int *inicio) {
    int maiorTamanho = 0; // Inicializa com o menor valor possível
    int maiorInicio = -1;
    int consecutivos = 0;

    for (int i = 0; i < TAM_MEMORIA; i++) {
        if (mapa->bitmap[i] == 0) {
            consecutivos++;
            if (i == TAM_MEMORIA - 1 || mapa->bitmap[i + 1] == 1) { // Fim de um bloco livre
                if (consecutivos >= tamanho && consecutivos > maiorTamanho) {
                    maiorTamanho = consecutivos;
                    maiorInicio = i - consecutivos + 1;
                }
                consecutivos = 0;
            }
        } else {
            consecutivos = 0;
        }
    }

    if (maiorInicio != -1) {
        *inicio = maiorInicio;
        printf("Espaço livre encontrado na posição %d com tamanho %d\n",*inicio, maiorTamanho);
        return 1; // Encontrou o maior bloco livre
    }

    return 0; // Não encontrou espaço livre suficiente
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
    desempenho->numeroVezesDesalocacao++;
    FILE *disco = fopen(DISCO, "r+"); // Abrir o arquivo para leitura e escrita

    if (disco == NULL)
    {
        return;
    }

    // Pular para a linha específica no arquivo correspondente ao processo
    char buffer[1024];
    int linhaAtual = 0;

    // Encontra a linha correspondente ao processo no arquivo
    while (fgets(buffer, sizeof(buffer), disco) != NULL)
    {
        if (linhaAtual == processo->linhaDoDisco)
        {
            // Apaga a linha atual e reposiciona o ponteiro do arquivo para a posição inicial da linha
            fseek(disco, -strlen(buffer), SEEK_CUR);
            break;
        }
        linhaAtual++;
    }

    // Verifica se a linha foi encontrada
    if (linhaAtual != processo->linhaDoDisco)
    {
        fclose(disco);
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
    printf("Recuperando dados do processo %d do Disco\n", processo->ID_Processo);
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

    fclose(disco);
}

void removerProcesso(FilaDinamica *fila, int idProcesso) {
    if (isFilaDinamicaVazia(fila)) {
        printf("A fila está vazia, não é possível remover o processo.\n");
        return;
    }

    Node *atual = fila->frente;
    Node *anterior = NULL;

    // Percorre a fila para encontrar o processo a ser removido
    while (atual != NULL) {
        if (atual->dado == idProcesso) {
            if (anterior == NULL) {
                // Removendo o primeiro nó
                fila->frente = atual->proximo;
            } else {
                // Removendo um nó no meio ou final
                anterior->proximo = atual->proximo;
            }

            if (atual->proximo == NULL) {
                // Se for o último nó, atualiza o ponteiro 'tras'
                fila->tras = anterior;
            }

            free(atual);  // Libera a memória do nó removido
            fila->tamanho--;  // Decrementa o tamanho da fila
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    printf("Processo %d não encontrado na fila.\n", idProcesso);
}

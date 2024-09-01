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
    printf("Alocando memória para o processo %d, tamanho %d\n", proceso->ID_Processo, proceso->quantidadeInteiros);
    if (localizarBlocoLivre(mapa, tamanho, &inicio, desempenho))
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
        int sucesso = desalocarMemoriaFirstFit(memoria, lista, mapa, tamanho, tabela, desempenho);
        
        if(sucesso == -1)
        {
            printf("Não foi possível desalocar memória\n");
            return;
        }

        printf("\nFila depois de desalocar\n");
        imprimirFilaDinamica(lista);
        printf("\n\n");

        printf("Tentando alocar memória novamente\n");
        alocarMemoriaFirstFit(memoria, mapa, lista, tamanho, proceso, tabela,desempenho);
        return;
    }
}

int desalocarMemoriaFirstFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int tamanho, tabelaProcessos *tabela ,Desempenho * desempenho)
{
    

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
    swapParaDisco(memoria, mapa, processo, desempenho);

    return id;
}

void alocarMemoriaNextFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *processo, tabelaProcessos *tabela, int *ultimaPosicaoAlocacao, Desempenho * desempenho)
{
    int inicio = -1;
    printf("Alocando memória para o processo %d, tamanho %d\n", processo->ID_Processo, processo->quantidadeInteiros);

    // Tentar encontrar um bloco livre começando da última posição de alocação
    if (localizarBlocoLivreNextFit(mapa, tamanho, &inicio, *ultimaPosicaoAlocacao, desempenho))
    {
        // Alocando memória para o processo
        atualizarMapa(mapa, inicio, tamanho, 1);
        printMapaDeBits(mapa);

        // Adicionando o processo à lista de processos na memória
        enfileirarDinamicaProcesso(lista, processo->ID_Processo, processo->quantidadeInteiros);

        printf("\nFila depois de adicionar\n");
        imprimirFilaDinamica(lista);
        printf("\n\n");

        processo->regBase = inicio;
        processo->regLimite = inicio + tamanho;
        *ultimaPosicaoAlocacao = processo->regLimite; // Atualizar a última posição de alocação

        printf("Processo %d alocado na memória\n", processo->ID_Processo);
        printf("Registrador Base: %d\n", processo->regBase);
        printf("Registrador Limite: %d\n", processo->regLimite);
        return;
    }
    else
    {
        printf("Não há espaço suficiente. Tentando desalocar um processo...\n");
        int sucesso = desalocarMemoriaNextFit(memoria, lista, mapa, tamanho, tabela, ultimaPosicaoAlocacao, desempenho);

        if (sucesso == -1)
        {
            printf("Não foi possível desalocar memória\n");
            return;
        }

        printf("\nFila depois de desalocar\n");
        imprimirFilaDinamica(lista);
        printf("\n\n");

        printf("Tentando alocar memória novamente\n");
        alocarMemoriaNextFit(memoria, mapa, lista, tamanho, processo, tabela, ultimaPosicaoAlocacao, desempenho);
        return;
    }
}
int desalocarMemoriaNextFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int tamanho, tabelaProcessos *tabela, int *ultimaPosicaoAlocacao, Desempenho * desempenho)
{
    printf("Retirando o primeiro processo que encontrar (Next Fit)\n");

    // Desenfileira o primeiro processo da lista
    int id = desenfileirarDinamica(lista);

    printf("Fila depois de remover um processo:\n");
    imprimirFilaDinamica(lista);

    printf("\nID do processo retirado: %d\n", id);

    if (id == -1)
    {
        printf("Não foi possível desalocar memória\n");
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
    printMapaDeBits(mapa);

    // Se o bloco desalocado estava no final da memória, atualiza a última posição de alocação
    if (processo->regLimite == *ultimaPosicaoAlocacao)
    {
        *ultimaPosicaoAlocacao = processo->regBase;
    }

    // Transfere o processo para o disco, se necessário
    swapParaDisco(memoria, mapa, processo, desempenho);

    return id;
}

int localizarBlocoLivreNextFit(MapaDeBits *mapa, int tamanho, int *inicio, int ultimaPosicaoAlocacao, Desempenho * desempenho)
{
    int encontrado = 0;
    int j = ultimaPosicaoAlocacao;
    int consecutivos = 0;
    for(int i = 0; i < TAM_MEMORIA; i++){
        if(mapa->bitmap[i] == 0){
            consecutivos++;
           
        } else {
            if(consecutivos > 0){
                desempenho->numeroMedioFragmentosExternos += 1;
            }
            consecutivos = 0;
        }
    }
    consecutivos = 0;

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
                desempenho->tempoMedioAlocacao += 1;
                encontrado = 1;
                *inicio = j - tamanho + 1;
                printf("Espaço livre encontrado na posição %d\n", *inicio);
                return 1;
            }
        }
        else
        {
            desempenho->tempoMedioAlocacao += 1;
            consecutivos = 0;
        }
        j++;
    }

    return encontrado;
}

void alocarMemoriaWorstFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *processo, tabelaProcessos *tabela, Desempenho * desempenho) {
    int inicio;

    if(localizarBlocoLivre(mapa, tamanho, &inicio, desempenho)){
        atualizarMapa(mapa, inicio, tamanho, 1);
        printMapaDeBits(mapa);

        // Adiciona o processo à lista de processos na memória
        printf("Alocando memória para o processo %d, tamanho %d\n", processo->ID_Processo, processo->quantidadeInteiros);
        enfileirarDinamicaProcesso(lista, processo->ID_Processo, processo->quantidadeInteiros);
        
        printf("Processo %d alocado na memória de %d a %d usando Worst Fit.\n", processo->ID_Processo, inicio, inicio + tamanho - 1);

        printf("\nFila depois de adicionar\n");
        imprimirFilaDinamica(lista);
        printf("\n\n");

        processo->regBase = inicio;
        processo->regLimite = inicio + tamanho;

    } else {
        printf("Memória insuficiente para alocar o processo %d usando Worst Fit.\n Desalocando processo...\n", processo->ID_Processo);

        int sucesso = desalocarMemoriaWorstFit(memoria, lista, mapa, tamanho, tabela, desempenho);
        
        if(sucesso == -1)
        {
            printf("Não foi possível desalocar memória\n");
            return;
        }

        printf("\nFila depois de desalocar\n");
        imprimirFilaDinamica(lista);
        printf("\n\n");

        printf("Tentando alocar memória novamente\n");
        alocarMemoriaWorstFit(memoria, mapa, lista, tamanho, processo, tabela, desempenho);
        return;
    }
}

int desalocarMemoriaWorstFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int idProcesso, tabelaProcessos *tabela, Desempenho * desempenho) {
    if (isFilaDinamicaVazia(lista)) {
        printf("Não foi possível desalocar memória, fila vazia.\n");
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
        printf("Erro: Nenhum processo encontrado na memória para desalocar.\n");
        return -1; // Indica falha no desalocamento
    }

    int id = maiorProcessoNode->dado;
    printf("Desalocando memória para o processo maior: ID %d, tamanho %d.\n", id, maiorTamanho);

    ProcessoSimulado *processoParaDesalocar = getProcesso(tabela, id);
    if (processoParaDesalocar == NULL) {
        printf("Erro: Processo %d não encontrado na tabela de processos.\n", id);
        return -1;
    }

    atualizarMapa(mapa, processoParaDesalocar->regBase, maiorTamanho, 0);
    printMapaDeBits(mapa);
    removerNoPorValor(lista, id);
    swapParaDisco(memoria, mapa, processoParaDesalocar, desempenho);

    printf("Processo %d desalocado da memória.\n", id);
    return id; // Indica que o desalocamento foi bem-sucedido
}

int localizarBlocoLivreBestFit(MapaDeBits *mapa, int tamanho, int *inicio, Desempenho * desempenho)
{
    int menorTamanho = 0;
    int consecutivos = 0;

    for (int i = 0; i < TAM_MEMORIA; i++)
    {
        if (mapa->bitmap[i] == 0)
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
                    *inicio = i - consecutivos + 1;
                }
            }
            consecutivos = 0;
        }
    }

    if (menorTamanho >= tamanho)
    {
        desempenho->tempoMedioAlocacao += 1;
        printf("Espaço livre encontrado na posição %d\n", *inicio);
        return 1; // Encontrou espaço livre
    }
    
    printf("Não há espaço suficiente na memória\n");
    return 0; // Não encontrou espaço livre suficiente
}

void alocarMemoriaBestFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *processo, tabelaProcessos *tabela, Desempenho * desempenho) {
    int inicio;

    if(localizarBlocoLivreBestFit(mapa, tamanho, &inicio, desempenho)){
        atualizarMapa(mapa, inicio, tamanho, 1);
        printMapaDeBits(mapa);

        // Adiciona o processo à lista de processos na memória
        printf("Alocando memória para o processo %d, tamanho %d\n", processo->ID_Processo, processo->quantidadeInteiros);
        enfileirarDinamicaProcesso(lista, processo->ID_Processo, processo->quantidadeInteiros);
        
        printf("Processo %d alocado na memória de %d a %d usando Best Fit.\n", processo->ID_Processo, inicio, inicio + tamanho - 1);

        printf("\nFila depois de adicionar\n");
        imprimirFilaDinamica(lista);
        printf("\n\n");

        processo->regBase = inicio;
        processo->regLimite = inicio + tamanho;

    } else {
        printf("Memória insuficiente para alocar o processo %d usando Best Fit.\n Desalocando processo...\n", processo->ID_Processo);

        int sucesso = desalocarMemoriaBestFit(memoria, lista, mapa, tamanho, tabela, desempenho);
        
        if(sucesso == -1)
        {
            printf("Não foi possível desalocar memória\n");
            return;
        }

        printf("\nFila depois de desalocar\n");
        imprimirFilaDinamica(lista);
        printf("\n\n");

        printf("Tentando alocar memória novamente\n");
        alocarMemoriaBestFit(memoria, mapa, lista, tamanho, processo, tabela, desempenho);
        return;
    }
}

int desalocarMemoriaBestFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int idProcesso, tabelaProcessos *tabela, Desempenho * desempenho) {
    if (isFilaDinamicaVazia(lista)) {
        printf("Não foi possível desalocar memória, fila vazia.\n");
        return -1;
    }

    Node *atual = lista->frente;
    Node *menorProcessoNode = NULL;
    int menorTamanho;    // Encontra o processo com menor tamanho

    if(atual != NULL){
        ProcessoSimulado *processo = getProcesso(tabela, atual->dado);
        menorTamanho = processo->quantidadeInteiros;
        menorProcessoNode = atual;
    }

    while (atual != NULL) {
        ProcessoSimulado *processo = getProcesso(tabela, atual->dado);
        if (processo != NULL && processo->quantidadeInteiros < menorTamanho) {
            menorTamanho = processo->quantidadeInteiros;
            menorProcessoNode = atual;
        }
        atual = atual->proximo;
    }

    if (menorProcessoNode == NULL) {
        printf("Erro: Nenhum processo encontrado na memória para desalocar.\n");
        return -1; // Indica falha no desalocamento
    }

    int id = menorProcessoNode->dado;
    printf("Desalocando memória para o processo maior: ID %d, tamanho %d.\n", id, menorTamanho);

    ProcessoSimulado *processoParaDesalocar = getProcesso(tabela, id);
    if (processoParaDesalocar == NULL) {
        printf("Erro: Processo %d não encontrado na tabela de processos.\n", id);
        return -1;
    }

    atualizarMapa(mapa, processoParaDesalocar->regBase, menorTamanho, 0);
    printMapaDeBits(mapa);
    removerNoPorValor(lista, id);
    swapParaDisco(memoria, mapa, processoParaDesalocar, desempenho);

    printf("Processo %d desalocado da memória.\n", id);
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

int localizarBlocoLivre(MapaDeBits *mapa, int tamanho, int *inicio, Desempenho * desempenho)
{

    int consecutivos = 0;
    for(int i = 0; i < TAM_MEMORIA; i++){
        if(mapa->bitmap[i] == 0){
            consecutivos++;
           
        } else {
            if(consecutivos > 0){
                desempenho->numeroMedioFragmentosExternos += 1;
            }
            consecutivos = 0;
        }
    }
    consecutivos = 0;


    for (int i = 0; i < TAM_MEMORIA; i++)
    {
        // printf("Consecutivos: %d\n", consecutivos);
        if (mapa->bitmap[i] == 0)
        {
            consecutivos++;
            if (consecutivos == tamanho)
            {
                desempenho->tempoMedioAlocacao += 1;
                *inicio = i - tamanho + 1;
                printf("Espaço livre encontrado na posição %d\n", *inicio);
                return 1; // Encontrou espaço livre
            } else{
                
            }
        }
        else
        {
            if(consecutivos > 0){
                desempenho->tempoMedioAlocacao += 1;

            }
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

void swapParaDisco(Memoria *memoria, MapaDeBits *mapaDeBits, ProcessoSimulado *processo, Desempenho *desempenho)
{
    desempenho->numeroVezesDesalocacao++;
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

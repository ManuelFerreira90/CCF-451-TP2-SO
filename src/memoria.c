#include "../headers/memoria.h"

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
    if (localizarBlocoLivre(mapa, tamanho, &inicio, desempenho))
    {
        atualizarMapa(mapa, inicio, tamanho, 1);

        enfileirarDinamicaProcesso(lista, proceso->ID_Processo, proceso->quantidadeInteiros);
        printf("Processo %d alocado na memória de %d a %d usando Firs Fit.\n", proceso->ID_Processo, inicio, inicio + tamanho - 1);


        proceso->regBase = inicio;
        proceso->regLimite = inicio + tamanho;

        printf("Registrador Base: %d\n", proceso->regBase);
        printf("Registrador Limite: %d\n", proceso->regLimite);
        return;
    }
    else
    {
        int sucesso = desalocarMemoriaFirstFit(memoria, lista, mapa, tamanho, tabela, desempenho);
        
        if(sucesso == -1)
        {
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

    if (localizarBlocoLivreNextFit(mapa, tamanho, &inicio, *ultimaPosicaoAlocacao, desempenho))
    {
        atualizarMapa(mapa, inicio, tamanho, 1);

        enfileirarDinamicaProcesso(lista, processo->ID_Processo, processo->quantidadeInteiros);

        printf("Processo %d alocado na memória de %d a %d usando Next Fit.\n", processo->ID_Processo, inicio, inicio + tamanho - 1);

        processo->regBase = inicio;
        processo->regLimite = inicio + tamanho;
        *ultimaPosicaoAlocacao = processo->regLimite; // Atualizar a última posição de alocação

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

        printf("\n\n");

        alocarMemoriaNextFit(memoria, mapa, lista, tamanho, processo, tabela, ultimaPosicaoAlocacao, desempenho);
        return;
    }
}
int desalocarMemoriaNextFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int tamanho, tabelaProcessos *tabela, int *ultimaPosicaoAlocacao, Desempenho * desempenho)
{

    int id = desenfileirarDinamica(lista);



    if (id == -1)
    {
        return id;
    }

    printf("Desalocando memória para o processo %d\n", id);
    ProcessoSimulado *processo = getProcesso(tabela, id);

    if (processo == NULL)
    {
        return -1;
    }

    atualizarMapa(mapa, processo->regBase, processo->quantidadeInteiros, 0);

    if (processo->regLimite == *ultimaPosicaoAlocacao)
    {
        *ultimaPosicaoAlocacao = processo->regBase;
    }

    swapParaDisco(memoria, mapa, processo, desempenho);

    return id;
}

int localizarBlocoLivreNextFit(MapaDeBits *mapa, int tamanho, int *inicio, int ultimaPosicaoAlocacao, Desempenho * desempenho)
{
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
                printf("Espaço livre encontrado na posição %d\n", *inicio);
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

    if(localizarBlocoLivreWorstFit(mapa, tamanho, &inicio, desempenho)){
        atualizarMapa(mapa, inicio, tamanho, 1);

        enfileirarDinamicaProcesso(lista, processo->ID_Processo, processo->quantidadeInteiros);
        
        printf("Processo %d alocado na memória de %d a %d usando Worst Fit.\n", processo->ID_Processo, inicio, inicio + tamanho - 1);


        processo->regBase = inicio;
        processo->regLimite = inicio + tamanho;
        printf("Registrador Base: %d\n", processo->regBase);
        printf("Registrador Limite: %d\n", processo->regLimite);
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

int desalocarMemoriaWorstFit(Memoria *memoria, FilaDinamica *lista, MapaDeBits *mapa, int idProcesso, tabelaProcessos *tabela, Desempenho * desempenho) {
    if (isFilaDinamicaVazia(lista)) {
        return -1;
    }

    Node *atual = lista->frente;
    Node *maiorProcessoNode = NULL;
    int maiorTamanho = 0;

    while (atual != NULL) {
        ProcessoSimulado *processo = getProcesso(tabela, atual->dado);
        if (processo != NULL && processo->quantidadeInteiros > maiorTamanho) {
            maiorTamanho = processo->quantidadeInteiros;
            maiorProcessoNode = atual;
        }
        atual = atual->proximo;
    }

    if (maiorProcessoNode == NULL) {
        return -1;
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
    
    return id;
}

int localizarBlocoLivreBestFit(MapaDeBits *mapa, int tamanho, int *inicio, Desempenho * desempenho)
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
        return 1;
    }
    
    return 0; 
}

void alocarMemoriaBestFit(Memoria *memoria, MapaDeBits *mapa, FilaDinamica *lista, int tamanho, ProcessoSimulado *processo, tabelaProcessos *tabela, Desempenho * desempenho) {
    int inicio;
    if(localizarBlocoLivreBestFit(mapa, tamanho, &inicio, desempenho)){
        atualizarMapa(mapa, inicio, tamanho, 1);

        enfileirarDinamicaProcesso(lista, processo->ID_Processo, processo->quantidadeInteiros);
        
        printf("Processo %d alocado na memória de %d a %d usando Best Fit.\n", processo->ID_Processo, inicio, inicio + tamanho - 1);


        processo->regBase = inicio;
        processo->regLimite = inicio + tamanho;

        printf("Registrador Base: %d\n", processo->regBase);
        printf("Registrador Limite: %d\n", processo->regLimite);

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

    Node *atual = lista->frente;
    Node *menorProcessoNode = NULL;
    int menorTamanho; 

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
        return -1;
    }

    int id = menorProcessoNode->dado;

    ProcessoSimulado *processoParaDesalocar = getProcesso(tabela, id);
    if (processoParaDesalocar == NULL) {
        printf("Erro: Processo %d não encontrado na tabela de processos.\n", id);
        return -1;
    }

    atualizarMapa(mapa, processoParaDesalocar->regBase, menorTamanho, 0);
    removerNoPorValor(lista, id);
    swapParaDisco(memoria, mapa, processoParaDesalocar, desempenho);

    printf("Processo %d desalocado da memória.\n", id);
    return id; 
}

void atualizarMapa(MapaDeBits *mapa, int inicio, int tamanho, int valor)
{
    for (int i = inicio; i < inicio + tamanho; i++)
    {
        mapa->bitmap[i] = valor;
    }
}

int localizarBlocoLivreWorstFit(MapaDeBits *mapa, int tamanho, int *inicio, Desempenho *desempenho) {
    int maiorTamanho = 0; 
    int maiorInicio = -1;
    int consecutivos = 0;

    for (int i = 0; i < TAM_MEMORIA; i++) {
        if (mapa->bitmap[i] == 0) {
            consecutivos++;
            if (i == TAM_MEMORIA - 1 || mapa->bitmap[i + 1] == 1) { 
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
        return 1; 
    }

    return 0;
}


int localizarBlocoLivre(MapaDeBits *mapa, int tamanho, int *inicio, Desempenho * desempenho)
{

    int consecutivos = 0;

    for (int i = 0; i < TAM_MEMORIA; i++)
    {
        if (mapa->bitmap[i] == 0)
        {
            consecutivos++;
            if (consecutivos == tamanho)
            {
                *inicio = i - tamanho + 1;
                printf("Espaço livre encontrado na posição %d\n", *inicio);
                return 1;
            } else{
                
            }
        }
        else
        {
            consecutivos = 0;
        }
    }

    
    return 0;
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
        if (mapa->bitmap[i] == 1) 
        {
            printf("│ %6d │ %-32d │\n", i, memoria->memoriaPrincipal[i]);
        }
    }

    printf("└─────────────────────────────────────────────┘\n");
}

void swapParaDisco(Memoria *memoria, MapaDeBits *mapaDeBits, ProcessoSimulado *processo, Desempenho *desempenho)
{
    desempenho->numeroVezesDesalocacao++;
    FILE *disco = fopen(DISCO, "r+"); 

    if (disco == NULL)
    {
        perror("Erro ao abrir arquivo de disco");
        return;
    }

    char buffer[1024];
    int linhaAtual = 0;


    while (fgets(buffer, sizeof(buffer), disco) != NULL)
    {
        if (linhaAtual == processo->linhaDoDisco)
        {
            fseek(disco, -strlen(buffer), SEEK_CUR);
            break;
        }
        linhaAtual++;
    }

    if (linhaAtual != processo->linhaDoDisco)
    {
        fclose(disco);
        return;
    }

    for (int i = processo->regBase; i < processo->regLimite; i++)
    {
        fprintf(disco, "%d ", memoria->memoriaPrincipal[i]);
    }

    fprintf(disco, "\n");

    fclose(disco);
    printf("\n");
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

    if (linhaAtual == processo->linhaDoDisco)
    {
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

#include "../headers/memoria.h"

void alocarMemoriaProcesso(Memoria *memoria, int numVariaveis)
{
    if (memoria->tamanho + numVariaveis > TAM_MEMORIA)
    {
        printf("\nErro: Memória insuficiente para alocar %d variáveis.\n", numVariaveis);
        return;
    }

    memoria->tamanho += numVariaveis;
}

void printMemoria(Memoria *memoria)
{
    printf("┌─────────────────────────────────────────────┐\n");
    printf("│             Memoria Estrutura               │\n");
    printf("├─────────────────────────────────────────────┤\n");
    printf("│ Tamanho: %-37d │\n", memoria->tamanho);
    printf("├─────────────────────────────────────────────┤\n");
    printf("│ Indice │ Valor                             │\n");
    printf("├─────────────────────────────────────────────┤\n");

    for (int i = 0; i < memoria->tamanho; i++)
    {
        printf("│ %6d │ %-32d │\n", i, memoria->memoriaPrincipal[i]);
    }

    printf("└─────────────────────────────────────────────┘\n");
}

void swapParaDisco(ProcessoSimulado *processo, Memoria *memoria) {
    FILE *disco = fopen(DISCO, "a");
    
    if (disco == NULL) {
        perror("Erro ao abrir arquivo de disco");
        return;
    }
    
    // Grava as variáveis do processo no disco, que estão localizadas na memória principal
    for (int i = processo->regBase; i < processo->regLimite; i++) {
        fprintf(disco, "%d ", memoria->memoriaPrincipal[i]);
    }
    
    // Finaliza a linha para o processo
    fprintf(disco, "\n");
    
    fclose(disco);
}

void recuperarDoDisco(ProcessoSimulado *processo, Memoria *memoria) {
    FILE *disco = fopen(DISCO, "r");
    
    if (disco == NULL) {
        perror("Erro ao abrir arquivo de disco");
        return;
    }

    int valor;
    int regBase = processo->regBase;
    int linhaAtual = 0;
    char buffer[1024];

    while (linhaAtual < processo->linhaDoDisco && fgets(buffer, sizeof(buffer), disco) != NULL) {
        linhaAtual++;
    }

    if (linhaAtual == processo->linhaDoDisco) {
        // Lê os valores da linha correspondente e armazena na memória principal
        for (int i = regBase; i < processo->regLimite; i++) {
            if (fscanf(disco, "%d", &valor) == 1) {
                memoria->memoriaPrincipal[i] = valor;
            }
        }
    } else {
        printf("Linha %d não encontrada no Disco\n", processo->linhaDoDisco);
    }

    fclose(disco);
}

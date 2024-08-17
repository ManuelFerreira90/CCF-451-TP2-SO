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
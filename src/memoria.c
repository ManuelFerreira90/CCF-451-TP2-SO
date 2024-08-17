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
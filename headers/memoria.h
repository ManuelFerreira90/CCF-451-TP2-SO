#ifndef MEMORIA_H
#define MEMORIA_H

#include "./instrucao.h"

typedef struct {
    int memoriaPrincipal[TAM_MEMORIA];
    int tamanho;
} Memoria;

void alocarMemoriaProcesso(Memoria *memoria, int numVariaveis);

#endif // MEMORIA_H


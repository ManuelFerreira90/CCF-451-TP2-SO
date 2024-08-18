#ifndef TEMPO_H
#define TEMPO_H

#include "./instrucao.h"

// Estrutura que representa um tempo no sistema.
// A estrutura pode ser expandida no futuro para incluir mais informações relacionadas ao tempo.
typedef struct Tempo
{
    int valor; // Valor que representa a unidade de tempo (por exemplo, milissegundos, ciclos, etc.).
} Tempo;

// Declarações de funções para manipulação de tempo
//---------------------------------------------------------------------------------------

// Inicializa uma estrutura Tempo com um valor padrão (normalmente zero).
void inicializarTempo(Tempo *tempo);

// Define o valor do tempo na estrutura Tempo.
// Permite atualizar o valor do tempo com base em um novo valor fornecido.
void setTempo(Tempo *tempo, int valor);

//---------------------------------------------------------------------------------------

#endif // TEMPO_H

#include "../headers/tempo.h"

// Função para inicializar a estrutura de Tempo com valor zero
void inicializarTempo(Tempo *tempo)
{
    tempo->valor = 0;
}

// Função para definir um valor específico para o Tempo
void setTempo(Tempo *tempo, int valor)
{
    tempo->valor = valor;
}

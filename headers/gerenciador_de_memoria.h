#ifndef GERENCIADOR_DE_MEMORIA_H
#define GERENCIADOR_DE_MEMORIA_H

#include "./memoria.h"

typedef struct
{
    Memoria memoria;                               // Memória compartilhada entre os processos.
    MapaDeBits mapaDeBits;                         // Mapa de bits para gerenciar a memória.
    FilaDinamica processosNaMemoriaLista;          // Lista de processos na memória.
    int controleDoDisco;  
    Desempenho desempenho;                         // Controle de disco para troca de contexto.
    int nextfit;
} GerenciadorDeMemoria;

void iniciarGerenciadorDeMemoria(GerenciadorDeMemoria *gerenciadorMemoria);
void gerenciarMemoriaParaProcesso(GerenciadorDeMemoria *gerenciadorMemoria, ProcessoSimulado *processo, tabelaProcessos *tabela);
void gerenciarTerminoProcesso(GerenciadorDeMemoria *gerenciadorMemoria, ProcessoSimulado *processo);
void incrementoControleDisco(GerenciadorDeMemoria *gerenciadorMemoria);
int getControleDisco(GerenciadorDeMemoria *gerenciadorMemoria);
void imprimirDesempenho(Desempenho desempenho);

#endif // GERENCIADOR_DE_MEMORIA_H

#include "../headers/gerenciador_de_memoria.h"

void iniciarGerenciadorDeMemoria(GerenciadorDeMemoria *gerenciadorMemoria)
{
    gerenciadorMemoria->controleDoDisco = 0;

    iniciarMapaDeBits(&gerenciadorMemoria->mapaDeBits);
    inicializarFilaDinamica(&gerenciadorMemoria->processosNaMemoriaLista);
}

int getControleDisco(GerenciadorDeMemoria *gerenciadorMemoria)
{
    return gerenciadorMemoria->controleDoDisco;
}

void gerenciarMemoriaParaProcesso(GerenciadorDeMemoria *gerenciadorMemoria, ProcessoSimulado *processo, tabelaProcessos *tabela)
{
    if (isProcessoNaMemoria(&gerenciadorMemoria->processosNaMemoriaLista, processo->ID_Processo))
    {
        return;
    }
    else
    {
        printf("Processo %d não está na memória\n", processo->ID_Processo);
        imprimirFilaDinamica(&gerenciadorMemoria->processosNaMemoriaLista);


        //alocarMemoriaFirstFit(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), &(gerenciadorMemoria->processosNaMemoriaLista), processo->quantidadeInteiros, processo, tabela);
        alocarMemoriaNextFit(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), &(gerenciadorMemoria->processosNaMemoriaLista), processo->quantidadeInteiros, processo, tabela, &(gerenciadorMemoria->controleDoDisco));
        printf("Processo %d alocado na memória\n", processo->ID_Processo);

        if(processo->isExecutado == 1)
        {
            printf("Recuperando processo %d da memória\n", processo->ID_Processo);
            recuperarDoDisco(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), processo);
        }
    }
}

void gerenciarTerminoProcesso(GerenciadorDeMemoria *gerenciadorMemoria, ProcessoSimulado *processo)
{
    swapParaDisco(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), processo);
    removerNoPorValor(&(gerenciadorMemoria->processosNaMemoriaLista), processo->ID_Processo);
    imprimirFilaDinamica(&(gerenciadorMemoria->processosNaMemoriaLista));
}

void incrementoControleDisco(GerenciadorDeMemoria *gerenciadorMemoria)
{
    gerenciadorMemoria->controleDoDisco++;
}
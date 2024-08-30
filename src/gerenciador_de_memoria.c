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
        alocarMemoriaFirstFit(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), &(gerenciadorMemoria->processosNaMemoriaLista), processo->quantidadeInteiros, processo, tabela,&gerenciadorMemoria->desempenho);
        gerenciadorMemoria->desempenho.numeroVezesAlocacao += 1;
        
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
    swapParaDisco(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), processo,&gerenciadorMemoria->desempenho);
    removerNoPorValor(&(gerenciadorMemoria->processosNaMemoriaLista), processo->ID_Processo);
    imprimirFilaDinamica(&(gerenciadorMemoria->processosNaMemoriaLista));
}

void incrementoControleDisco(GerenciadorDeMemoria *gerenciadorMemoria)
{
    gerenciadorMemoria->controleDoDisco++;
}


void imprimirDesempenho(Desempenho desempenho) {
    printf("+--------------------------------------------+\n");
    printf("| Desempenho da Simulação                    |\n");
    printf("+--------------------------+-----------------+\n");
    printf("| Métrica                  | Valor           |\n");
    printf("+--------------------------+-----------------+\n");
    printf("| Número Médio de Fragmentos Externos | %15d |\n", desempenho.numeroMedioFragmentosExternos);
    printf("| Tempo Médio de Alocação           | %15d ms |\n", desempenho.tempoMedioAlocacao);
    printf("| Número de Vezes de Desalocação    | %15d |\n", desempenho.numeroVezesDesalocacao);
    printf("| Número de Vezes de Alocação       | %15d |\n", desempenho.numeroVezesAlocacao);
    printf("+--------------------------+-----------------+\n");
}
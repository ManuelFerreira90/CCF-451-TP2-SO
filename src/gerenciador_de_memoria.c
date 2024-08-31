#include "../headers/gerenciador_de_memoria.h"

void iniciarGerenciadorDeMemoria(GerenciadorDeMemoria *gerenciadorMemoria, int tipoAlocacao)
{
    gerenciadorMemoria->controleDoDisco = 0;
    gerenciadorMemoria->nextfit = 0;
    gerenciadorMemoria->tipoAlocacao = tipoAlocacao;
    iniciarMapaDeBits(&gerenciadorMemoria->mapaDeBits);
    inicializarFilaDinamica(&gerenciadorMemoria->processosNaMemoriaLista);
    iniciarDesempenho(&gerenciadorMemoria->desempenho);
}

void iniciarDesempenho(Desempenho *desempenho)
{
    desempenho->numeroMedioFragmentosExternos = 0;
    desempenho->numeroVezesAlocacao = 0;
    desempenho->numeroVezesDesalocacao = 0;
    desempenho->tempoMedioAlocacao = 0;
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
    else if (processo->quantidadeInteiros > TAM_MEMORIA)
    {
        printf("Processo %d é maior que a memória\n", processo->ID_Processo);
        return;
    }
    else
    {

        printf("Processo %d não está na memória\n", processo->ID_Processo);
        imprimirFilaDinamica(&gerenciadorMemoria->processosNaMemoriaLista);
        switch (gerenciadorMemoria->tipoAlocacao)
        {
        case 0:
            alocarMemoriaFirstFit(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), &(gerenciadorMemoria->processosNaMemoriaLista), processo->quantidadeInteiros, processo, tabela, &gerenciadorMemoria->desempenho);
            break;
        case 1:
            alocarMemoriaBestFit(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), &(gerenciadorMemoria->processosNaMemoriaLista), processo->quantidadeInteiros, processo, tabela, &gerenciadorMemoria->desempenho);

        case 2:
            alocarMemoriaWorstFit(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), &(gerenciadorMemoria->processosNaMemoriaLista), processo->quantidadeInteiros, processo, tabela, &gerenciadorMemoria->desempenho);
            break;
        case 3:
            alocarMemoriaNextFit(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), &(gerenciadorMemoria->processosNaMemoriaLista), processo->quantidadeInteiros, processo, tabela, &gerenciadorMemoria->nextfit, &gerenciadorMemoria->desempenho);
            break;

            break;
        default:
            printf("Tipo de alocação inválido\n");
            break;
        }

        gerenciadorMemoria->desempenho.numeroVezesAlocacao += 1;

        printf("Processo %d alocado na memória\n", processo->ID_Processo);
        if (processo->isExecutado == 1)
        {
            printf("Recuperando processo %d da memória\n", processo->ID_Processo);
            recuperarDoDisco(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), processo);
        }
    }
}

void gerenciarTerminoProcesso(GerenciadorDeMemoria *gerenciadorMemoria, ProcessoSimulado *processo)
{
    swapParaDisco(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), processo, &gerenciadorMemoria->desempenho);
    removerNoPorValor(&(gerenciadorMemoria->processosNaMemoriaLista), processo->ID_Processo);
    imprimirFilaDinamica(&(gerenciadorMemoria->processosNaMemoriaLista));
    // atualizarMapa(&(gerenciadorMemoria->mapaDeBits), processo->regBase, processo->quantidadeInteiros, 0);
}

void incrementoControleDisco(GerenciadorDeMemoria *gerenciadorMemoria)
{
    gerenciadorMemoria->controleDoDisco++;
}

void imprimirDesempenho(Desempenho desempenho)
{
    printf("+---------------------------------------------------------+\n");
    printf("| Desempenho da Simulação                                 |\n");
    printf("+--------------------------+------------------------------+\n");
    printf("| Métrica                  | Valor                        |\n");
    printf("+--------------------------+------------------------------+\n");
    printf("| Número Médio de Fragmentos Externos | %17d |\n", desempenho.numeroMedioFragmentosExternos);
    printf("| Tempo Médio de Alocação           | %19d |\n", desempenho.tempoMedioAlocacao);
    printf("| Número de Vezes de Desalocação    | %19d |\n", desempenho.numeroVezesDesalocacao);
    printf("| Número de Vezes de Alocação       | %19d |\n", desempenho.numeroVezesAlocacao);
    printf("+--------------------------+------------------------------+\n");
}
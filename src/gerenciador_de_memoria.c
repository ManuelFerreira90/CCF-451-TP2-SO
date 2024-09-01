#include "../headers/gerenciador_de_memoria.h"
#include <time.h> 

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

        clock_t start, end;
        double cpu_time_used;

        start = clock(); // Início da medição do tempo

        switch (gerenciadorMemoria->tipoAlocacao)
        {
        case 0:
            alocarMemoriaFirstFit(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), &(gerenciadorMemoria->processosNaMemoriaLista), processo->quantidadeInteiros, processo, tabela, &gerenciadorMemoria->desempenho);
            break;
        case 1:
            alocarMemoriaBestFit(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), &(gerenciadorMemoria->processosNaMemoriaLista), processo->quantidadeInteiros, processo, tabela, &gerenciadorMemoria->desempenho);
            break;
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

        end = clock(); // Fim da medição do tempo
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0; // Tempo em milissegundos
        gerenciadorMemoria->desempenho.tempoMedioAlocacao += cpu_time_used;

        calcularFragmentosExternos(&(gerenciadorMemoria->mapaDeBits), &gerenciadorMemoria->desempenho);
        gerenciadorMemoria->desempenho.numeroVezesAlocacao += 1;

        printf("Processo %d alocado na memória\n", processo->ID_Processo);
        if(processo->isExecutado == 1)
        {
            printf("Recuperando processo %d da memória\n", processo->ID_Processo);
            recuperarDoDisco(&(gerenciadorMemoria->memoria), &(gerenciadorMemoria->mapaDeBits), processo);
        }
    }
}

void calcularFragmentosExternos(MapaDeBits *mapa, Desempenho *desempenho){
    int fragmentoAtual = 0;
    for (int i = 0; i < TAM_MEMORIA; i++)
    {
        if(mapa->bitmap[i] == 0){
            fragmentoAtual = 1;
        }else{
            desempenho->numeroMedioFragmentosExternos += fragmentoAtual;
            fragmentoAtual = 0;
        }
    }
    desempenho->numeroMedioFragmentosExternos += fragmentoAtual;
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

void imprimirDesempenho(Desempenho desempenho)
{
     if(desempenho.numeroVezesAlocacao > 0){
        desempenho.numeroMedioFragmentosExternos = desempenho.numeroMedioFragmentosExternos / desempenho.numeroVezesAlocacao;
        desempenho.tempoMedioAlocacao = desempenho.tempoMedioAlocacao / desempenho.numeroVezesAlocacao;
    }
    printf("+---------------------------------------------------------+\n");
    printf("| Desempenho da Simulação                                 |\n");
    printf("+--------------------------+------------------------------+\n");
    printf("| Métrica                  | Valor                        |\n");
    printf("+--------------------------+------------------------------+\n");
    printf("| Número Médio de Fragmentos Externos | %17d |\n", desempenho.numeroMedioFragmentosExternos);
    printf("| Tempo Médio de Alocação           | %16lf ms |\n", desempenho.tempoMedioAlocacao);
    printf("| Número de Vezes de Desalocação    | %19d |\n", desempenho.numeroVezesDesalocacao);
    printf("+--------------------------+------------------------------+\n");
}
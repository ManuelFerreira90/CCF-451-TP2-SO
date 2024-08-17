#include "../headers/CPU.h"

// Função para imprimir uma borda de tabela
// Esta função imprime uma linha de '=' com o comprimento definido por TABLE_WIDTH.
// É usada para melhorar a formatação visual da saída da função imprimeCPU.
void printTableBorder()
{
    for (int i = 0; i < TABLE_WIDTH; i++)
    {
        printf("=");
    }
    printf("\n");
}

// Função para imprimir o estado atual da CPU
// Exibe as informações relevantes sobre o estado de uma CPU específica,
// incluindo o estado de execução, o contador de programa, a fatia de tempo e o conteúdo da memória.
void imprimeCPU(CPU cpu, int index)
{
    printf("\n");
    printTableBorder();
    printf("| %-32s %-3d |\n", "CPU Status ID", index); // Exibe o índice da CPU
    printTableBorder();
    
    // Determina o estado da CPU (ocupada ou vazia) com base na presença de um processo em execução
    const char *estadoCPU = (cpu.processoEmExecucao != NULL) ? "Ocupada" : "Vazia";
    printf("| %-25s | %-8s |\n", "Estado CPU", estadoCPU);
    printTableBorder();

    // Exibe o ID do processo em execução, se houver
    if (cpu.processoEmExecucao != NULL)
    {
        printf("| %-25s | %8d |\n", "ID do processo", cpu.processoEmExecucao->ID_Processo);
        printTableBorder();
    }
    
    // Exibe o valor atual do contador de programa
    printf("| %-25s | %8d |\n", "Contador de programa", cpu.contadorPrograma);
    printTableBorder();
    
    // Exibe o valor da fatia de tempo (quantum) atribuída à CPU
    printf("| %-25s | %8d |\n", "Fatia de tempo", cpu.fatiaTempo.valor);
    printTableBorder();
    
    // Cabeçalho para a seção de memória da CPU
    printf("| %-36s |\n", "Memória atual na CPU");
    printTableBorder();

    // Exibe o conteúdo da memória da CPU, se disponível
    if (cpu.memoriaVect != NULL)
    {
        // Imprime os índices e os valores da memória
        printf("| %-15s | %-18s |\n", "Index", "Valor");
        printTableBorder();
        for (int i = 0; i < cpu.quantidadeInteiros; i++)
        {
            printf("|  %14d | %18d |\n", i, cpu.memoriaVect[i]);
        }
        printTableBorder();
    }
    else
    {
        // Indica que a memória está vazia (NULL)
        printf("| %-36s |\n", "NULL");
        printTableBorder();
    }
    printf("\n\n");
}

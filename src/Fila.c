#include "../headers/Fila.h"

// Função para inicializar uma fila dinâmica
// Define os ponteiros frente e tras como NULL e o tamanho da fila como 0.
void inicializarFilaDinamica(FilaDinamica *fila)
{
    fila->frente = NULL;
    fila->tras = NULL;
    fila->tamanho = 0;
}

// Verifica se a fila dinâmica está vazia
// Retorna 1 se a fila estiver vazia, caso contrário, retorna 0.
int isFilaDinamicaVazia(FilaDinamica *fila)
{
    return (fila->tamanho == 0);
}

// Adiciona um item à fila dinâmica
// Cria um novo nó, aloca memória para ele, e o adiciona ao final da fila.
void enfileirarDinamica(FilaDinamica *fila, int item)
{
    Node *novoNode = (Node *)malloc(sizeof(Node));  // Aloca memória para o novo nó
    if (novoNode == NULL)
    {
        // Caso ocorra um erro de alocação de memória, o programa é encerrado
        exit(EXIT_FAILURE);
    }
    novoNode->dado = item;      // Define o valor do novo nó
    novoNode->proximo = NULL;   // O novo nó será o último, então seu próximo é NULL

    if (isFilaDinamicaVazia(fila))  // Se a fila estiver vazia, o novo nó é também o primeiro
    {
        fila->frente = novoNode;
    }
    else  // Caso contrário, o último nó atual aponta para o novo nó
    {
        fila->tras->proximo = novoNode;
    }
    fila->tras = novoNode;  // O novo nó se torna o último na fila
    fila->tamanho++;  // Incrementa o tamanho da fila
}

// Remove e retorna o item na frente da fila dinâmica
// Se a fila estiver vazia, retorna -1. Caso contrário, remove e retorna o item da frente.
int desenfileirarDinamica(FilaDinamica *fila)
{
    if (isFilaDinamicaVazia(fila))  // Verifica se a fila está vazia
    {
        return -1;  // Retorna -1 se a fila estiver vazia
    }
    Node *tempNode = fila->frente;  // Armazena o nó da frente temporariamente
    int item = tempNode->dado;  // Captura o dado do nó da frente
    fila->frente = fila->frente->proximo;  // Avança o ponteiro frente para o próximo nó

    if (fila->frente == NULL)  // Se a fila estiver vazia após a remoção, o ponteiro tras também é NULL
    {
        fila->tras = NULL;
    }
    free(tempNode);  // Libera a memória do nó removido
    fila->tamanho--;  // Decrementa o tamanho da fila
    return item;  // Retorna o item removido
}

// Retorna o item na frente da fila dinâmica sem removê-lo
// Se a fila estiver vazia, retorna -1. Caso contrário, retorna o dado do nó da frente.
int frenteFilaDinamica(FilaDinamica *fila)
{
    if (isFilaDinamicaVazia(fila))  // Verifica se a fila está vazia
    {
        return -1;  // Retorna -1 se a fila estiver vazia
    }
    return fila->frente->dado;  // Retorna o dado do nó da frente sem removê-lo
}

// Libera toda a memória alocada pela fila dinâmica
// Remove e libera todos os nós da fila, esvaziando-a completamente.
void liberarFilaDinamica(FilaDinamica *fila)
{
    while (!isFilaDinamicaVazia(fila))  // Continua removendo enquanto a fila não estiver vazia
    {
        desenfileirarDinamica(fila);  // Remove o nó da frente
    }
}

// Imprime todos os elementos da fila dinâmica
// Exibe o tamanho da fila e todos os elementos nela presentes.
void imprimirFilaDinamica(FilaDinamica *fila)
{
    printf("Tamanho da fila: %d\n", fila->tamanho);
    if (fila->tamanho == 0)  // Se a fila estiver vazia, apenas retorna
    {
        return;
    }

    Node *tempNode = fila->frente;  // Começa pelo primeiro nó
    printf("Fila: ");
    while (tempNode != NULL)  // Percorre a fila até o último nó
    {
        printf("-> %d ", tempNode->dado);  // Imprime o valor do nó atual
        tempNode = tempNode->proximo;  // Avança para o próximo nó
    }
    printf("\n");
}

void removerNo(FilaDinamica *fila, Node *no)
{
    if (no == NULL) return;

    if (no == fila->frente)
    {
        // Caso o nó seja o primeiro da fila
        fila->frente = no->proximo;
    }
    else
    {
        Node *atual = fila->frente;
        while(atual->proximo != no)
        {
            atual = atual->proximo;
        }

        atual->proximo = no->proximo;
    }

    if (no == fila->tras)
    {
        // Caso o nó seja o último da fila
        fila->tras = NULL;
    }

    fila->tamanho--;
    free(no); // Libera a memória do nó removido
}
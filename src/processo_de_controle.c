#define BUFFER 1 // Definindo o tamanho do buffer para leitura de um caractere por vez

#include "../headers/gerenciador_de_processos.h"
#include "../headers/processoControle.h"

// TODO: @Tarik 1. Implementar função para imprimir cada processo com sua memória e tempo de CPU
// TODO: @Tarik 2. Implementar função para as filas de prioridades ou fila de round robin, conforme a escolha inicial do escalonamento

// Declaração da função para imprimir os processos no Gerenciador de Processos
void processoImpressao(GerenciadorProcessos gerenciador);

int processoControle()
{
    int fd[2], fd_filho[2]; /* Descritores de arquivo para o Pipe */
    pid_t pid;              /* Variável para armazenar o PID do processo */
    int file_fd;            /* Descritor de arquivo para o arquivo init.txt */
    char buffer[BUFFER];    /* Buffer para leitura de dados */
    ssize_t bytes_read;     /* Variável para armazenar a quantidade de bytes lidos */
    char escolha;           /* Variável para armazenar a escolha do usuário */
    char stringEntrada[1000]; /* Buffer para armazenar a entrada do usuário */
    FILE *entrada = stdin;  // Por padrão, a entrada será lida do terminal

    /* Criando o Pipe para comunicação entre processos pai e filho */
    if (pipe(fd) < 0)
    {
        perror("pipe"); // Caso haja um erro ao criar o pipe, exibe uma mensagem
        return -1;
    }

    /* Criando o processo filho */
    if ((pid = fork()) < 0)
    {
        perror("fork"); // Caso haja um erro ao criar o processo filho, exibe uma mensagem
        exit(1);
    }

    /* Processo Pai */
    if (pid > 0)
    {
        /* No processo pai, ler comandos e escrever no Pipe */
        close(fd[0]); // Fecha o descritor de leitura do Pipe no processo pai

        // Leitura do número de CPUs desejadas pelo usuário
        int numero_CPUS;

        do
        {
            printf("Digite o número de CPUs desejadas: ");
            scanf("%d", &numero_CPUS);
            if (numero_CPUS < 1)
            {
                printf("Número de CPUs inválido. Tente novamente.\n");
            }
        } while (numero_CPUS < 1);

        // Leitura do tipo de escalonamento escolhido pelo usuário
        int tipo_escalonamento;
        do
        {
            printf("Escolha o tipo de escalonamento (0: Fila de Prioridades, 1: Round Robin): ");
            scanf("%d", &tipo_escalonamento);
            if (tipo_escalonamento != 0 && tipo_escalonamento != 1)
            {
                printf("Tipo de escalonamento inválido. Tente novamente.\n");
            }
        } while (tipo_escalonamento != 0 && tipo_escalonamento != 1);

        // Enviar o número de CPUs e o tipo de escalonamento para o processo filho
        write(fd[1], &numero_CPUS, sizeof(int));
        write(fd[1], &tipo_escalonamento, sizeof(int));

        int entradaUsu;
        do
        {
            sleep(2); // Pausa para dar tempo ao usuário
            printf("Escolha o tipo de entrada (1: terminal, 2: arquivo): ");
            scanf("%d", &entradaUsu);

            if (entradaUsu == 1 || entradaUsu == 2)
            {
                if (entradaUsu == 1)
                {
                    // Lê entrada do terminal
                    lerTerminal(stringEntrada);
                }
                else if (entradaUsu == 2)
                {
                    // Lê entrada de um arquivo
                    lerArquivo(stringEntrada);
                }
                break;
            }
            else
            {
                printf("Entrada inválida. Tente novamente.\n");
            }

        } while (entradaUsu != 1 || entradaUsu != 2);

        // Enviar a string de entrada para o processo filho via Pipe
        printf("String enviada: %s\n", stringEntrada);
        write(fd[1], stringEntrada, sizeof(stringEntrada));

        close(fd[1]); // Fecha o descritor de escrita do Pipe no processo pai
        wait(NULL);   // Espera o processo filho terminar

        exit(0);
    }

    /* Processo Filho */
    else
    {
        if (pipe(fd_filho) < 0)
        {
            perror("pipe"); // Caso haja um erro ao criar o pipe no processo filho, exibe uma mensagem
            return -1;
        }

        char str_recebida[BUFFER + 1]; // Buffer para leitura da string enviada pelo pai (+1 para o terminador nulo)
        ssize_t bytes_read; // Variável para armazenar a quantidade de bytes lidos

        /* Inicializar o Gerenciador de Processos */
        int unidadeTempo = 0;
        int numero_CPUS, tipo_escalonamento;

        // Ler o número de CPUs e o tipo de escalonamento do pipe
        read(fd[0], &numero_CPUS, sizeof(int));
        read(fd[0], &tipo_escalonamento, sizeof(int));

        // Iniciar o Gerenciador de Processos com os parâmetros recebidos
        GerenciadorProcessos gerenciador;
        iniciarGerenciadorProcessos(&gerenciador, "./entry/input1.txt", 0, numero_CPUS, tipo_escalonamento);

        /* No processo filho, ler do Pipe e processar comandos */
        close(fd[1]); // Fechar o descritor de escrita do Pipe no processo filho

        // Mensagem de depuração
        wait(NULL);
        printf("Filho: Processando comandos...\n");

        while ((bytes_read = read(fd[0], str_recebida, BUFFER)) > 0)
        {
            str_recebida[bytes_read] = '\0'; // Garantir que a string seja terminada corretamente

            // Processar comandos com o Gerenciador de Processos
            switch (str_recebida[0])
            {
            case 'U':
                printf("\nUnidade de tempo: %d\n", unidadeTempo);
                if (tipo_escalonamento == 0)
                {
                    // Executar o escalonador de Fila de Prioridades
                    escalonadorFilaDePrioridades(&gerenciador);
                }
                else if (tipo_escalonamento == 1)
                {
                    // Executar o escalonador Round Robin
                    escalonadorRoundRobin(&gerenciador);
                }
                // Incrementar o tempo de CPU após o escalonamento
                incrementarTempoCPU(&gerenciador);

                printf("Fim de uma unidade de tempo.\n");
                unidadeTempo++;
                break;
            case 'I':
                printf("\nImprimindo estado atual do sistema.\n");
                // Chamar função do Gerenciador de Processos para imprimir o estado das CPUs
                for (int i = 0; i < gerenciador.quantidadeCPUs; i++)
                {
                    imprimeCPU(gerenciador.cpus[i], i);
                }
                break;
            case 'M':
            {
                printf("\nImprimindo tempo médio de resposta e finalizando.\n");
                // Chamar função para imprimir o tempo médio de resposta dos processos
                processoImpressao(gerenciador);
                break;
            }
            case ' ':
                break;
            default:
                printf("Comando desconhecido: %c\n", str_recebida[0]);
                break;
            }

            if (str_recebida[0] == 'M')
            {
                break; // Finaliza o loop quando o comando 'M' for recebido
            }
        }

        if (bytes_read < 0)
        {
            perror("read"); // Caso ocorra um erro na leitura do pipe
        }
        close(fd[0]); // Fecha o descritor de leitura do Pipe
        exit(0);
    }

    return 0;
}


void processoImpressao(GerenciadorProcessos gerenciador)
{
    pid_t print_pid = fork();
    if (print_pid == 0)
    {
        // Processo filho para impressão
        // Imprime o tempo médio dos processos no gerenciador
        imprimirTempoMedioProcessos(gerenciador);

        // Imprime a tabela de processos com base no algoritmo de escalonamento
        imprimeTabelaProcessos(&gerenciador.TabelaProcessos, gerenciador.algoritmoEscalonamento);

        // Imprime detalhes de todos os processos no gerenciador
        imprimirTodosProcessos(&gerenciador);

        // Imprime as filas de processos no gerenciador
        imprimirFilas(&gerenciador);

        exit(0); // Finaliza o processo de impressão
    }
    else if (print_pid > 0)
    {
        // Processo pai espera o filho terminar a impressão
        wait(NULL);
    }
    else
    {
        // Caso ocorra um erro ao tentar criar o processo filho para impressão
        perror("fork");
    }
}

void lerArquivo(char *retorno)
{
    FILE *arquivo;
    char str[MAX_CMD_LEN];

    // Abre o arquivo "init.txt" na pasta "entry" para leitura
    arquivo = fopen("./entry/init.txt", "r");

    if (arquivo == NULL)
    {
        // Caso ocorra um erro ao abrir o arquivo, exibe uma mensagem de erro
        printf("Erro ao abrir arquivo\n");
        return;
    }
    else
    {
        // Lê o conteúdo do arquivo palavra por palavra e concatena na string de retorno
        while (fscanf(arquivo, "%s", str) != EOF)
        {
            strcat(retorno, str);
            strcat(retorno, " ");
        }

        fclose(arquivo); // Fecha o arquivo após a leitura
    }
}

void lerTerminal(char *retorno)
{
    char comando;
    int i = 0;

    printf("Entre com os comandos:\n");

    do
    {
        // Lê um comando do terminal
        scanf(" %c", &comando);

        // Converte letras minúsculas para maiúsculas
        if (comando >= 97 && comando <= 120)
        {
            comando = comando - 32;
        }

        // Armazena o comando na string de retorno
        retorno[i] = comando;

        // Adiciona um espaço após o comando
        strcat(retorno, " ");
        i++;
    } while (comando != 'M'); // Continua lendo até o comando 'M' ser inserido

    // Remove caracteres de nova linha da string de retorno
    remove_char(retorno, '\n');
}

void remove_char(char *str, char c)
{
    char *pr = str, *pw = str;

    // Percorre a string, removendo o caractere especificado
    while (*pr)
    {
        *pw = *pr++;
        pw += (*pw != c);
    }

    *pw = '\0'; // Finaliza a string
}

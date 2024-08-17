#include "../headers/gerenciador_de_processos.h"

// TODO : veriricar getpid()
// TODO : Se o processo for bloqueado, antes de expirar seu quantum alocado, a sua prioridade é aumentada.
// TODO : Fatia de tempo para cada nível de prioridade.

// Adiciona um processo à fila de prontos, baseado na sua prioridade
void adicionarProcessoProntoFilaDePrioridade(GerenciadorProcessos *gerenciador, int processoIndex)
{
    // Obtém a prioridade do processo da tabela de processos
    ProcessoSimulado *processo = getProcesso(&gerenciador->TabelaProcessos, processoIndex);

    // Enfileira o processo na fila de prontos correspondente à sua prioridade
    enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[processo->prioridade]), processoIndex);

    // Atualiza o estado do processo para "Pronto"
    processo->EstadosProcesso = Pronto;
}

// Adiciona um processo à fila de bloqueados, baseado na sua prioridade
void adicionarProcessoBloqueadoFilaDePrioridade(GerenciadorProcessos *gerenciador, int processoIndex)
{
    // Obtém a prioridade do processo da tabela de processos
    ProcessoSimulado *processo = getProcesso(&gerenciador->TabelaProcessos, processoIndex);

    // TODO : avaliar aumento de prioridade
    if (processo->prioridade > 0)
    {
        // Aumenta a prioridade do processo em 1
        processo->prioridade--;
        processo->quantum = getFatiaTempoPrioridade(processo->prioridade);
    }

    // Enfileira o processo na fila de bloqueados correspondente à sua prioridade
    enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasBloqueados[processo->prioridade]), processoIndex);

    // Atualiza o estado do processo para "Bloqueado"
    processo->EstadosProcesso = Bloqueado;
}

// Move o processo em execução para a fila de bloqueados
void comandoB(GerenciadorProcessos *gerenciador, int indexCPU, int valor)
{
    // Obtém o índice do processo em execução na CPU especificada
    int processoIndex = gerenciador->processosEmExecucao[indexCPU];

    // Verifica se há um processo em execução
    if (processoIndex != -1)
    {
        // Verifica qual algoritmo de escalonamento está em uso
        if (gerenciador->algoritmoEscalonamento == 0)
        {
            // Adiciona o processo à fila de bloqueados com base na prioridade
            adicionarProcessoBloqueadoFilaDePrioridade(gerenciador, processoIndex);
        }
        else if (gerenciador->algoritmoEscalonamento == 1)
        {
            // Adiciona o processo à fila de bloqueados do Round Robin
            enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaBloqueado), processoIndex);
        }

        // Marca a CPU como não tendo nenhum processo em execução
        gerenciador->processosEmExecucao[indexCPU] = -1;
    }

    gerenciador->cpus[indexCPU].processoEmExecucao->tempoBloqueado.valor = valor;
    gerenciador->cpus[indexCPU].processoEmExecucao->PC++;
    // Reinicia a CPU especificada para estar pronta para executar um novo processo
    iniciarCPU(&gerenciador->cpus[indexCPU]);
}

// Comando 'D': Declara uma nova variável no processo em execução
void comandoD(CPU *cpu, int index)
{
    // Define o valor da variável no vetor de memória da CPU
    cpu->memoriaVect[index] = 0;

    // Incrementa o contador de programa da CPU
    cpu->contadorPrograma++;
}

// Comando 'N': Cria um novo vetor de memória para o processo em execução
void comandoN(CPU *cpu, int valor)
{
    // Se o processo em execução já tiver um vetor de memória, libera-o
    if (cpu->quantidadeInteiros > 0)
    {
        free(cpu->processoEmExecucao->memoria);
        cpu->processoEmExecucao->memoria = NULL;
    }

    // Aloca um novo vetor de memória com o tamanho especificado
    cpu->processoEmExecucao->memoria = (int *)malloc(valor * sizeof(int));
    cpu->memoriaVect = cpu->processoEmExecucao->memoria;
    cpu->quantidadeInteiros = valor;
    cpu->processoEmExecucao->quantidadeInteiros = valor;

    // Incrementa o contador de programa da CPU
    cpu->contadorPrograma++;
}

// Comando 'V': Define o valor de uma variável no vetor de memória do processo em execução
void comandoV(CPU *cpu, int index, int valor)
{
    // Define o valor da variável no índice especificado
    cpu->memoriaVect[index] = valor;

    // Incrementa o contador de programa da CPU
    cpu->contadorPrograma++;
}

// Comando 'A': Adiciona um valor a uma variável no vetor de memória do processo em execução
void comandoA(CPU *cpu, int index, int valor)
{
    // Adiciona o valor à variável no índice especificado
    cpu->memoriaVect[index] += valor;

    // Incrementa o contador de programa da CPU
    cpu->contadorPrograma++;
}

// Comando 'S': Subtrai um valor de uma variável no vetor de memória do processo em execução
void comandoS(CPU *cpu, int index, int valor)
{
    // Subtrai o valor da variável no índice especificado
    cpu->memoriaVect[index] -= valor;

    // Incrementa o contador de programa da CPU
    cpu->contadorPrograma++;
}

// Comando 'F': Cria um novo processo simulado a partir do processo em execução
void comandoF(GerenciadorProcessos *gerenciador, int indexCPU, int valor)
{
    // Cria um novo processo simulado a partir do processo pai (o processo em execução)
    ProcessoSimulado *novoProcesso = criarNovoProcessoAPartirdoPai(gerenciador->cpus[indexCPU].processoEmExecucao, gerenciador->TabelaProcessos.ultimoProcessoIndex);

    // Insere o novo processo na tabela de processos do gerenciador
    inserirTabelaProcessos(novoProcesso, &(gerenciador->TabelaProcessos));

    // Verifica qual algoritmo de escalonamento está em uso e enfileira o novo processo
    if (gerenciador->algoritmoEscalonamento == 0)
    {
        novoProcesso->quantum = getFatiaTempoPrioridade(novoProcesso->prioridade);
        adicionarProcessoProntoFilaDePrioridade(gerenciador, novoProcesso->ID_Processo);
    }
    else if (gerenciador->algoritmoEscalonamento == 1)
    {
        novoProcesso->quantum = QUANTUM;
        enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaPronto), novoProcesso->ID_Processo);
    }

    // Incrementa o contador de programa da CPU, considerando o valor adicional
    gerenciador->cpus[indexCPU].contadorPrograma += (valor + 1);
}

// Comando 'R': Substitui o programa do processo simulado em execução
void comandoR(CPU *cpu, Instrucao instrucao)
{
    // Copia o nome do arquivo de instruções para o conjunto de instruções do processo
    strcpy(cpu->processoEmExecucao->conjuntoInstrucoes, instrucao.arquivo);

    // Reseta o contador de programa do processo para iniciar a nova instrução
    cpu->processoEmExecucao->PC = 0;
    cpu->contadorPrograma = 0;
}

// Comando 'T': Termina o processo em execução na CPU especificada
void comandoT(GerenciadorProcessos *gerenciador, int indexCPU)
{
    // Obtém o índice do processo em execução
    int processoIndex = gerenciador->processosEmExecucao[indexCPU];

    // Obtém o processo simulado a partir do índice
    ProcessoSimulado *processo = getProcesso(&gerenciador->TabelaProcessos, processoIndex);

    // Se o processo for válido, termina-o
    if (processo != NULL)
    {
        // Remove o processo da tabela de processos do gerenciador
        retirarTabelaProcessos(&(gerenciador->TabelaProcessos), processoIndex);

        // Marca a CPU como não tendo nenhum processo em execução
        gerenciador->processosEmExecucao[indexCPU] = -1;

        // Atualiza o tempo médio de execução e o contador de processos terminados
        gerenciador->tempoMedio.valor += processo->tempoCPU.valor;
        gerenciador->processosTerminados += 1;

        // Reinicia a CPU especificada para estar pronta para executar um novo processo
        iniciarCPU(&gerenciador->cpus[indexCPU]);
    }
}

// Processa um comando específico para um determinado processo em execução.
// Dependendo do comando contido na estrutura Instrucao, executa a ação correspondente.
void processarComando(GerenciadorProcessos *gerenciador, Instrucao instrucao, int indexCPU)
{
    switch (instrucao.comando)
    {
    case 'N':
        // Cria um vetor de memória com o tamanho especificado em instrucao.valor.
        comandoN(&(gerenciador->cpus[indexCPU]), instrucao.valor);
        break;
    case 'D':
        // Declara uma nova variável no processo atual, inicializando-a com 0.
        comandoD(&(gerenciador->cpus[indexCPU]), instrucao.valor);
        break;
    case 'V':
        // Define o valor de uma variável na memória do processo atual.
        comandoV(&(gerenciador->cpus[indexCPU]), instrucao.index, instrucao.valor);
        break;
    case 'A':
        // Adiciona o valor especificado a uma variável existente na memória do processo atual.
        comandoA(&(gerenciador->cpus[indexCPU]), instrucao.index, instrucao.valor);
        break;
    case 'S':
        // Subtrai o valor especificado de uma variável existente na memória do processo atual.
        comandoS(&(gerenciador->cpus[indexCPU]), instrucao.index, instrucao.valor);
        break;
    case 'B':
        // Bloqueia o processo em execução, movendo-o para a fila de processos bloqueados.
        comandoB(gerenciador, indexCPU, instrucao.valor);
        break;
    case 'T':
        // Termina o processo atual, removendo-o da tabela de processos e atualizando as métricas.
        comandoT(gerenciador, indexCPU);
        break;
    case 'F':
        // Cria um novo processo simulado com base no processo atual e adiciona-o à fila apropriada.
        comandoF(gerenciador, indexCPU, instrucao.valor);
        break;
    case 'R':
        // Substitui o programa do processo simulado com o conteúdo do arquivo especificado na instrução.
        comandoR(&(gerenciador->cpus[indexCPU]), instrucao);
        break;
    default:
        // Comando não reconhecido; não realiza nenhuma ação.
        break;
    }
    // Decrementa o valor do tempo de fatia da CPU.
    gerenciador->cpus[indexCPU].fatiaTempo.valor--;
    // Atualiza os dados do processo na CPU após a execução do comando.
    atualizaDadosProcesso(&(gerenciador->cpus[indexCPU]));
}

// Inicializa o vetor de memória para a CPU com base na memória do processo em execução.
// Se o processo em execução tem memória alocada, a memória da CPU é apontada para essa memória.
void iniciarVetorMemoria(CPU *cpu)
{
    if (cpu->processoEmExecucao->memoria != NULL)
    {
        cpu->memoriaVect = cpu->processoEmExecucao->memoria;
    }
}

// Inicializa a CPU, configurando seus parâmetros básicos e inicializando o tempo de fatia.
// A CPU começa sem um processo em execução e com o contador de programa zerado.
void iniciarCPU(CPU *cpu)
{
    cpu->processoEmExecucao = NULL;       // Sem processo em execução inicialmente.
    inicializarTempo(&(cpu->tempoUsado)); // Inicializa o tempo utilizado pela CPU.

    cpu->contadorPrograma = 0; // Zera o contador de programa.

    cpu->memoriaVect = NULL; // Inicialmente, sem memória associada.
}

// Função que retorna o número de linhas no arquivo até o primeiro comando 'F' encontrado.
// Se o comando 'F' não for encontrado, retorna o total de linhas do arquivo.
int contarQuantidadeInstrucoes(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return -1; // Retorna -1 em caso de erro ao abrir o arquivo.
    }

    char line[MAX_CMD_LEN];
    int line_count = 0;

    // Lê o arquivo linha por linha.
    while (fgets(line, sizeof(line), file))
    {
        line_count++;
        // Verifica se a linha contém o comando 'F'.
        if (strstr(line, "F") != NULL)
        {
            fclose(file);
            return line_count - 1; // Retorna o número de linhas antes do primeiro 'F'.
        }
    }

    fclose(file);
    return line_count; // Retorna o total de linhas se 'F' não for encontrado.
}

// Inicializa o Gerenciador de Processos com base nos parâmetros fornecidos.
// Configura o número de CPUs, o algoritmo de escalonamento e o processo inicial.
void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada, int PID_Pai, int numsCPUs, int escalonador)
{
    printf("Iniciando gerenciador de processos...\n");
    printf("Iniciado com %d CPUs\n", numsCPUs);

    // Inicializa os tempos de referência para o gerenciamento dos processos.
    inicializarTempo(&gerenciador->tempoAtual);
    inicializarTempo(&gerenciador->tempoMedio);

    // Inicializa contadores e variáveis para o gerenciador.
    gerenciador->processosTerminados = 0;
    gerenciador->tempoMedio.valor = 0;
    gerenciador->algoritmoEscalonamento = escalonador;

    // Inicializa a tabela de processos e cria o processo inicial a partir do arquivo de entrada.
    inicializarTabelaProcessos(&(gerenciador->TabelaProcessos));
    ProcessoSimulado *processo = inicializaProcesso(arquivoEntrada, contarQuantidadeInstrucoes(arquivoEntrada), PID_Pai, 0, gerenciador->algoritmoEscalonamento);
    inserirTabelaProcessos(processo, &(gerenciador->TabelaProcessos));

    // Aloca memória para a estrutura de CPUs e inicializa cada CPU.
    gerenciador->cpus = (CPU *)malloc(sizeof(CPU) * numsCPUs);
    if (gerenciador->cpus == NULL)
    {
        perror("Falha na alocação de memória para CPUs");
        exit(EXIT_FAILURE);
    }
    gerenciador->quantidadeCPUs = numsCPUs;

    // Inicializa a estrutura de escalonamento com base no algoritmo escolhido.
    if (escalonador == 0)
    {
        iniciarFilaDePrioridades(gerenciador);
    }
    else if (escalonador == 1)
    {
        iniciarRoundRobin(gerenciador);
    }

    // Aloca memória para a estrutura de processos em execução e inicializa as CPUs.
    gerenciador->processosEmExecucao = (int *)malloc(sizeof(int) * gerenciador->quantidadeCPUs);
    if (gerenciador->processosEmExecucao == NULL)
    {
        perror("Falha na alocação de memória para processos em execução");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    {
        gerenciador->processosEmExecucao[i] = -1; // Inicializa todos os processadores como sem processo em execução.
        iniciarCPU(&gerenciador->cpus[i]);        // Inicializa cada CPU.
    }
}

// Inicializa a estrutura de escalonamento do tipo Round Robin.
// Cria e inicializa as filas de pronto e bloqueado.
void iniciarRoundRobin(GerenciadorProcessos *gerenciador)
{
    inicializarFilaDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaPronto));
    inicializarFilaDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaBloqueado));

    // Adiciona o processo inicial à fila de processos prontos.
    enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaPronto), 0);
}

// Inicializa a estrutura de escalonamento do tipo Fila de Prioridades.
// Cria e inicializa filas de prontos e bloqueados para cada nível de prioridade.
void iniciarFilaDePrioridades(GerenciadorProcessos *gerenciador)
{
    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        inicializarFilaDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[i]));
        inicializarFilaDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasBloqueados[i]));
    }

    // Adiciona o processo inicial à fila de prioridade de nível 0.
    adicionarProcessoProntoFilaDePrioridade(gerenciador, 0);
}

// Imprime o tempo médio de execução dos processos e o número total de processos terminados.
// Formata a saída para melhor apresentação.
void imprimirTempoMedioProcessos(GerenciadorProcessos gerenciador)
{
    if (gerenciador.processosTerminados > 0)
    {
        // Calcula o tempo médio dividindo o tempo total pelo número de processos terminados.
        gerenciador.tempoMedio.valor = gerenciador.tempoMedio.valor / gerenciador.processosTerminados;
    }
    printf("\n");
    printTableBorder(); // Imprime a borda da tabela para formatação.

    printf("Processos Terminados: %d\n", gerenciador.processosTerminados);
    printf("Tempo medio de Execução dos processos: %d\n", gerenciador.tempoMedio.valor);

    printTableBorder(); // Imprime a borda da tabela novamente para formatação.
    printf("\n");
}

// Lê e processa uma linha específica de um arquivo de comandos.
// Retorna uma estrutura Instrucao com os dados da linha lida.
Instrucao processarLinhaEspecifica(char *caminhoArquivo, int numeroLinha)
{
    Instrucao instrucao;
    instrucao.sucesso = 0;

    // Abre o arquivo para leitura.
    FILE *file = fopen(caminhoArquivo, "r");
    if (!file)
    {
        // Caso não consiga abrir o arquivo, retorna a estrutura Instrucao com sucesso = 0.
        return instrucao;
    }

    char linha[MAX_CMD_LEN];
    int linhaAtual = 0;
    int achou = 0;

    // Lê o arquivo linha por linha.
    while (fgets(linha, sizeof(linha), file) != NULL && !achou)
    {
        linhaAtual++;
        // Verifica se é a linha desejada.
        if (linhaAtual == numeroLinha)
        {
            achou = 1;
            char cmd;
            int v1, v2;

            // Lê o comando da linha.
            if (sscanf(linha, "%c", &cmd) != 1)
            {
                break; // Linha vazia ou inválida, encerra o loop.
            }

            instrucao.comando = cmd;

            // Processa a linha de acordo com o comando.
            switch (cmd)
            {
            case 'N':
            case 'D':
            case 'F':
            case 'B':
                // Comando 'N', 'D' ou 'F' espera um valor inteiro.
                if (sscanf(linha + 2, "%d", &v1) == 1)
                {
                    instrucao.valor = v1;
                }
                break;
            case 'A':
            case 'S':
            case 'V':
                // Comando 'A', 'S' ou 'V' espera dois valores inteiros.
                if (sscanf(linha + 2, "%d %d", &v1, &v2) == 2)
                {
                    instrucao.index = v1;
                    instrucao.valor = v2;
                }
                break;
            case 'R':
                // Comando 'R' espera um caminho de arquivo.
                {
                    char caminhoArquivoNovo[MAX_CMD_LEN - 13]; // Reservar espaço para "./entry/" e ".txt"
                    if (sscanf(linha + 2, "%s", caminhoArquivoNovo) == 1)
                    {
                        snprintf(instrucao.arquivo, MAX_CMD_LEN, "./entry/%s.txt", caminhoArquivoNovo);
                    }
                }
                break;
            default:
                break;
            }
            break; // Encerra a leitura após processar a linha desejada.
        }
    }

    // Verifica se a linha foi encontrada.
    if (linhaAtual < numeroLinha)
    {
        // Linha não encontrada no arquivo.
    }
    else
    {
        instrucao.sucesso = achou; // Marca o sucesso da operação.
    }

    fclose(file); // Fecha o arquivo após a leitura.

    return instrucao;
}

// Atualiza o estado de um processo em execução na CPU especificada.
// Configura a CPU com o processo e as informações correspondentes.
void atualizarProcessoEmExecucao(GerenciadorProcessos *gerenciador, int cpuIndex, int processoId)
{
    ProcessoSimulado *processo = getProcesso(&gerenciador->TabelaProcessos, processoId);

    // Atualiza a CPU com o processo em execução.
    gerenciador->cpus[cpuIndex].processoEmExecucao = processo;
    gerenciador->cpus[cpuIndex].contadorPrograma = processo->PC;

    gerenciador->cpus[cpuIndex].memoriaVect = processo->memoria;
    gerenciador->cpus[cpuIndex].quantidadeInteiros = processo->quantidadeInteiros;

    // Atualiza a estrutura de gerenciamento de processos.
    gerenciador->processosEmExecucao[cpuIndex] = processoId;
    processo->EstadosProcesso = Execucao;

    // TODO : Verificar se a fatia de tempo é apropriada para o processo.
    gerenciador->cpus[cpuIndex].fatiaTempo.valor = processo->quantum;
}

// Imprime uma descrição simplificada de uma instrução de comando.
// Fornece informações sobre a CPU, o processo e a instrução executada.
void printInstrucaoSimplificada(Instrucao instrucao, int cpuIndex, int processoId)
{
    char instrucaoStr[MAX_CMD_LEN + 10];

    // Constrói a string que representa a instrução.
    switch (instrucao.comando)
    {
    case 'N':
    case 'D':
    case 'F':
    case 'T':
    case 'B':
        snprintf(instrucaoStr, sizeof(instrucaoStr), "%c %d", instrucao.comando, instrucao.valor);
        break;
    case 'A':
    case 'S':
    case 'V':
        snprintf(instrucaoStr, sizeof(instrucaoStr), "%c %d %d", instrucao.comando, instrucao.index, instrucao.valor);
        break;
    case 'R':
        snprintf(instrucaoStr, sizeof(instrucaoStr), "%c %s", instrucao.comando, instrucao.arquivo);
        break;
    default:
        snprintf(instrucaoStr, sizeof(instrucaoStr), "Comando desconhecido");
        break;
    }
    // Imprime as informações sobre a CPU, o processo e a instrução.
    printf("CPU %d | executando processo %d | instrução (%s)\n", cpuIndex, processoId, instrucaoStr);
}

// Executa o processo em cada CPU, processando a próxima instrução.
// Lê a instrução atual, imprime uma representação simplificada e a processa.
void executandoProcessoCPU(GerenciadorProcessos *gerenciador)
{
    Instrucao instrucao;
    for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    {
        if (gerenciador->cpus[i].processoEmExecucao != NULL)
        {
            // Processa a próxima linha do arquivo de instruções do processo.
            instrucao = processarLinhaEspecifica(gerenciador->cpus[i].processoEmExecucao->conjuntoInstrucoes, gerenciador->cpus[i].contadorPrograma + 1);

            if (instrucao.sucesso)
            {
                // Imprime uma descrição simplificada da instrução e processa o comando.
                printInstrucaoSimplificada(instrucao, i, gerenciador->cpus[i].processoEmExecucao->ID_Processo);
                processarComando(gerenciador, instrucao, i);
            }
        }
    }
}

// Coloca um processo na CPU especificada da fila de prioridades.
// Desenfileira um processo pronto para a CPU e o atualiza.
void colocaProcessoNaCPUFilaDePrioridades(GerenciadorProcessos *gerenciador, int cpuIndex)
{
    int processoId = -1;

    // Verifica se o índice da CPU é válido.
    if (cpuIndex < 0 || cpuIndex >= gerenciador->quantidadeCPUs)
    {
        return;
    }

    // Tenta desenfileirar um processo da fila de prioridades para a CPU especificada.
    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        processoId = desenfileirarDinamica(&gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[i]);
        if (processoId != -1)
        {
            // Atualiza a CPU com o processo desenfileirado.
            atualizarProcessoEmExecucao(gerenciador, cpuIndex, processoId);
            return; // Sai da função após atribuir o processo à CPU.
        }
    }
}

// Realiza a troca de contexto na CPU especificada.
// Verifica a fatia de tempo e aumenta a prioridade do processo, se necessário.
int trocaDeContexto(GerenciadorProcessos *gerenciador, int i)
{
    int processoEmExecucaoID = gerenciador->processosEmExecucao[i];

    if (processoEmExecucaoID != -1)
    {
        // Verifica se há um processo em execução na CPU.
        ProcessoSimulado *processo = getProcesso(&gerenciador->TabelaProcessos, processoEmExecucaoID);

        // Verifica se a fatia de tempo expirou.
        if (gerenciador->cpus[i].fatiaTempo.valor == 0)
        {
            // Remove o processo da CPU e marca-o como pronto.
            gerenciador->processosEmExecucao[i] = -1;
            processo->EstadosProcesso = Pronto;

            // Reinicializa a CPU para o próximo ciclo.
            iniciarCPU(&gerenciador->cpus[i]);
            return processoEmExecucaoID;
        }
    }

    return -1; // Retorna -1 se não houver troca de contexto.
}

// Atualiza os dados do processo em execução na CPU especificada.
// Atualiza o contador de programa e o tempo de CPU utilizado.
void atualizaDadosProcesso(CPU *cpu)
{
    if (cpu->processoEmExecucao != NULL)
    {
        cpu->processoEmExecucao->PC = cpu->contadorPrograma;
        cpu->processoEmExecucao->tempoCPU.valor += cpu->tempoUsado.valor;
    }
}

// Incrementa o tempo de CPU usado em cada CPU do gerenciador.
void incrementarTempoCPU(GerenciadorProcessos *gerenciador)
{
    for (int x = 0; x < gerenciador->quantidadeCPUs; x++)
    {
        gerenciador->cpus[x].tempoUsado.valor += 1;
    }
}

// Função principal do escalonador para o algoritmo de filas de prioridades.
// Troca o contexto dos processos e executa o próximo processo na CPU.
void escalonadorFilaDePrioridades(GerenciadorProcessos *gerenciador)
{
    verificarBloqueadosFilaDePrioridades(gerenciador);
    trocaDeContextoFilaDePrioridade(gerenciador);
    executandoProcessoCPU(gerenciador);
}

// Troca o contexto dos processos em todas as CPUs utilizando o algoritmo de filas de prioridades.
// Processos são re-enfileirados com prioridade atualizada.
void trocaDeContextoFilaDePrioridade(GerenciadorProcessos *gerenciador)
{
    int idProcesso;
    for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    {
        // Troca o contexto do processo atual na CPU e obtém o ID do processo.
        idProcesso = trocaDeContexto(gerenciador, i);
        if (idProcesso != -1)
        {
            ProcessoSimulado *processo = getProcesso(&gerenciador->TabelaProcessos, idProcesso);
            
            // Aumenta a prioridade do processo se não for a máxima.
            if (processo->prioridade < NUM_PRIORIDADES - 1)
            {
                processo->prioridade++;
                processo->quantum = getFatiaTempoPrioridade(processo->prioridade);
            }
            // Re-enfileira o processo na fila de prontos com a nova prioridade.
            enfileirarDinamica(&gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[processo->prioridade], idProcesso);
        }

        // Se não há processo em execução na CPU, tenta colocar um novo processo.
        if (gerenciador->processosEmExecucao[i] == -1)
        {
            colocaProcessoNaCPUFilaDePrioridades(gerenciador, i);
        }
    }
}

// Função principal do escalonador para o algoritmo Round Robin.
// Troca o contexto dos processos e executa o próximo processo na CPU.
void escalonadorRoundRobin(GerenciadorProcessos *gerenciador)
{
    verificarBloqueadosRoundRobin(gerenciador);
    trocaDeContextoRoundRobin(gerenciador);
    executandoProcessoCPU(gerenciador);
}

// Troca o contexto dos processos em todas as CPUs utilizando o algoritmo Round Robin.
// Processos são re-enfileirados na fila de prontos.
void trocaDeContextoRoundRobin(GerenciadorProcessos *gerenciador)
{
    int idProcesso;
    for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    {
        // Troca o contexto do processo atual na CPU e obtém o ID do processo.
        idProcesso = trocaDeContexto(gerenciador, i);
        if (idProcesso != -1)
        {
            // Re-enfileira o processo na fila de prontos do Round Robin.
            enfileirarDinamica(&gerenciador->EstruturaEscalonamento.roundRobin.filaPronto, idProcesso);
        }

        // Se não há processo em execução na CPU, tenta colocar um novo processo.
        if (gerenciador->processosEmExecucao[i] == -1)
        {
            colocaProcessoNaCPURoundRobin(gerenciador, i);
        }
    }
}

// Coloca um processo na CPU especificada utilizando o algoritmo Round Robin.
// Desenfileira um processo da fila de prontos e o atribui à CPU.
void colocaProcessoNaCPURoundRobin(GerenciadorProcessos *gerenciador, int cpuIndex)
{
    int processoId = -1;

    // Verifica se o índice da CPU é válido.
    if (cpuIndex < 0 || cpuIndex >= gerenciador->quantidadeCPUs)
    {
        return;
    }

    // Tenta desenfileirar um processo para a CPU especificada.
    processoId = desenfileirarDinamica(&gerenciador->EstruturaEscalonamento.roundRobin.filaPronto);
    if (processoId != -1)
    {
        // Atualiza a CPU com o processo desenfileirado.
        atualizarProcessoEmExecucao(gerenciador, cpuIndex, processoId);
    }
}

// Imprime informações sobre todos os processos no gerenciador de processos.
void imprimirTodosProcessos(GerenciadorProcessos *gerenciador)
{
    for (int i = 0; i < gerenciador->TabelaProcessos.ultimoProcessoIndex; i++)
    {
        // Imprime detalhes do processo.
        imprimirProcesso(gerenciador->TabelaProcessos.listaProcessos[i]);
    }
}

// Imprime o estado das filas de prontos e bloqueados conforme o algoritmo de escalonamento.
// Mostra as filas de prioridades ou o estado das filas Round Robin.
void imprimirFilas(GerenciadorProcessos *gerenciador)
{
    if (gerenciador->algoritmoEscalonamento == 0)
    {
        printf("\n===========================================================\n");
        printf("|      Filas usadas no algoritmo de escalonamento          |\n");
        printf("|                 Filas De Prioridades                     |\n");
        printf("===========================================================\n");
        for (int i = 0; i < NUM_PRIORIDADES; i++)
        {
            printf("Fila de Prontos da prioridade %d: \n", i);
            imprimirFilaDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[i]));
            printf("\n");
            printf("Fila de Bloqueados da prioridade %d: \n", i);
            imprimirFilaDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasBloqueados[i]));
            printf("\n");
        }
    }
    else if (gerenciador->algoritmoEscalonamento == 1)
    {
        printf("\n===========================================================\n");
        printf("|      Filas usadas no algoritmo de escalonamento         |\n");
        printf("|                     Round Robin                         |\n");
        printf("===========================================================\n");
        printf("Fila de Prontos: \n");
        imprimirFilaDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaPronto));
        printf("\n");
        printf("Fila de Bloqueados: \n");
        imprimirFilaDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaBloqueado));
        printf("\n");
    }
}

int getFatiaTempoPrioridade(int prioridade)
{
    switch (prioridade)
    {
    case 0:
        return prioridade0;
    case 1:
        return prioridade1;
    case 2:
        return prioridade2;
    case 3:
        return prioridade3;
    default:
        return prioridade0;
    }
}

void verificarBloqueadosFilaDePrioridades(GerenciadorProcessos *gerenciador)
{
    // Percorrer cada fila de bloqueados de acordo com a prioridade
    for(int i = 0; i < NUM_PRIORIDADES; i++)
    {
        FilaDinamica *filaBloqueados = &(gerenciador->EstruturaEscalonamento.filaPrioridades.filasBloqueados[i]);
        Node *atual = filaBloqueados->frente;

        
        while(atual != NULL)
        {
            ProcessoSimulado *processo = getProcesso(gerenciador, atual->dado);

            if(processo->tempoBloqueado.valor == 0)
            {
                Node *proximo = atual->proximo; // Salva o próximo nó antes de remover o atual
                removerNo(filaBloqueados, atual);
                adicionarProcessoProntoFilaDePrioridade(gerenciador, processo->ID_Processo);
                atual = proximo; // Avança para o próximo nó
            }
            else
            {
                processo->tempoBloqueado.valor--;
                atual = atual->proximo; // Avança para o próximo nó
            }
        }
    }
}

void verificarBloqueadosRoundRobin(GerenciadorProcessos *gerenciador)
{
    FilaDinamica *filaBloqueados = &(gerenciador->EstruturaEscalonamento.roundRobin.filaBloqueado);
    Node *atual = filaBloqueados->frente;

    while(atual != NULL)
    {
        ProcessoSimulado *processo = getProcesso(gerenciador, atual->dado);

        if(processo->tempoBloqueado.valor == 0)
        {
            Node *proximo = atual->proximo; // Salva o próximo nó antes de remover o atual
            removerNo(filaBloqueados, atual);
            enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaPronto), processo->ID_Processo);
            atual = proximo; // Avança para o próximo nó
        }
        else
        {
            processo->tempoBloqueado.valor--;
            atual = atual->proximo; // Avança para o próximo nó
        }
    }
}

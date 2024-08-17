#ifndef PROCESSOCONTROLE_H
#define PROCESSOCONTROLE_H

// Declarações de funções para o controle de processos
//---------------------------------------------------------------------------------------

// Função principal de controle de processos, responsável por gerenciar o ciclo de vida dos processos.
int processoControle();

// Lê o conteúdo de um arquivo e armazena-o na string `retorno`.
void lerArquivo(char *retorno);

// Lê a entrada fornecida pelo usuário através do terminal e armazena-a na string `retorno`.
void lerTerminal(char *retorno);

// Remove todas as ocorrências do caractere `c` na string `str`.
void remove_char(char *str, char c);

//---------------------------------------------------------------------------------------

#endif // PROCESSOCONTROLE_H

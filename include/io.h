#ifndef IO_H
#define IO_H

#include "mochila.h"
#include <stdio.h>

/**
 * Lê uma instância da mochila a partir de um arquivo aberto ou stdin.
 * Formato esperado:
 *   capacidade
 *   n
 *   peso_1 valor_1
 *   ...
 *   peso_n valor_n
 *   matriz n x n (dependências)
 *
 * Linhas em branco ou iniciadas com '#' são tratadas como comentários.
 *
 * @param stream Fluxo de entrada (arquivo ou stdin).
 * @param capacidade Ponteiro de saída para a capacidade lida.
 * @param n Ponteiro de saída para o número de itens lido.
 * @param itens Ponteiro de saída para o vetor de itens alocado.
 * @param grafo Ponteiro de saída para a matriz de dependências alocada.
 * @return 0 em caso de sucesso, -1 em caso de erro na leitura ou formato inválido.
 */
int ler_instancia(FILE *stream, int *capacidade, int *n, Item **itens, int ***grafo);

/**
 * Lê uma instância de forma interativa via terminal, com mensagens guiadas.
 *
 * @param capacidade Ponteiro de saída para a capacidade.
 * @param n Ponteiro de saída para o número de itens.
 * @param itens Ponteiro de saída para o vetor de itens alocado.
 * @param grafo Ponteiro de saída para a matriz de dependências alocada.
 * @return 0 em caso de sucesso, -1 em caso de erro.
 */
int ler_instancia_interativa(int *capacidade, int *n, Item **itens, int ***grafo);

/**
 * Cria em memória a instância clássica utilizada como exemplo de aula.
 *
 * @param capacidade Ponteiro de saída para a capacidade (5).
 * @param n Ponteiro de saída para a quantidade de itens (4).
 * @param itens Ponteiro de saída para os itens alocados.
 * @param grafo Ponteiro de saída para o grafo de dependências alocado.
 * @return 0 em caso de sucesso, -1 em caso de falha de alocação.
 */
int carregar_exemplo_aula(int *capacidade, int *n, Item **itens, int ***grafo);

/**
 * Exibe as informações da instância (itens e lista de dependências).
 *
 * @param capacidade Capacidade máxima.
 * @param n Quantidade de itens.
 * @param itens Vetor de itens.
 * @param grafo Matriz de dependências.
 */
void imprimir_instancia(int capacidade, int n, const Item *itens, int **grafo);

/**
 * Exibe a tabela de programação dinâmica formatada em grid.
 *
 * @param res Resultado da mochila contendo a tabela.
 * @param itens Vetor com os itens para exibir rótulos informativos nas linhas.
 */
void imprimir_tabela_dp(const ResultadoMochila *res, const Item *itens);

/**
 * Exibe o relatório final da solução com os objetos incluídos, peso total e valor ótimo.
 *
 * @param res Estrutura de resultado calculada.
 * @param itens Vetor com os itens originais.
 */
void imprimir_solucao(const ResultadoMochila *res, const Item *itens);

#endif /* IO_H */


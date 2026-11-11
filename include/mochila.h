#ifndef MOCHILA_H
#define MOCHILA_H

#include <stddef.h>

/**
 * Estrutura representando um item com peso e valor associados.
 */
typedef struct {
    int id;    /**< Identificador do item (1-indexed para exibição amigável) */
    int peso;  /**< Peso do item */
    int valor; /**< Valor/benefício do item */
} Item;

/**
 * Estrutura com o resultado completo da resolução da mochila.
 */
typedef struct {
    int n;              /**< Quantidade de itens */
    int capacidade;     /**< Capacidade máxima da mochila */
    int valor_maximo;   /**< Valor ótimo alcançado */
    int peso_total;     /**< Soma dos pesos dos itens selecionados */
    int *incluso;       /**< Vetor de flags (tamanho n): 1 se item i foi incluído, 0 se não */
    int **tabela;       /**< Matriz DP de dimensões (n + 1) x (capacidade + 1) */
} ResultadoMochila;

/**
 * Aloca uma matriz contígua em memória de inteiros de tamanho linhas x colunas.
 * Inicializa todos os elementos com zero.
 *
 * @param linhas Número de linhas.
 * @param colunas Número de colunas.
 * @return Ponteiro para vetor de ponteiros das linhas, ou NULL se falhar.
 */
int **alocar_matriz(int linhas, int colunas);

/**
 * Libera a memória de uma matriz alocada por alocar_matriz.
 *
 * @param matriz Ponteiro para a matriz a ser liberada.
 */
void liberar_matriz(int **matriz);

/**
 * Libera todos os recursos alocados na estrutura ResultadoMochila.
 *
 * @param res Ponteiro para a estrutura de resultado.
 */
void liberar_resultado(ResultadoMochila *res);

/**
 * Verifica se todas as dependências do objeto foram satisfeitas
 * para a capacidade considerada na tabela de programação dinâmica.
 *
 * @param objeto Índice do objeto atual (0 a n-1).
 * @param capacidade_atual Capacidade restante considerada.
 * @param tabela Matriz de programação dinâmica.
 * @param grafo Matriz de dependências (grafo[i][k] == 1 se item i depende de item k).
 * @param n Número total de itens.
 * @return 1 se todas as dependências estão satisfeitas (ou se não há dependências),
 *         0 se pelo menos uma dependência não foi incluída.
 */
int dependencias_satisfeitas(int objeto, int capacidade_atual, int **tabela, int **grafo, int n);

/**
 * Determina recursivamente os objetos incluídos na solução ótima através do
 * backtracking na tabela de programação dinâmica.
 *
 * @param i Índice da linha atual na tabela (inicia em n, vai até 0).
 * @param j Capacidade restante considerada (inicia em capacidade, vai até 0).
 * @param itens Vetor contendo os itens (pesos e valores).
 * @param tabela Matriz de programação dinâmica calculada.
 * @param incluso Vetor de tamanho n onde itens selecionados recebem valor 1.
 */
void determinar_objetos_inclusos(int i, int j, const Item *itens, int **tabela, int *incluso);

/**
 * Resolve o Problema da Mochila com Dependência usando Programação Dinâmica.
 *
 * @param itens Vetor com os n itens.
 * @param grafo Matriz de adjacência n x n indicando as dependências.
 * @param n Quantidade de itens.
 * @param capacidade Capacidade máxima da mochila.
 * @return Ponteiro para ResultadoMochila com a solução, ou NULL em caso de erro.
 */
ResultadoMochila *resolver_mochila(const Item *itens, int **grafo, int n, int capacidade);

#endif /* MOCHILA_H */

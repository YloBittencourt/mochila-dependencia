#include "mochila.h"
#include <stdlib.h>

int **alocar_matriz(int linhas, int colunas) {
    if (linhas <= 0 || colunas <= 0) {
        return NULL;
    }

    int **matriz = (int **)malloc((size_t)linhas * sizeof(int *));
    if (!matriz) {
        return NULL;
    }

    int *bloco = (int *)calloc((size_t)linhas * (size_t)colunas, sizeof(int));
    if (!bloco) {
        free(matriz);
        return NULL;
    }

    for (int i = 0; i < linhas; i++) {
        matriz[i] = bloco + (i * colunas);
    }

    return matriz;
}

void liberar_matriz(int **matriz) {
    if (!matriz) {
        return;
    }
    free(matriz[0]); // Libera o bloco contíguo alocado com calloc
    free(matriz);    // Libera o vetor de ponteiros
}

void liberar_resultado(ResultadoMochila *res) {
    if (!res) {
        return;
    }
    if (res->tabela) {
        liberar_matriz(res->tabela);
        res->tabela = NULL;
    }
    if (res->incluso) {
        free(res->incluso);
        res->incluso = NULL;
    }
    free(res);
}

int dependencias_satisfeitas(int objeto, int capacidade_atual, int **tabela, int **grafo, int n) {
    if (!grafo) {
        return 1;
    }

    for (int k = 0; k < n; k++) {
        // Se existe uma aresta objeto -> k, o objeto depende do item k
        if (grafo[objeto][k]) {
            // Na Programação Dinâmica, se tabela[k+1][cap] == tabela[k][cap],
            // o item k não foi selecionado na subsolução com a capacidade restante.
            if (tabela[k + 1][capacidade_atual] == tabela[k][capacidade_atual]) {
                return 0; // Dependência obrigatória ausente
            }
        }
    }
    return 1; // Todas as dependências foram atendidas
}

void determinar_objetos_inclusos(int i, int j, const Item *itens, int **tabela, int *incluso) {
    // Caso base: nenhum item restante ou capacidade esgotada
    if (i <= 0 || j <= 0) {
        return;
    }

    // Se o valor na tabela difere da linha anterior, o item (i - 1) foi incluído
    if (tabela[i][j] != tabela[i - 1][j]) {
        incluso[i - 1] = 1;
        int cap_restante = j - itens[i - 1].peso;
        if (cap_restante < 0) {
            cap_restante = 0;
        }
        determinar_objetos_inclusos(i - 1, cap_restante, itens, tabela, incluso);
    } else {
        // O item (i - 1) não foi selecionado
        determinar_objetos_inclusos(i - 1, j, itens, tabela, incluso);
    }
}

ResultadoMochila *resolver_mochila(const Item *itens, int **grafo, int n, int capacidade) {
    if (n < 0 || capacidade < 0) {
        return NULL;
    }

    ResultadoMochila *res = (ResultadoMochila *)malloc(sizeof(ResultadoMochila));
    if (!res) {
        return NULL;
    }

    res->n = n;
    res->capacidade = capacidade;
    res->valor_maximo = 0;
    res->peso_total = 0;
    res->incluso = NULL;
    res->tabela = NULL;

    if (n == 0 || capacidade == 0) {
        res->incluso = (int *)calloc((size_t)(n > 0 ? n : 1), sizeof(int));
        res->tabela = alocar_matriz(n + 1, capacidade + 1);
        return res;
    }

    res->tabela = alocar_matriz(n + 1, capacidade + 1);
    res->incluso = (int *)calloc((size_t)n, sizeof(int));

    if (!res->tabela || !res->incluso) {
        liberar_resultado(res);
        return NULL;
    }

    // Preenchimento da tabela por Programação Dinâmica
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= capacidade; j++) {
            // Opção 1: não incluir o item i (herda valor da linha anterior)
            res->tabela[i + 1][j] = res->tabela[i][j];

            // Opção 2: avaliar se o item cabe e suas dependências estão satisfeitas
            if (itens[i].peso <= j) {
                int cap_restante = j - itens[i].peso;
                if (dependencias_satisfeitas(i, cap_restante, res->tabela, grafo, n)) {
                    int valor_com_item = res->tabela[i][cap_restante] + itens[i].valor;
                    if (valor_com_item > res->tabela[i + 1][j]) {
                        res->tabela[i + 1][j] = valor_com_item;
                    }
                }
            }
        }
    }

    res->valor_maximo = res->tabela[n][capacidade];

    // Backtracking recursivo para identificar itens escolhidos
    determinar_objetos_inclusos(n, capacidade, itens, res->tabela, res->incluso);

    // Cálculo do peso total selecionado
    int peso_acumulado = 0;
    for (int k = 0; k < n; k++) {
        if (res->incluso[k]) {
            peso_acumulado += itens[k].peso;
        }
    }
    res->peso_total = peso_acumulado;

    return res;
}

#include "io.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

/**
 * Lê a próxima linha não comentada e não vazia do arquivo.
 */
static int ler_linha_valida(FILE *stream, char *buffer, size_t tamanho) {
    while (fgets(buffer, (int)tamanho, stream) != NULL) {
        // Ignora espaços em branco iniciais
        char *p = buffer;
        while (isspace((unsigned char)*p)) {
            p++;
        }
        // Se a linha for vazia ou comentário iniciado com '#', pula
        if (*p == '\0' || *p == '#') {
            continue;
        }
        return 1; // Linha válida encontrada
    }
    return 0; // Fim do arquivo
}

int ler_instancia(FILE *stream, int *capacidade, int *n, Item **itens, int ***grafo) {
    if (!stream || !capacidade || !n || !itens || !grafo) {
        return -1;
    }

    char buffer[BUFFER_SIZE];

    // Lê capacidade
    if (!ler_linha_valida(stream, buffer, sizeof(buffer))) {
        return -1;
    }
    if (sscanf(buffer, "%d", capacidade) != 1 || *capacidade < 0) {
        return -1;
    }

    // Lê número de itens
    if (!ler_linha_valida(stream, buffer, sizeof(buffer))) {
        return -1;
    }
    if (sscanf(buffer, "%d", n) != 1 || *n < 0) {
        return -1;
    }

    *itens = (Item *)malloc((size_t)(*n) * sizeof(Item));
    if (!*itens) {
        return -1;
    }

    // Lê peso e valor de cada item
    for (int i = 0; i < *n; i++) {
        if (!ler_linha_valida(stream, buffer, sizeof(buffer))) {
            free(*itens);
            *itens = NULL;
            return -1;
        }
        (*itens)[i].id = i + 1;
        if (sscanf(buffer, "%d %d", &(*itens)[i].peso, &(*itens)[i].valor) != 2) {
            free(*itens);
            *itens = NULL;
            return -1;
        }
    }

    // Aloca a matriz do grafo
    *grafo = alocar_matriz(*n, *n);
    if (!*grafo) {
        free(*itens);
        *itens = NULL;
        return -1;
    }

    // Lê a matriz de dependências n x n
    for (int i = 0; i < *n; i++) {
        if (!ler_linha_valida(stream, buffer, sizeof(buffer))) {
            liberar_matriz(*grafo);
            free(*itens);
            *grafo = NULL;
            *itens = NULL;
            return -1;
        }
        char *ptr = buffer;
        for (int j = 0; j < *n; j++) {
            char *endptr;
            long val = strtol(ptr, &endptr, 10);
            if (ptr == endptr) {
                liberar_matriz(*grafo);
                free(*itens);
                *grafo = NULL;
                *itens = NULL;
                return -1;
            }
            (*grafo)[i][j] = (int)val;
            ptr = endptr;
        }
    }

    return 0;
}

int ler_instancia_interativa(int *capacidade, int *n, Item **itens, int ***grafo) {
    printf("\n--- Entrada Interativa de Dados ---\n");
    printf("Digite a capacidade da mochila: ");
    if (scanf("%d", capacidade) != 1 || *capacidade < 0) {
        return -1;
    }

    printf("Digite o número de objetos: ");
    if (scanf("%d", n) != 1 || *n <= 0) {
        return -1;
    }

    *itens = (Item *)malloc((size_t)(*n) * sizeof(Item));
    if (!*itens) {
        return -1;
    }

    printf("\nDigite o peso e o valor de cada objeto:\n");
    for (int i = 0; i < *n; i++) {
        (*itens)[i].id = i + 1;
        printf("Objeto %d (peso valor): ", i + 1);
        if (scanf("%d %d", &(*itens)[i].peso, &(*itens)[i].valor) != 2) {
            free(*itens);
            *itens = NULL;
            return -1;
        }
    }

    *grafo = alocar_matriz(*n, *n);
    if (!*grafo) {
        free(*itens);
        *itens = NULL;
        return -1;
    }

    printf("\nDigite a matriz de dependência (%dx%d):\n", *n, *n);
    printf("(1 na posição [i][j] indica que Objeto (i+1) depende do Objeto (j+1))\n");
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            if (scanf("%d", &(*grafo)[i][j]) != 1) {
                liberar_matriz(*grafo);
                free(*itens);
                *grafo = NULL;
                *itens = NULL;
                return -1;
            }
        }
    }

    return 0;
}

int carregar_exemplo_aula(int *capacidade, int *n, Item **itens, int ***grafo) {
    *capacidade = 5;
    *n = 4;

    *itens = (Item *)malloc((size_t)(*n) * sizeof(Item));
    if (!*itens) {
        return -1;
    }

    int pesos[] = {1, 3, 2, 4};
    int valores[] = {8, 7, 5, 9};

    for (int i = 0; i < *n; i++) {
        (*itens)[i].id = i + 1;
        (*itens)[i].peso = pesos[i];
        (*itens)[i].valor = valores[i];
    }

    *grafo = alocar_matriz(*n, *n);
    if (!*grafo) {
        free(*itens);
        *itens = NULL;
        return -1;
    }

    // Grafo de dependência:
    // Objeto 2 depende do Objeto 1
    (*grafo)[1][0] = 1;
    // Objeto 3 depende do Objeto 1
    (*grafo)[2][0] = 1;
    // Objeto 4 depende do Objeto 3
    (*grafo)[3][2] = 1;

    return 0;
}

void imprimir_instancia(int capacidade, int n, const Item *itens, int **grafo) {
    printf("=====================================================\n");
    printf("                DADOS DA INSTÂNCIA                   \n");
    printf("=====================================================\n");
    printf("Capacidade da mochila: %d\n", capacidade);
    printf("Quantidade de objetos: %d\n\n", n);

    printf("Itens disponíveis:\n");
    printf("  ID  | Peso | Valor | Depende de\n");
    printf("------+------+-------+------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("  %-3d |  %-3d |  %-4d | ", itens[i].id, itens[i].peso, itens[i].valor);
        int tem_dep = 0;
        for (int j = 0; j < n; j++) {
            if (grafo && grafo[i][j]) {
                if (tem_dep) {
                    printf(", ");
                }
                printf("Objeto %d", j + 1);
                tem_dep = 1;
            }
        }
        if (!tem_dep) {
            printf("Nenhuma");
        }
        printf("\n");
    }
    printf("-----------------------------------------------------\n\n");
}

void imprimir_tabela_dp(const ResultadoMochila *res, const Item *itens) {
    if (!res || !res->tabela) {
        return;
    }

    printf("=====================================================\n");
    printf("           TABELA DE PROGRAMAÇÃO DINÂMICA            \n");
    printf("=====================================================\n");

    // Cabeçalho com as capacidades (colunas)
    printf("%-16s |", "Item / Cap");
    for (int j = 0; j <= res->capacidade; j++) {
        printf("%4d", j);
    }
    printf("\n");

    printf("-----------------+");
    for (int j = 0; j <= res->capacidade; j++) {
        printf("----");
    }
    printf("\n");

    // Linha 0 (Caso base: 0 itens)
    printf("%-16s |", "Base (0 itens)");
    for (int j = 0; j <= res->capacidade; j++) {
        printf("%4d", res->tabela[0][j]);
    }
    printf("\n");

    // Linhas 1 a n
    for (int i = 1; i <= res->n; i++) {
        char rotulo[32];
        if (itens) {
            snprintf(rotulo, sizeof(rotulo), "Obj %d (P:%d, V:%d)", itens[i - 1].id, itens[i - 1].peso, itens[i - 1].valor);
        } else {
            snprintf(rotulo, sizeof(rotulo), "Objeto %d", i);
        }
        printf("%-16s |", rotulo);
        for (int j = 0; j <= res->capacidade; j++) {
            printf("%4d", res->tabela[i][j]);
        }
        printf("\n");
    }
    printf("-----------------------------------------------------\n\n");
}

void imprimir_solucao(const ResultadoMochila *res, const Item *itens) {
    if (!res) {
        return;
    }

    printf("=====================================================\n");
    printf("                  SOLUÇÃO ÓTIMA                      \n");
    printf("=====================================================\n");
    printf("Valor máximo alcançado: %d\n", res->valor_maximo);
    printf("Peso total utilizado:   %d / %d\n\n", res->peso_total, res->capacidade);

    printf("Objetos incluídos na mochila:\n");
    printf("  ID  | Peso | Valor\n");
    printf("------+------+-------\n");

    int qtd_inclusos = 0;
    for (int k = 0; k < res->n; k++) {
        if (res->incluso[k]) {
            printf("  %-3d |  %-3d |  %-4d\n", itens[k].id, itens[k].peso, itens[k].valor);
            qtd_inclusos++;
        }
    }

    if (qtd_inclusos == 0) {
        printf("  (Nenhum objeto foi incluído)\n");
    }
    printf("=====================================================\n");
}

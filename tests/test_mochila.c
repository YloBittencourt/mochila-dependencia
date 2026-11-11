#include "mochila.h"
#include <stdio.h>
#include <stdlib.h>

#define ANSI_GREEN "\033[0;32m"
#define ANSI_RED   "\033[0;31m"
#define ANSI_RESET "\033[0m"

static int total_testes = 0;
static int testes_passados = 0;

#define ASSERT_TRUE(cond, msg) do { \
    total_testes++; \
    if (cond) { \
        testes_passados++; \
        printf("  [" ANSI_GREEN "PASS" ANSI_RESET "] %s\n", msg); \
    } else { \
        printf("  [" ANSI_RED "FAIL" ANSI_RESET "] %s (Linha %d)\n", msg, __LINE__); \
    } \
} while (0)

static void test_exemplo_aula(void) {
    printf("\n[TESTE] Exemplo da Aula (Capacidade 5, 4 Itens com Dependência)\n");

    Item itens[] = {
        {1, 1, 8},
        {2, 3, 7},
        {3, 2, 5},
        {4, 4, 9}
    };
    int n = 4;
    int capacidade = 5;

    int **grafo = alocar_matriz(n, n);
    grafo[1][0] = 1; // 2 depende de 1
    grafo[2][0] = 1; // 3 depende de 1
    grafo[3][2] = 1; // 4 depende de 3

    ResultadoMochila *res = resolver_mochila(itens, grafo, n, capacidade);

    ASSERT_TRUE(res != NULL, "Resultado não deve ser nulo");
    ASSERT_TRUE(res->valor_maximo == 15, "Valor máximo deve ser 15");
    ASSERT_TRUE(res->peso_total == 4, "Peso total deve ser 4 (Item 1 + Item 2)");
    ASSERT_TRUE(res->incluso[0] == 1, "Item 1 deve estar incluído");
    ASSERT_TRUE(res->incluso[1] == 1, "Item 2 deve estar incluído");
    ASSERT_TRUE(res->incluso[2] == 0, "Item 3 NÃO deve estar incluído");
    ASSERT_TRUE(res->incluso[3] == 0, "Item 4 NÃO deve estar incluído");

    liberar_resultado(res);
    liberar_matriz(grafo);
}

static void test_sem_dependencias(void) {
    printf("\n[TESTE] Mochila Clássica 0/1 (Sem Dependências)\n");

    Item itens[] = {
        {1, 2, 10},
        {2, 3, 15},
        {3, 4, 20},
        {4, 5, 25}
    };
    int n = 4;
    int capacidade = 7;

    int **grafo = alocar_matriz(n, n); // Todo zerado

    ResultadoMochila *res = resolver_mochila(itens, grafo, n, capacidade);

    ASSERT_TRUE(res != NULL, "Resultado não deve ser nulo");
    ASSERT_TRUE(res->valor_maximo == 35, "Valor máximo deve ser 35 (Itens 2 e 3 ou 1 e 4)");
    ASSERT_TRUE(res->peso_total <= capacidade, "Peso total deve ser menor ou igual à capacidade");

    liberar_resultado(res);
    liberar_matriz(grafo);
}

static void test_cadeia_dependencias(void) {
    printf("\n[TESTE] Cadeia de Dependências (3 -> 2 -> 1)\n");

    Item itens[] = {
        {1, 2, 5},
        {2, 2, 10},
        {3, 2, 30}
    };
    int n = 3;

    int **grafo = alocar_matriz(n, n);
    grafo[1][0] = 1; // 2 depende de 1
    grafo[2][1] = 1; // 3 depende de 2

    // Cenário 1: Capacidade 6 (cabem todos os 3)
    ResultadoMochila *res1 = resolver_mochila(itens, grafo, n, 6);
    ASSERT_TRUE(res1 != NULL, "Resultado 1 não nulo");
    ASSERT_TRUE(res1->valor_maximo == 45, "Capacidade 6: Valor deve ser 45 (5 + 10 + 30)");
    ASSERT_TRUE(res1->incluso[0] == 1 && res1->incluso[1] == 1 && res1->incluso[2] == 1,
                "Capacidade 6: Todos os 3 itens devem estar inclusos");
    liberar_resultado(res1);

    // Cenário 2: Capacidade 4 (só cabem itens 1 e 2)
    ResultadoMochila *res2 = resolver_mochila(itens, grafo, n, 4);
    ASSERT_TRUE(res2 != NULL, "Resultado 2 não nulo");
    ASSERT_TRUE(res2->valor_maximo == 15, "Capacidade 4: Valor deve ser 15 (5 + 10)");
    ASSERT_TRUE(res2->incluso[2] == 0, "Capacidade 4: Item 3 não pode entrar sem estourar capacidade da cadeia");
    liberar_resultado(res2);

    liberar_matriz(grafo);
}

static void test_capacidade_insuficiente(void) {
    printf("\n[TESTE] Capacidade Insuficiente\n");

    Item itens[] = {
        {1, 5, 10},
        {2, 3, 8}
    };
    int n = 2;
    int capacidade = 1;

    int **grafo = alocar_matriz(n, n);

    ResultadoMochila *res = resolver_mochila(itens, grafo, n, capacidade);

    ASSERT_TRUE(res != NULL, "Resultado não nulo");
    ASSERT_TRUE(res->valor_maximo == 0, "Nenhum item cabe, valor máximo deve ser 0");
    ASSERT_TRUE(res->peso_total == 0, "Peso total deve ser 0");
    ASSERT_TRUE(res->incluso[0] == 0 && res->incluso[1] == 0, "Nenhum item deve estar incluso");

    liberar_resultado(res);
    liberar_matriz(grafo);
}

static void test_dependencia_bloqueada(void) {
    printf("\n[TESTE] Item Valioso Bloqueado por Pré-requisito Pesado\n");

    // Item 2 é muito valioso (valor 1000, peso 1), mas depende do Item 1 (peso 10)
    // Capacidade é 5, logo Item 1 não cabe e Item 2 não pode ser pego
    Item itens[] = {
        {1, 10, 10},
        {2, 1, 1000}
    };
    int n = 2;
    int capacidade = 5;

    int **grafo = alocar_matriz(n, n);
    grafo[1][0] = 1; // Item 2 depende de Item 1

    ResultadoMochila *res = resolver_mochila(itens, grafo, n, capacidade);

    ASSERT_TRUE(res != NULL, "Resultado não nulo");
    ASSERT_TRUE(res->valor_maximo == 0, "Item 2 não pode ser incluído porque pré-requisito não cabe");
    ASSERT_TRUE(res->incluso[1] == 0, "Item 2 deve ser rejeitado");

    liberar_resultado(res);
    liberar_matriz(grafo);
}

static void test_casos_limite(void) {
    printf("\n[TESTE] Casos Limite (n=0 ou capacidade=0)\n");

    Item itens[] = {{1, 2, 10}};
    ResultadoMochila *res1 = resolver_mochila(itens, NULL, 1, 0);
    ASSERT_TRUE(res1 != NULL && res1->valor_maximo == 0, "Capacidade 0 deve retornar valor 0");
    liberar_resultado(res1);

    ResultadoMochila *res2 = resolver_mochila(itens, NULL, 0, 10);
    ASSERT_TRUE(res2 != NULL && res2->valor_maximo == 0, "0 Itens deve retornar valor 0");
    liberar_resultado(res2);
}

int main(void) {
    printf("=====================================================\n");
    printf("        SUÍTE DE TESTES: MOCHILA COM DEPENDÊNCIA     \n");
    printf("=====================================================\n");

    test_exemplo_aula();
    test_sem_dependencias();
    test_cadeia_dependencias();
    test_capacidade_insuficiente();
    test_dependencia_bloqueada();
    test_casos_limite();

    printf("\n=====================================================\n");
    printf("Resumo: %d de %d testes passaram.\n", testes_passados, total_testes);
    printf("=====================================================\n");

    return (testes_passados == total_testes) ? 0 : 1;
}

#define _POSIX_C_SOURCE 200809L
#include "io.h"
#include "mochila.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void exibir_ajuda(const char *prog) {
    printf("Uso: %s [OPÇÕES] [ARQUIVO]\n\n", prog);
    printf("Resolve o Problema da Mochila com Dependência (PCKP) usando Programação Dinâmica.\n\n");
    printf("Opções:\n");
    printf("  -e, --exemplo      Executa a instância didática do exemplo de aula\n");
    printf("  -i, --interativo   Entra no modo interativo guiado via terminal\n");
    printf("  -t, --tabela       Exibe a tabela de Programação Dinâmica detalhada\n");
    printf("  -h, --help         Exibe este menu de ajuda\n\n");
    printf("Exemplos de uso:\n");
    printf("  %s --exemplo\n", prog);
    printf("  %s tests/inputs/exemplo_aula.txt\n", prog);
    printf("  %s -t tests/inputs/exemplo_aula.txt\n", prog);
    printf("  %s < tests/inputs/sem_dependencia.txt\n", prog);
}

int main(int argc, char *argv[]) {
    int capacidade = 0;
    int n = 0;
    Item *itens = NULL;
    int **grafo = NULL;

    int mostrar_tabela = 0;
    int modo_exemplo = 0;
    int modo_interativo = 0;
    const char *caminho_arquivo = NULL;

    // Processamento de argumentos de linha de comando
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            exibir_ajuda(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--exemplo") == 0) {
            modo_exemplo = 1;
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interativo") == 0) {
            modo_interativo = 1;
        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tabela") == 0) {
            mostrar_tabela = 1;
        } else if (argv[i][0] != '-') {
            caminho_arquivo = argv[i];
        } else {
            fprintf(stderr, "Opção desconhecida: %s (use --help para ajuda)\n", argv[i]);
            return 1;
        }
    }

    // Carregamento dos dados
    if (modo_exemplo) {
        if (carregar_exemplo_aula(&capacidade, &n, &itens, &grafo) != 0) {
            fprintf(stderr, "Erro ao carregar o exemplo de aula.\n");
            return 1;
        }
        printf("[INFO] Executando exemplo clássico da aula...\n\n");
    } else if (modo_interativo) {
        if (ler_instancia_interativa(&capacidade, &n, &itens, &grafo) != 0) {
            fprintf(stderr, "Erro na leitura interativa dos dados.\n");
            return 1;
        }
    } else if (caminho_arquivo) {
        FILE *fp = fopen(caminho_arquivo, "r");
        if (!fp) {
            perror("Erro ao abrir arquivo");
            return 1;
        }
        if (ler_instancia(fp, &capacidade, &n, &itens, &grafo) != 0) {
            fprintf(stderr, "Erro ao processar o formato do arquivo: %s\n", caminho_arquivo);
            fclose(fp);
            return 1;
        }
        fclose(fp);
    } else if (!isatty(fileno(stdin))) {
        // Entrada redirecionada via pipe ou arquivo (ex: ./mochila < dados.txt)
        if (ler_instancia(stdin, &capacidade, &n, &itens, &grafo) != 0) {
            fprintf(stderr, "Erro ao ler dados da entrada padrão (stdin).\n");
            return 1;
        }
    } else {
        // Se nenhum parâmetro foi passado em terminal interativo, roda o exemplo da aula e avisa o usuário
        printf("Nenhum arquivo fornecido. Executando o exemplo de aula por padrão...\n");
        printf("(Dica: utilize '%s --help' para ver as opções completas de uso)\n\n", argv[0]);
        if (carregar_exemplo_aula(&capacidade, &n, &itens, &grafo) != 0) {
            fprintf(stderr, "Erro ao inicializar exemplo da aula.\n");
            return 1;
        }
    }

    // Exibe dados da instância
    imprimir_instancia(capacidade, n, itens, grafo);

    // Executa a resolução por Programação Dinâmica
    ResultadoMochila *res = resolver_mochila(itens, grafo, n, capacidade);
    if (!res) {
        fprintf(stderr, "Erro durante a resolução da mochila.\n");
        free(itens);
        liberar_matriz(grafo);
        return 1;
    }

    // Exibe tabela DP se solicitado
    if (mostrar_tabela) {
        imprimir_tabela_dp(res, itens);
    }

    // Exibe resultado e itens escolhidos
    imprimir_solucao(res, itens);

    // Liberação de memória
    liberar_resultado(res);
    free(itens);
    liberar_matriz(grafo);

    return 0;
}

/*
Código do slide para auxiliar:

Mochila PD(n,p,c,v)
    para b <- 0 até c faça
        X[0,b] <- 0
        para j <- 1 até n faça
            x <- X[j - 1, b]
            se b - pj >= 0
               então y <- X[j - 1, b - pj] + vj
                    se x < y então x <- y
            X[j,b] <- x
    devolva X[n,c]
*/

/*
OBSERVAÇÕES:
-> Aplicação:
Como dito na aula, esse código pode ser empregado em problemas de otimização envolvendo montagem de equipamentos com múltiplas peças

-> Programação Dinâmica:
Resolver de forma eficiente, armazenando subsoluções de problemas menores e reutilizando para construir a solução do problema maior

Está implementada na função mochila. Representada pela matriz tabela, que armazena as soluções de subproblemas menores para evitar cálculos maiores
*/
#include <stdio.h>

#define MAX 100 // Ajuste para o tamanho máximo necessário

/*
Função para verificar se um objeto tem ao menos um vizinho incluído
Parâmetros:
- objeto: objeto atual
- capacidade_atual: capacidade atual da mochila
- tabela: tabela de programação dinâmica
- grafo: matriz de dependência entre os objetos
- n: número de objetos

-> Se os valores nas duas posições forem iguais, indica que a inclusão não alterou o valor máximo da mochila, ou seja, o vizinho não foi incluído
*/
int tem_vizinho_incluido(int objeto, int capacidade_atual, int tabela[MAX][MAX], int grafo[MAX][MAX], int n) {
    for (int k = 0; k < n; k++) { // Percorre os objetos
        if (grafo[objeto][k]) { // Verifica se existe uma aresta entre o objeto atual e o objeto k na matriz
            if (tabela[k + 1][capacidade_atual] == tabela[k][capacidade_atual]) { // Verifica se o vizinho está incluído
                return 0; // Não há vizinho incluído
            }
        }
    }
    return 1; // Há ao menos um vizinho incluído
}

/*
Função recursiva para determinar os objetos incluídos (como pedido no quadro)
Parâmetros:
- i: índice do objeto atual
- j: capacidade restante
- pesos: vetor com os pesos dos objetos
- tabela: tabela de programação dinâmica
- incluso: vetor que armazena os objetos incluídos
*/
void determinar_objetos_inclusos(int i, int j, int pesos[], int valores[], int tabela[MAX][MAX], int incluso[], int n) {
    // Caso base: se não houver mais objetos ou capacidade, ele irá imprimir os objetos incluídos
    if (i == 0 || j == 0) {
        // Imprime os objetos incluídos
        printf("Objetos incluídos na mochila:\n");
        for (int k = 1; k <= n; k++) { // Percorre todos os objetos
            if (incluso[k]) { // Verifica se foi incluso
                printf("Objeto %d (Peso: %d, Valor: %d)\n", k + 1, pesos[k], valores[k]); // Printa o objeto com seu peso e valor
            }
        }
        return;
    }

    // Verifica se o objeto foi incluído, comparando com o objeto anterior
    if (tabela[i][j] != tabela[i - 1][j]) {
        incluso[i] = 1; // Marca o objeto como incluído
        determinar_objetos_inclusos(i - 1, j - pesos[i - 1], pesos, valores, tabela, incluso, n); // Recalcula para o próximo
    } else {
        // Caso contrário, continua sem incluir o objeto atual e chama para o próximo
        determinar_objetos_inclusos(i - 1, j, pesos, valores, tabela, incluso, n);
    }
}


/*
Função para resolver o problema da mochila com dependência
Parâmetros:
- pesos: vetor de inteiros com os pesos dos objetos
- valores: vetor de inteiros com os valores dos objetos
- grafo: matriz de dependência entre os objetos
- n: número de objetos
- capacidade: capacidade da mochila
*/
int mochila(int pesos[], int valores[], int grafo[MAX][MAX], int n, int capacidade) {
    // Inicialização da tabela de programação dinâmica e do vetor de objetos inclusos
    int tabela[MAX][MAX] = {0};
    int incluso[MAX] = {0};

    // Preenche a tabela de programação dinâmica
    for (int i = 0; i < n; i++) { // Percorre os objetos
        for (int j = 0; j <= capacidade; j++) {  // Percorre as capacidades
            tabela[i + 1][j] = tabela[i][j]; // Copiar o valor da linha anterior, assumindo que o objeto i não foi incluido
            if (pesos[i] <= j) { // Verificar se o peso é igual ou menor que a capacidade
                if (tem_vizinho_incluido(i, j - pesos[i], tabela, grafo, n)) { // Verifica se tem ao menos um vizinho incluído
                    int valor_com_objeto = tabela[i][j - pesos[i]] + valores[i]; // Calcula o valor com o objeto
                    if (valor_com_objeto > tabela[i + 1][j]) { // Verifica se o valor é maior
                        tabela[i + 1][j] = valor_com_objeto; // Atualiza o valor na tabela
                    }
                }
            }
        }
    }

    // Printar a tabela semelhante a feita no quadro e na atividade
    printf("Tabela (semelhante à atividade):\n");
    for (int i = 0; i <= n; i++) { 
        for (int j = 0; j <= capacidade; j++) {
            printf("%3d ", tabela[i][j]);
        }
        printf("\n");
    }

    // Chamada da função recursiva para determinar os objetos incluídos
    printf("---------------------------------------------------------------------\n");
    determinar_objetos_inclusos(n, capacidade, pesos, valores, tabela, incluso, n);

    return tabela[n][capacidade];
}

int main() {
    printf("-----------------PROBLEMA DA MOCHILA COM DEPENDÊNCIA-----------------\n");
    printf("---------------------------------------------------------------------\n");

    printf("Exemplo da aula:\n");

    int capacidade_aula = 5;
    int pesos_aula[] = {1, 3, 2, 4};
    int valores_aula[] = {8, 7, 5, 9};
    int grafo_aula[MAX][MAX] = {
        {0, 0, 0, 0},// Não há dependência
        {1, 0, 0, 0},// Objeto 2 depende do objeto 1
        {1, 0, 0, 0},// Objeto 3 depende do objeto 1
        {0, 0, 1, 0} // Objeto 4 depende do objeto 3
    };

    int valor_maximo_aula = mochila(pesos_aula, valores_aula, grafo_aula, 4, capacidade_aula);
    printf("\nValor máximo da mochila: %d\n", valor_maximo_aula);

    int capacidade, n;
    int pesos[MAX], valores[MAX];
    int grafo[MAX][MAX] = {0};

    printf("---------------------------------------------------------------------\n");

    printf("Exemplo com o usuário (entrada de dados): \n");

    printf("Digite a capacidade da mochila: ");
    scanf("%d", &capacidade);
    printf("Digite o número de objetos: ");
    scanf("%d", &n);

    printf("---------------------------------------------------------------------\n");

    printf("Digite o peso e o valor de cada objeto:\n");
    for (int i = 0; i < n; i++) {
        printf("Objeto %d - Peso e Valor: ", i + 1);
        scanf("%d %d", &pesos[i], &valores[i]);
    }

    printf("---------------------------------------------------------------------\n");

    printf("Digite a matriz de dependência (%dx%d):\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &grafo[i][j]);
        }
    }

    int valor_maximo = mochila(pesos, valores, grafo, n, capacidade);
    printf("---------------------------------------------------------------------\n");
    printf("Valor máximo da mochila: %d\n", valor_maximo);

    return 0;
}

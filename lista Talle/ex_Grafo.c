#include <stdio.h>
#include <stdlib.h>

void exibirMatriz(int **matriz, int n_vertices) {
    printf("\nMatriz de Adjacencia:\n");
    printf("  ");
    for (int i = 0; i < n_vertices; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (int i = 0; i < n_vertices; i++) {
        printf("%d ", i);
        for (int j = 0; j < n_vertices; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int n_vertices;
    int **matriz_adjacencia;
    int i, j;
    char continuar_conexoes;

    printf("Digite a quantidade de vertices: ");
    scanf("%d", &n_vertices);

    matriz_adjacencia = (int **)malloc(n_vertices * sizeof(int *));
    if (matriz_adjacencia == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }
    for (int k = 0; k < n_vertices; k++) {
        matriz_adjacencia[k] = (int *)calloc(n_vertices, sizeof(int));
        if (matriz_adjacencia[k] == NULL) {
            printf("Erro ao alocar memoria.\n");
            for (int l = 0; l < k; l++) {
                free(matriz_adjacencia[l]);
            }
            free(matriz_adjacencia);
            return 1;
        }
    }

    do {
        printf("\nDigite a conexao (vertice de origem e vertice de destino):\n");
        printf("Vertice de origem (0 a %d): ", n_vertices - 1);
        scanf("%d", &i);
        printf("Vertice de destino (0 a %d): ", n_vertices - 1);
        scanf("%d", &j);

        if (i >= 0 && i < n_vertices && j >= 0 && j < n_vertices) {
            matriz_adjacencia[i][j] = 1;
            matriz_adjacencia[j][i] = 1;
        } else {
            printf("Vertices invalidos. Por favor, tente novamente.\n");
        }

        printf("\nDeseja adicionar mais conexoes? (s/n): ");
        scanf(" %c", &continuar_conexoes);

    } while (continuar_conexoes == 's' || continuar_conexoes == 'S');

    exibirMatriz(matriz_adjacencia, n_vertices);

    for (int k = 0; k < n_vertices; k++) {
        free(matriz_adjacencia[k]);
    }
    free(matriz_adjacencia);

    return 0;
}
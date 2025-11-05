#include <stdio.h>
#include <stdlib.h>

int main() {
    int numLinha, numColuna;

    printf("Informe a quantidade de linhas da matriz: ");
    scanf("%d", &numLinha);
    printf("Informe o número de colunas da matriz: ");
    scanf("%d", &numColuna);

    int **matriz = (int **) malloc(numLinha * sizeof(int *));
    if (matriz == 0) { 
        printf("Falha ao alocar matriz\n");
        return 1;
    }

    for (int i = 0; i < numLinha; i++) {
        matriz[i] = (int *) malloc(numColuna * sizeof(int));
        if (matriz[i] == 0) {
            printf("Falha ao alocar linha %d\n", i+1);
            return 1;
        }
    }

    printf("\nDigite os elementos da matriz:\n");
    for (int i = 0; i < numLinha; i++) {
        for (int j = 0; j < numColuna; j++) {
            printf("Elemento [%d][%d]: ", i+1, j+1);
            scanf("%d", &matriz[i][j]);
        }
    }

    int maior = matriz[0][0];
    for (int i = 0; i < numLinha; i++) {
        for (int j = 0; j < numColuna; j++) {
            if (matriz[i][j] > maior) {
                maior = matriz[i][j];
            }
        }
    }

    printf("\nO maior elemento da matriz é %d\n", maior);

    for (int i = 0; i < numLinha; i++) {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}
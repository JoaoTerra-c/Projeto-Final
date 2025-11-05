#include <stdio.h>
#include <stdlib.h>

int main() {
    int **matriz;
    int **matrizTrans;

    // Alocação da matriz 3x2
    matriz = (int**) malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++) {
        matriz[i] = (int*) malloc(2 * sizeof(int));
    }

    // Alocação da matriz transposta 2x3
    matrizTrans = (int**) malloc(2 * sizeof(int*));
    for (int i = 0; i < 2; i++) {
        matrizTrans[i] = (int*) malloc(3 * sizeof(int));
    }

    // Entrada de dados
    printf("\nDigite os elementos da matriz (3x2):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            printf("Elemento [%d][%d]: ", i+1, j+1);
            scanf("%d", &matriz[i][j]);
        }
    }

    // Construção da transposta
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            matrizTrans[j][i] = matriz[i][j];
        }
    }

    // Impressão da matriz transposta
    printf("\nMatriz Transposta (2x3):\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%4d ", matrizTrans[i][j]);
        }
        printf("\n");
    }

    // Liberação da memória
    for (int i = 0; i < 3; i++) free(matriz[i]);
    free(matriz);
    for (int i = 0; i < 2; i++) free(matrizTrans[i]);
    free(matrizTrans);

    return 0;
}

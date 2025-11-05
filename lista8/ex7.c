#include <stdio.h>

void transporMatriz(int matriz[5][5]) {
    int i, j, temp;

    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 5; j++) {
            temp = matriz[i][j];
            matriz[i][j] = matriz[j][i];
            matriz[j][i] = temp;
        }
    }
}

void imprimirMatriz(int matriz[5][5]) {
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            printf("%3d ", matriz[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int A[5][5] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };

    printf("Matriz original:\n");
    imprimirMatriz(A);

    transporMatriz(A);

    printf("\nMatriz transposta (in-place):\n");
    imprimirMatriz(A);

    return 0;
}
#include <stdio.h>

int somarMatriz(int matriz[10][10]) {
    int soma = 0;
    int i, j;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            soma += matriz[i][j];
        }
    }

    return soma;
}

int main() {
    int minhaMatriz[10][10];
    int i, j, valor = 1;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            minhaMatriz[i][j] = valor++;
        }
    }

    int resultado = somarMatriz(minhaMatriz);

    printf("A soma de todos os elementos da matriz eh: %d\n", resultado);

    return 0;
}
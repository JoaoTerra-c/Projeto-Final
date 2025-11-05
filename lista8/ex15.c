#include <stdio.h>

void encontrarMaiorMenor(const int vetor[], int n, int *maior, int *menor) {
    int i;
    *maior = vetor[0];
    *menor = vetor[0];

    for (i = 1; i < n; i++) {
        if (vetor[i] > *maior) {
            *maior = vetor[i];
        }
        if (vetor[i] < *menor) {
            *menor = vetor[i];
        }
    }
}

int main() {
    int meuVetor[] = {15, 7, 2, 28, 9, 12, 1, 30};
    int tamanho = 8;
    int maior_elemento, menor_elemento;

    encontrarMaiorMenor(meuVetor, tamanho, &maior_elemento, &menor_elemento);

    printf("O maior elemento do vetor eh: %d\n", maior_elemento);
    printf("O menor elemento do vetor eh: %d\n", menor_elemento);

    return 0;
}
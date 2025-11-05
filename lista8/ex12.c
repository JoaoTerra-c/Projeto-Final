#include <stdio.h>

void somarParesEImpares(const int vetor[], int n, int *somaPares, int *somaImpares) {
    int i;
    *somaPares = 0;
    *somaImpares = 0;

    for (i = 0; i < n; i++) {
        if (vetor[i] % 2 == 0) {
            *somaPares += vetor[i];
        } else {
            *somaImpares += vetor[i];
        }
    }
}

int main() {
    int meuVetor[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int tamanho = 10;
    int soma_pares, soma_impares;

    somarParesEImpares(meuVetor, tamanho, &soma_pares, &soma_impares);

    printf("Soma dos numeros pares: %d\n", soma_pares);
    printf("Soma dos numeros impares: %d\n", soma_impares);

    return 0;
}
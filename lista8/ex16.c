#include <stdio.h>

void encontrarMaiorEContagem(const int vetor[], int n, int *maior, int *contagem) {
    int i;
    *maior = vetor[0];
    *contagem = 0;

    for (i = 1; i < n; i++) {
        if (vetor[i] > *maior) {
            *maior = vetor[i];
        }
    }

    for (i = 0; i < n; i++) {
        if (vetor[i] == *maior) {
            *contagem = *contagem + 1;
        }
    }
}

int main() {
    int meuVetor[] = {10, 2, 5, 10, 8, 10, 3, 7};
    int tamanho = 8;
    int maior_elemento, contagem_maior;

    encontrarMaiorEContagem(meuVetor, tamanho, &maior_elemento, &contagem_maior);

    printf("O maior elemento do vetor e: %d\n", maior_elemento);
    printf("Ocorreu %d vez(es) no vetor.\n", contagem_maior);

    return 0;
}
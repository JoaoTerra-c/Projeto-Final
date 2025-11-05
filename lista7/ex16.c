#include <stdio.h>

// Função que imprime o triângulo lateral
void imprimirTriangulo(int n) {
    // Parte crescente
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            printf("*");
        }
        printf("\n");
    }

    // Parte decrescente
    for (int i = n - 1; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            printf("*");
        }
        printf("\n");
    }
}

int main() {
    int n;

    printf("Digite um número inteiro positivo: ");
    scanf("%d", &n);

    if (n > 0) {
        imprimirTriangulo(n);
    } else {
        printf("Número inválido! Digite um inteiro positivo.\n");
    }

    return 0;
}

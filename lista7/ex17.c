#include <stdio.h>

// Função que calcula a série S
double calcularSerie(int N) {
    double S = 0.0;

    for (int i = 1; i <= N; i++) {
        double numerador = i * i + 1;   // i^2 + 1
        double denominador = i + 3;     // i + 3
        S += numerador / denominador;
    }

    return S;
}

int main() {
    int N;

    printf("Digite um número inteiro positivo N: ");
    scanf("%d", &N);

    if (N > 0) {
        double resultado = calcularSerie(N);
        printf("O resultado da série S é: %.4f\n", resultado);
    } else {
        printf("Número inválido! Digite um inteiro positivo.\n");
    }

    return 0;
}

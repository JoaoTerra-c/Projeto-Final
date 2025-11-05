#include <stdio.h>

// Função que calcula o somatório de 1 até n
int somatorio(int n) {
    int soma = 0;
    for (int i = 1; i <= n; i++) {
        soma += i;
    }
    return soma;
}

int main() {
    int numero;

    printf("Digite um número inteiro positivo: ");
    scanf("%d", &numero);

    if (numero > 0) {
        int resultado = somatorio(numero);
        printf("O somatório de 1 até %d é: %d\n", numero, resultado);
    } else {
        printf("Número inválido! Digite um inteiro positivo.\n");
    }

    return 0;
}

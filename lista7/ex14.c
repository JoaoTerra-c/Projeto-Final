#include <stdio.h>

// Função que calcula o fatorial de n
long long fatorial(int n) {
    if (n < 0) {
        printf("Número negativo não possui fatorial.\n");
        return -1; // valor de erro
    }

    long long resultado = 1;
    for (int i = 1; i <= n; i++) {
        resultado *= i;
    }
    return resultado;
}

int main() {
    int numero;

    printf("Digite um número inteiro positivo: ");
    scanf("%d", &numero);

    long long resultado = fatorial(numero);

    if (resultado != -1) {
        printf("%d! = %lld\n", numero, resultado);
    }

    return 0;
}

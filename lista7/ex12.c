#include <stdio.h>

// Função que retorna o maior fator primo de um número
int maiorFatorPrimo(int n) {
    int maior = -1;

    // Divide n por 2 até não ser mais divisível
    while (n % 2 == 0) {
        maior = 2;
        n /= 2;
    }

    // Divide n por números ímpares a partir de 3
    for (int i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            maior = i;
            n /= i;
        }
    }

    // Se sobrar algum número maior que 2, ele também é primo
    if (n > 2)
        maior = n;

    return maior;
}

int main() {
    int numero;

    printf("Digite um número inteiro positivo: ");
    scanf("%d", &numero);

    int resultado = maiorFatorPrimo(numero);

    printf("O maior fator primo de %d é: %d\n", numero, resultado);

    return 0;
}

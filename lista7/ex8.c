#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Função que verifica se um número é um quadrado perfeito
bool ehQuadradoPerfeito(int num) {
    if (num < 0) {
        return false; // números negativos não podem ser quadrados perfeitos
    }
    int raiz = (int)sqrt(num); // calcula a raiz quadrada truncada
    return (raiz * raiz == num); // verifica se o quadrado da raiz é igual ao número
}

int main() {
    int n;

    printf("Digite um número inteiro: ");
    scanf("%d", &n);

    if (ehQuadradoPerfeito(n)) {
        printf("%d é um quadrado perfeito.\n", n);
    } else {
        printf("%d não é um quadrado perfeito.\n", n);
    }

    return 0;
}

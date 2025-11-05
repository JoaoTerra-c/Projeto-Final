#include <stdio.h>

// Função que calcula x^y
int potencia(int x, int y) {
    if (y < 0) {
        printf("Expoente negativo não é suportado nesta função.\n");
        return 0; // ou poderia tratar como erro
    }

    int resultado = 1;
    for (int i = 0; i < y; i++) {
        resultado *= x; // multiplica x por ele mesmo y vezes
    }

    return resultado;
}

int main() {
    int base, expoente;

    printf("Digite a base e o expoente (inteiros): ");
    scanf("%d %d", &base, &expoente);

    int resultado = potencia(base, expoente);

    printf("%d elevado a %d é: %d\n", base, expoente, resultado);

    return 0;
}

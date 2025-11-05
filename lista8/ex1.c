#include <stdio.h>

void separarPartes(double numero, int *inteira, double *fracionaria) {
    *inteira = (int)numero;
    *fracionaria = numero - *inteira;
}

int main() {
    double numero_real;
    int parte_inteira;
    double parte_fracionaria;

    printf("Digite um numero real: ");
    scanf("%lf", &numero_real);

    separarPartes(numero_real, &parte_inteira, &parte_fracionaria);

    printf("Parte inteira: %d\n", parte_inteira);
    printf("Parte fracionaria: %f\n", parte_fracionaria);

    return 0;
}
#include <stdio.h>
#define PI 3.14159265358979323846

double calcularCosseno(double graus) {
    double radianos = graus * PI / 180.0;
    double cosseno = 1.0;
    double termo = 1.0;
    int i = 1;

    do {
        termo = termo * (-1) * radianos * radianos / ((2.0 * i) * (2.0 * i - 1.0));
        cosseno += termo;
        i++;
    } while (termo > 1e-9 || termo < -1e-9);

    return cosseno;
}

int main() {
    double angulo_graus;
    double cosseno_calculado;

    printf("Digite o valor do angulo em graus: ");
    scanf("%lf", &angulo_graus);

    cosseno_calculado = calcularCosseno(angulo_graus);

    printf("O valor do cosseno de %.2f graus e: %.6f\n", angulo_graus, cosseno_calculado);

    return 0;
}
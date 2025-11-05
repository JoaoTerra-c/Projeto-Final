#include <stdio.h>
#define PI 3.14159265358979323846

double calcularSeno(double graus) {
    double radianos = graus * PI / 180.0;
    double seno = radianos;
    double termo = radianos;
    int i = 1;

    do {
        termo = termo * (-1) * radianos * radianos / ((2.0 * i) * (2.0 * i + 1.0));
        seno += termo;
        i++;
    } while (termo > 1e-9 || termo < -1e-9);

    return seno;
}

int main() {
    double angulo_graus;
    double seno_calculado;

    printf("Digite o valor do angulo em graus: ");
    scanf("%lf", &angulo_graus);

    seno_calculado = calcularSeno(angulo_graus);

    printf("O valor do seno de %.2f graus e: %.6f\n", angulo_graus, seno_calculado);

    return 0;
}
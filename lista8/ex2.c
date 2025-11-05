#include <stdio.h>
#define PI 3.14159265358979323846

void calcularEsfera(double r, double *volume, double *area) {
    *volume = (4.0 / 3.0) * PI * r * r * r;
    *area = 4.0 * PI * r * r;
}

int main() {
    double raio;
    double vol;
    double ar;

    printf("Digite o valor do raio da esfera: ");
    scanf("%lf", &raio);

    calcularEsfera(raio, &vol, &ar);

    printf("Volume da esfera: %f\n", vol);
    printf("Area da esfera: %f\n", ar);

    return 0;
}
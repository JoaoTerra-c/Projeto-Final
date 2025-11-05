#include <stdio.h>
#include <math.h>

#define PI 3.141592

float calculo(float raio) {
    return (4.0/3.0) * PI * pow(raio, 3);
}

int main() {
    float raio, volume;

    printf("Informe o valor do raio da esfera: ");
    scanf("%f", &raio);

    volume = calculo(raio);

    printf("O volume dessa esfera será %.2f mL\n", volume);

    return 0;
}
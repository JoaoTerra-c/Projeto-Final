#include <stdio.h>
#include <math.h>

double calcularDesvioPadrao(const double V[], int n) {
    int i;
    double soma = 0.0;
    double media;
    double soma_quadrados = 0.0;
    double variancia;
    double desvio_padrao;

    for (i = 0; i < n; i++) {
        soma += V[i];
    }
    media = soma / n;

    for (i = 0; i < n; i++) {
        soma_quadrados += pow(V[i] - media, 2);
    }
    variancia = soma_quadrados / n;
    desvio_padrao = sqrt(variancia);

    return desvio_padrao;
}

int main() {
    double vetor_exemplo[] = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    int n = sizeof(vetor_exemplo) / sizeof(vetor_exemplo[0]);
    double resultado;

    resultado = calcularDesvioPadrao(vetor_exemplo, n);

    printf("O desvio-padrao do vetor eh: %.4f\n", resultado);

    return 0;
}
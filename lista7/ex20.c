#include <stdio.h>

double calcularEuler() {
    double e = 1.0;
    double termo = 1.0;
    int n = 1;

    do {
        termo = termo / n;
        e = e + termo;
        n++;
    } while (termo > 1e-9);

    return e;
}

int main() {
    double euler_calculado;

    euler_calculado = calcularEuler();

    printf("O valor de e eh: %.10f\n", euler_calculado);

    return 0;
}
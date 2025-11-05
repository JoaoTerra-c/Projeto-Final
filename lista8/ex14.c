#include <stdio.h>
#include <math.h>

void resolverEquacaoSegundoGrau(double a, double b, double c) {
    if (a == 0) {
        printf("Nao e uma equacao do segundo grau.\n");
        return;
    }

    double delta = b * b - 4 * a * c;

    if (delta > 0) {
        double x1 = (-b + sqrt(delta)) / (2 * a);
        double x2 = (-b - sqrt(delta)) / (2 * a);
        printf("Duas raizes reais e distintas:\n");
        printf("x1 = %f\n", x1);
        printf("x2 = %f\n", x2);
    } else if (delta == 0) {
        double x = -b / (2 * a);
        printf("Duas raizes reais e iguais:\n");
        printf("x = %f\n", x);
    } else {
        double real_part = -b / (2 * a);
        double imaginary_part = sqrt(-delta) / (2 * a);
        printf("Duas raizes complexas e conjugadas:\n");
        printf("x1 = %f + %fi\n", real_part, imaginary_part);
        printf("x2 = %f - %fi\n", real_part, imaginary_part);
    }
}

int main() {
    double a, b, c;

    printf("Digite os coeficientes da equacao do segundo grau (a, b, c): \n");
    printf("a: ");
    scanf("%lf", &a);
    printf("b: ");
    scanf("%lf", &b);
    printf("c: ");
    scanf("%lf", &c);

    resolverEquacaoSegundoGrau(a, b, c);

    return 0;
}
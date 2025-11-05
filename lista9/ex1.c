#include <stdio.h>

int SomaCubo(int n) {
    int Soma = 0;

    for (int i = 1; i <= n; i++) {
        Soma = Soma + (i * i * i); // ou pow(i,3) se quiser usar <math.h>
    }

    return Soma;
}

int main() {
    int n;
    int result;

    printf("Informe um número: \n");
    scanf("%d", &n);

    result = SomaCubo(n);

    printf("A soma de todos os cubos de 1 até %d será %d\n", n, result);

    return 0;
}
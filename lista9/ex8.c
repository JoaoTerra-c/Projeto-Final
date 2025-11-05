#include <stdio.h>

int Multiplicacao(int x, int y) {
    if (y == 0) {
        return 0; // base da recursão
    } else {
        return x + Multiplicacao(x, y - 1); // soma repetida
    }
}

int main() {
    int x, y;
    int result;

    printf("Informe um número: ");
    scanf("%d", &x);

    printf("Esse número será multiplicado por qual? ");
    scanf("%d", &y);

    result = Multiplicacao(x, y);

    printf("O resultado dessa multiplicação será: %d\n", result);

    return 0;
}
#include <stdio.h>

int MDC(int x, int y) {
    int resto;
    while (y != 0) {
        resto = x % y;
        x = y;
        y = resto;
    }
    return x;
}

int main() {
    int x, y;
    int result;
    int temp;

    printf("Informe um número: \n");
    scanf("%d", &x);

    printf("Informe outro número: \n");
    scanf("%d", &y);

    if (y < x) {
        temp = x;
        x = y;
        y = temp;
    }

    result = MDC(x, y);

    printf("O máximo divisor comum entre esses dois números é: %d \n", result);

    return 0;
}

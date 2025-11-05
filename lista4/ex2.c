#include <stdio.h>

int main() {
    int a, b;
    int *p1 = &a, *p2 = &b;

    printf("Informe o valor da primeira variável: ");
    scanf("%d", p1);

    printf("Informe o valor da segunda variável: ");
    scanf("%d", p2);

    if (*p1 >= *p2) {
        printf("O maior valor é %d\n",a);
    } else {
        printf("O maior valor é %d\n",b);
    }

    return 0;
}
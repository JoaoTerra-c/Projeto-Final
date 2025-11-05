#include <stdio.h>

int main() {
    int a, b;
    int *p1 = &a, *p2 = &b;

    printf("Informe o valor da primeira variável: ");
    scanf("%d", p1);

    printf("Informe o valor da segunda variável: ");
    scanf("%d", p2);

    if (*p1 >= *p2) {
        printf("A posição do ponteiro 1 é maior\n");
    } else {
        printf("A posição do ponteiro 2 é maior\n");
    }

    return 0;
}
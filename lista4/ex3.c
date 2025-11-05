#include <stdio.h>

int main() {
    float v[10];
    float *p = v;

    for (int i = 0; i < 10; i++) {
        printf("Informe o valor da posição %d: ", i);
        scanf("%d", p + i); // (p+i) aponta para v[i]
    }

    printf("\nValores e endereços:\n");
    for (int i = 0; i < 10; i++) {
        printf("*(p+%d) = %d\t Endereço = %p\n", i, *(p+i), (void*)(p+i));
    }

    return 0;
}
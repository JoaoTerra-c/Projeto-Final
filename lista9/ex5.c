#include <stdio.h>
#include <stdlib.h>

int Somatorio(int *p, int n) {
    int soma = 0;
    for (int i = 0; i < n; i++) {
        soma += p[i];
    }
    return soma;
}

int main() {
    int n;
    int result;
    int *p;

    printf("Informe o tamanho do arranjo: ");
    scanf("%d", &n);

    p = (int *)malloc(n * sizeof(int));
    if (p == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    printf("Informe os elementos do arranjo:\n");
    for (int i = 0; i < n; i++) {
        printf("Elemento %d: ", i + 1);
        scanf("%d", &p[i]);
    }

    result = Somatorio(p, n);
    printf("A soma dos elementos desse arranjo será: %d\n", result);

    free(p); 

    return 0;
}
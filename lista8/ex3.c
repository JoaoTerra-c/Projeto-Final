#include <stdio.h>

int somarArray(int arr[], int tamanho) {
    int soma = 0;
    int i;
    for (i = 0; i < tamanho; i++) {
        soma += arr[i];
    }
    return soma;
}

int main() {
    int meuArray[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int resultado;
    resultado = somarArray(meuArray, 10);
    printf("A soma dos elementos do array eh: %d\n", resultado);
    return 0;
}
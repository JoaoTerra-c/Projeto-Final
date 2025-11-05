#include <stdio.h>

void ordenarVetor(int vetor[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (vetor[j] > vetor[j+1]) {
                temp = vetor[j];
                vetor[j] = vetor[j+1];
                vetor[j+1] = temp;
            }
        }
    }
}

int main() {
    int vetor_exemplo[] = {5, 2, 9, 1, 5, 6};
    int n = 6;
    int i;
    
    printf("Vetor original:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", vetor_exemplo[i]);
    }
    printf("\n");
    
    ordenarVetor(vetor_exemplo, n);
    
    printf("Vetor ordenado:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", vetor_exemplo[i]);
    }
    printf("\n");
    
    return 0;
}
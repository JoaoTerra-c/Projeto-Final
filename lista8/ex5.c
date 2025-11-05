#include <stdio.h>

double calcularMedia(float notas[], int tamanho) {
    float soma = 0.0;
    int i;

    for (i = 0; i < tamanho; i++) {
        soma += notas[i];
    }

    return soma / tamanho;
}

int main() {
    float notas_alunos[10] = {7.5, 8.0, 6.5, 9.0, 7.0, 5.5, 8.5, 6.0, 9.5, 8.0};
    double media;

    media = calcularMedia(notas_alunos, 10);

    printf("A media dos alunos e: %.2f\n", media);

    return 0;
}
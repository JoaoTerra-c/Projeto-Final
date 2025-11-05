#include <stdio.h>

// Função que calcula a média do aluno
float calcularMedia(float n1, float n2, float n3, char tipo) {
    if (tipo == 'A' || tipo == 'a') {  // Média aritmética
        return (n1 + n2 + n3) / 3.0;
    } else if (tipo == 'P' || tipo == 'p') {  // Média ponderada com pesos 5, 3 e 2
        return (n1 * 5 + n2 * 3 + n3 * 2) / 10.0;
    } else {
        printf("Tipo de média inválido! Use 'A' ou 'P'.\n");
        return -1; // valor de erro
    }
}

int main() {
    float nota1, nota2, nota3;
    char tipoMedia;

    printf("Digite as três notas do aluno (separadas por espaço): ");
    scanf("%f %f %f", &nota1, &nota2, &nota3);

    printf("Digite o tipo de média ('A' para aritmética, 'P' para ponderada): ");
    scanf(" %c", &tipoMedia); // espaço antes do %c para ignorar '\n'

    float media = calcularMedia(nota1, nota2, nota3, tipoMedia);

    if (media != -1) {
        printf("A média calculada é: %.2f\n", media);
    }

    return 0;
}

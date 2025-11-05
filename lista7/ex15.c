#include <stdio.h>

// Função que imprime n linhas com pontos de exclamação
void imprimirExclamacoes(int n) {
    for (int i = 1; i <= n; i++) {       // Para cada linha
        for (int j = 1; j <= i; j++) {   // Imprime i pontos de exclamação
            printf("!");
        }
        printf("\n");                     // Pula para a próxima linha
    }
}

int main() {
    int n;

    printf("Digite um número inteiro positivo: ");
    scanf("%d", &n);

    if (n > 0) {
        imprimirExclamacoes(n);
    } else {
        printf("Número inválido! Digite um inteiro positivo.\n");
    }

    return 0;
}

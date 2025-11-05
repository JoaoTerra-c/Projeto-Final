#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 20
#define VAZIO "VAZIO"

int main() {
    int quantidade;
    
    printf("Informe a quantidade de nomes a serem alocados: ");
    scanf("%d", &quantidade);
    char** nomes = (char**)malloc(quantidade * sizeof(char*));
    if (nomes == NULL) {
        printf("Erro ao alocar memória para o vetor de nomes.\n");
        return 1;
    }
    for (int i = 0; i < quantidade; i++) {
        nomes[i] = (char*)malloc((TAM_NOME + 1) * sizeof(char));
        if (nomes[i] == NULL) {
            printf("Erro ao alocar memória para o nome %d.\n", i);
            for (int j = 0; j < i; j++) {
                free(nomes[j]);
            }
            free(nomes);
            return 1;
        }
        strcpy(nomes[i], VAZIO);
    }
    printf("\n--- Vetor de Nomes ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("Nome %d: %s\n", i, nomes[i]);
    }
    printf("----------------------\n");
    printf("\nModificando o primeiro nome...\n");
    strcpy(nomes[0], "Joao Silva");
    printf("\n--- Vetor de Nomes Atualizado ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("Nome %d: %s\n", i, nomes[i]);
    }
    printf("------------------------------\n");
    for (int i = 0; i < quantidade; i++) {
        free(nomes[i]);
    }
    free(nomes);

    return 0;
}
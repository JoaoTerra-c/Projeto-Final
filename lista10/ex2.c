#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
void LerArquivo() {
    char nome[50];
    float nota;
    FILE *arq;
    arq = fopen("Alunos.txt", "r");

    if (arq == NULL) {
        printf("\nErro ao abrir o arquivo 'Alunos.txt' para leitura. Ele pode nao existir ou estar vazio.\n");
        return 1;
    } else {
        printf("\n--- Conteudo do Arquivo Alunos.txt ---\n");
        while (fgets(nome, sizeof(nome), arq) != NULL) {
            nome[strcspn(nome, "\n")] = 0;
            if (fscanf(arq, "%f\n", &nota) == 1) {
                printf("Aluno: %s\n", nome);
                printf("Nota: %.2f\n", nota);
            } else {
                printf("Aluno: %s (Nota nao encontrada ou invalida)\n", nome);
            }
        }
        printf("----------------------------------------\n");
        fclose(arq);
    }
}

int main() {
    char nome[50];
    float nota;
    FILE *arq;
    arq = fopen("Alunos.txt", "w+");

    if (arq == NULL) {
        printf("Erro ao abrir arquivo 'Alunos.txt' para escrita.\n");
        return 1;
    } else {
        for (int i = 0; i < 5; i++) {
            printf("\nInforme o nome do %d° aluno: ", i + 1);
            if (scanf("%49s", nome) != 1) break; 
            printf("Informe a nota do %d° aluno: ", i + 1);
            if (scanf("%f", &nota) != 1) break;
            fprintf(arq, "%s\n", nome);
            fprintf(arq, "%.2f\n", nota);
        }

        fclose(arq);
        printf("\nDados dos 5 alunos salvos em 'Alunos.txt'.\n");
    }

    LerArquivo();

    return 0; 
}
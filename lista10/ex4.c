#include <stdio.h>
#include <stdlib.h>

void copiarArquivo(char origem[], char destino[]) {
    FILE *arq_origem;
    FILE *arq_destino;
    int caractere;

    arq_origem = fopen(origem, "r");
    if (arq_origem == NULL) {
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo de origem '%s'.\n", origem);
        return;
    }

    arq_destino = fopen(destino, "w");
    if (arq_destino == NULL) {
        fprintf(stderr, "Erro: Nao foi possivel abrir/criar o arquivo de destino '%s'.\n", destino);
        fclose(arq_origem);
        return;
    }

    while ((caractere = fgetc(arq_origem)) != EOF) {
        fputc(caractere, arq_destino);
    }

    fclose(arq_origem);
    fclose(arq_destino);

    printf("Sucesso! Conteudo de '%s' copiado para '%s'.\n", origem, destino);
}

int main() {
    FILE *teste = fopen("origem.txt", "w");
    if (teste == NULL) {
        perror("Erro ao criar arquivo de teste");
        return 1;
    }
    fprintf(teste, "Este e o conteudo original.\nA copia deve ter duas linhas.");
    fclose(teste);
    printf("Arquivo 'origem.txt' criado com sucesso.\n\n");

    char nome_origem[] = "origem.txt";
    char nome_destino[] = "destino_copiado.txt";
    
    copiarArquivo(nome_origem, nome_destino);

    FILE *check = fopen(nome_destino, "r");
    if (check != NULL) {
        printf("\n--- Conteudo do Arquivo Destino ---\n");
        int c;
        while ((c = fgetc(check)) != EOF) {
            putchar(c);
        }
        printf("\n-----------------------------------\n");
        fclose(check);
    }

    return 0;
}
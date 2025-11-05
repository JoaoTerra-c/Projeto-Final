#include <stdio.h>
#include <stdlib.h>

#define NOME_ARQUIVO "contador.txt"

void ContarExecucao() {
    FILE *arq;
    int contador = 0;

    arq = fopen(NOME_ARQUIVO, "r+");

    if (arq == NULL) {
        printf("Criando arquivo de contador...\n");
        
        arq = fopen(NOME_ARQUIVO, "w+");
        
        if (arq == NULL) {
            perror("Erro ao criar o arquivo de contador");
            return;
        }
        
        contador = 1;
        
    } else {
        
        if (fscanf(arq, "%d", &contador) != 1) {
            fprintf(stderr, "Aviso: Nao foi possivel ler o contador. Reiniciando de 1.\n");
            contador = 0;
        }

        contador++;
        
        rewind(arq);
    }
    
    fprintf(arq, "%d", contador);
    
    fclose(arq); 

    printf("\n>>> Este programa foi executado %d vez(es)! <<<\n\n", contador);
}

int main() {
    ContarExecucao();

    return 0;
}
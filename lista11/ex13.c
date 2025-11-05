#include <stdio.h>
#include <string.h>

#define TAM_MAX 100

int main() {
    char s[TAM_MAX];
    int i, len;
    int inicio, fim;
    
    printf("13) Imprimir Segmento da String\n");
    printf("Digite uma string: ");
    
    // Limpa o buffer antes de usar fgets
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 

    if (fgets(s, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s[strcspn(s, "\n")] = 0;
    len = strlen(s);

    printf("Comprimento da string: %d\n", len);
    printf("Digite o indice de inicio (i): ");
    scanf("%d", &inicio);
    printf("Digite o indice final (j): ");
    scanf("%d", &fim);
    
    printf("Segmento: ");
    
    // Validação de índices
    if (inicio < 0 || fim >= len || inicio > fim) {
        printf("Indices invalidos (devem ser 0 <= i <= j < %d).\n", len);
    } else {
        for (i = inicio; i <= fim; i++) {
            printf("%c", s[i]);
        }
        printf("\n");
    }
    
    return 0;
}
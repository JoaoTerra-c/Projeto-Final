#include <stdio.h>
#include <string.h>

#define TAM_MAX 100

int main() {
    char s[TAM_MAX];
    int i;
    
    printf("2) Imprimir as Quatro Primeiras Letras\n");
    printf("Digite uma string: ");
    
    if (fgets(s, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s[strcspn(s, "\n")] = 0;
    
    printf("Quatro primeiras letras: ");
    for (i = 0; i < 4 && s[i] != '\0'; i++) {
        printf("%c", s[i]);
    }
    printf("\n");
    
    return 0;
}
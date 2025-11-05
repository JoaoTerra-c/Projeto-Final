#include <stdio.h>
#include <string.h>

#define TAM_MAX 100

int main() {
    char s[TAM_MAX];
    
    printf("1) Ler e Imprimir String\n");
    printf("Digite uma string: ");
    
    // Leitura da string (incluindo espaços)
    if (fgets(s, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    
    // Remove a quebra de linha do final
    s[strcspn(s, "\n")] = 0; 
    
    printf("String lida: %s\n", s);
    
    return 0;
}
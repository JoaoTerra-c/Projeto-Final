#include <stdio.h>
#include <string.h>

#define TAM_MAX 256 // Maior para acomodar o exemplo longo

int main() {
    char s[TAM_MAX];
    int i, len;
    
    printf("14) Cifra de Cesar (+3)\n");
    printf("Digite uma string para codificar:\n");
    
    // Limpa o buffer antes de usar fgets
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
    
    if (fgets(s, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s[strcspn(s, "\n")] = 0;
    len = strlen(s);
    
    for (i = 0; i < len; i++) {
        char c = s[i];
        
        if (c >= 'a' && c <= 'z') {
            // Formula para Cifra de César: c = 'a' + (c - 'a' + N) % 26
            c = 'a' + (c - 'a' + 3) % 26;
        } else if (c >= 'A' && c <= 'Z') {
            c = 'A' + (c - 'A' + 3) % 26;
        }
        // Outros caracteres (espaços, pontuação) permanecem inalterados
        s[i] = c;
    }
    
    printf("String codificada: %s\n", s);
    
    return 0;
}
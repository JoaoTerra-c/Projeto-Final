#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TAM_MAX 100

int main() {
    char s[TAM_MAX];
    char char_substituto;
    int cont_vogais = 0;
    int i, len;
    
    printf("6) Contar Vogais e Substituir\n");
    printf("Digite uma string: ");
    if (fgets(s, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s[strcspn(s, "\n")] = 0;
    len = strlen(s);
    
    printf("Digite o caractere substituto: ");
    // Usa scanf(" %c") para ler o caractere e ignorar espaços/newlines anteriores
    if (scanf(" %c", &char_substituto) != 1) {
        return 1;
    }

    for (i = 0; i < len; i++) {
        char c = tolower(s[i]);

        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            cont_vogais++;
            s[i] = char_substituto; // Substitui no local
        }
    }
    
    printf("Nova string: %s\n", s);
    printf("Numero de vogais na string original: %d\n", cont_vogais);
    
    return 0;
}
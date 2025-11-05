#include <stdio.h>
#include <string.h>

#define TAM_MAX 100

int main() {
    char s[TAM_MAX];
    int i, len;
    
    printf("11) Converter para Minuscula (Manual)\n");
    printf("Digite uma string: ");
    
    if (fgets(s, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s[strcspn(s, "\n")] = 0;
    len = strlen(s);
    
    for (i = 0; i < len; i++) {
        // Maiúsculas (A-Z) = 65 a 90, Minúsculas (a-z) = 97 a 122
        // Somando 32 de um caractere maiúsculo (65) ele se torna minúsculo (97)
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] + 32;
        }
    }
    
    printf("String em minuscula: %s\n", s);
    
    return 0;
}
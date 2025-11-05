#include <stdio.h>
#include <string.h>

#define TAM_MAX 100

int main() {
    char s[TAM_MAX];
    int len = 0;
    
    printf("3) Contar Caracteres sem strlen()\n");
    printf("Digite uma string: ");
    
    if (fgets(s, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s[strcspn(s, "\n")] = 0;

    // Contagem manual de caracteres
    while (s[len] != '\0') {
        len++;
    }
    
    printf("A string possui %d caracteres.\n", len);
    
    return 0;
}
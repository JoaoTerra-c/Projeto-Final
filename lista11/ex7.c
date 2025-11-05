#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TAM_MAX 100

int main() {
    char s[TAM_MAX];
    int i, j, len;
    
    printf("7) Verificar Palindromo\n");
    printf("Digite uma string: ");
    
    if (fgets(s, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s[strcspn(s, "\n")] = 0;
    len = strlen(s);
    
    int eh_palindromo = 1;
    
    for (i = 0, j = len - 1; i < j; i++, j--) {
        // Compara caracteres ignorando maiúsculas/minúsculas
        if (tolower(s[i]) != tolower(s[j])) {
            eh_palindromo = 0;
            break;
        }
    }

    if (eh_palindromo) {
        printf("A string '%s' E um palindromo.\n", s);
    } else {
        printf("A string '%s' NAO e um palindromo.\n", s);
    }
    
    return 0;
}
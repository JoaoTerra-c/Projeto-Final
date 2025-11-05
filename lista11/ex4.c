#include <stdio.h>
#include <string.h>

#define TAM_MAX 100

int main() {
    char s[TAM_MAX];
    int i, len;
    
    printf("4) Imprimir String de Tras para a Frente\n");
    printf("Digite uma string: ");
    
    if (fgets(s, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s[strcspn(s, "\n")] = 0;
    len = strlen(s);

    printf("String de tras para a frente: ");
    for (i = len - 1; i >= 0; i--) {
        printf("%c", s[i]);
    }
    printf("\n");
    
    return 0;
}
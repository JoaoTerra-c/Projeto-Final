#include <stdio.h>
#include <string.h>

#define TAM_MAX 100

int main() {
    char s[TAM_MAX];
    int i, j, len;
    
    printf("5) Inverter String no Local\n");
    printf("Digite uma string para ser invertida: ");
    
    if (fgets(s, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s[strcspn(s, "\n")] = 0;
    len = strlen(s);

    // Algoritmo de inversão (troca de extremidades)
    for (i = 0, j = len - 1; i < j; i++, j--) {
        char temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
    
    printf("String invertida: %s\n", s);
    
    return 0;
}
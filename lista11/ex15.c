#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TAM_MAX 100

int main() {
    char s1[TAM_MAX];
    char s2[TAM_MAX];
    char s_temp[TAM_MAX];
    
    printf("15) Imprimir Duas Strings em Ordem Alfabetica\n");
    
    // Limpa o buffer antes de usar fgets
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
    
    printf("Digite a primeira string (StrA): ");
    if (fgets(s1, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s1[strcspn(s1, "\n")] = 0;
    
    printf("Digite a segunda string (StrB): ");
    if (fgets(s2, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s2[strcspn(s2, "\n")] = 0;
    
    // strcmp retorna:
    // < 0 se s1 vier antes de s2
    // 0 se s1 for igual a s2
    // > 0 se s1 vier depois de s2
    
    if (strcmp(s1, s2) > 0) {
        // Se s1 for maior (vem depois), trocamos as strings
        strcpy(s_temp, s1);
        strcpy(s1, s2);
        strcpy(s2, s_temp);
    }
    
    printf("Ordem Alfabetica:\n");
    printf("1. %s\n", s1);
    printf("2. %s\n", s2);
    
    return 0;
}
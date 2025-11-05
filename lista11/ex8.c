#include <stdio.h>
#include <string.h>

#define TAM_MAX 100

int main() {
    char s1[TAM_MAX];
    char s2[TAM_MAX];
    
    printf("8) Verificar Substring (Contida)\n");
    printf("Digite a string principal (S1): ");
    if (fgets(s1, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s1[strcspn(s1, "\n")] = 0;
    
    printf("Digite a substring (S2): ");
    if (fgets(s2, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s2[strcspn(s2, "\n")] = 0;
    
    // strstr(str1, str2) retorna um ponteiro para a primeira ocorrência 
    // de str2 em str1, ou NULL se não for encontrada.
    if (strstr(s1, s2) != NULL) {
        printf("A string '%s' ESTA contida em '%s'.\n", s2, s1);
    } else {
        printf("A string '%s' NAO esta contida em '%s'.\n", s2, s1);
    }
    
    return 0;
}
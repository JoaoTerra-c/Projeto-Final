#include <stdio.h>
#include <string.h>

#define TAM_MAX 100

int main() {
    char s1[TAM_MAX];
    char s2[TAM_MAX];
    int i, j, contagem = 0;
    int len_s1, len_s2;
    
    printf("9) Contar Ocorrencias de Substring\n");
    printf("Digite a string principal (S1): ");
    if (fgets(s1, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s1[strcspn(s1, "\n")] = 0;
    len_s1 = strlen(s1);
    
    printf("Digite a substring (S2): ");
    if (fgets(s2, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    s2[strcspn(s2, "\n")] = 0;
    len_s2 = strlen(s2);
    
    if (len_s2 > 0) {
        // Percorre S1 até o ponto onde S2 pode começar
        for (i = 0; i <= len_s1 - len_s2; i++) {
            int encontrou = 1;
            // Verifica se os caracteres de S2 correspondem aos de S1 a partir da posição 'i'
            for (j = 0; j < len_s2; j++) {
                if (s1[i + j] != s2[j]) {
                    encontrou = 0;
                    break;
                }
            }
            if (encontrou) {
                contagem++;
                // Se a intenção é não contar sobreposições, adicione: i += len_s2 - 1;
            }
        }
    }
    
    printf("A string '%s' esta contida %d vez(es) em '%s'.\n", s2, contagem, s1);
    
    return 0;
}
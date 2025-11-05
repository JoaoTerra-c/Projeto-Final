#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Função para dividir uma string com base em um caractere delimitador.
 * Retorna um array de strings (char**) e armazena o número de substrings.
 */
char** divide_string(const char* str, char delimitador, int* num_substrings) {
    // Caso de erro: string de entrada ou ponteiro para o número de substrings inválidos
    if (str == NULL || num_substrings == NULL) {
        *num_substrings = 0;
        return NULL;
    }

    // Cria uma cópia da string para evitar modificar a original
    char* copia_str = strdup(str);
    if (copia_str == NULL) {
        *num_substrings = 0;
        return NULL;
    }

    // Conta o número de substrings para alocar a memória necessária
    int count = 0;
    char* temp_str = copia_str;
    while (*temp_str != '\0') {
        if (*temp_str == delimitador) {
            count++;
        }
        temp_str++;
    }
    // Adiciona 1 ao contador para a última substring
    count++;

    // Aloca memória para o array de ponteiros (o array de strings)
    char** substrings = (char**)malloc(count * sizeof(char*));
    if (substrings == NULL) {
        free(copia_str);
        *num_substrings = 0;
        return NULL;
    }

    // Divide a string e armazena as substrings
    char* token;
    int i = 0;
    
    token = strtok(copia_str, &delimitador);
    while (token != NULL) {
        substrings[i] = strdup(token);
        if (substrings[i] == NULL) {
            // Em caso de falha, libera a memória já alocada e retorna
            for (int j = 0; j < i; j++) {
                free(substrings[j]);
            }
            free(substrings);
            free(copia_str);
            *num_substrings = 0;
            return NULL;
        }
        i++;
        token = strtok(NULL, &delimitador);
    }
    
    *num_substrings = i;
    
    // Libera a cópia da string, pois as substrings já foram duplicadas
    free(copia_str);
    
    return substrings;
}

int main() {
    char str_entrada[256];
    char delimitador;
    int num_substrings_encontradas = 0;
    char** resultado_divisao;
    
    printf("Digite uma string: ");
    fgets(str_entrada, sizeof(str_entrada), stdin);
    str_entrada[strcspn(str_entrada, "\n")] = 0;
    printf("Digite um caractere delimitador: ");
    scanf(" %c", &delimitador);
    resultado_divisao = divide_string(str_entrada, delimitador, &num_substrings_encontradas);
      if (resultado_divisao == NULL) {
        printf("Erro na divisão da string.\n");
        return 1;
    }
    printf("\nString original: \"%s\"\n", str_entrada);
    printf("Delimitador: '%c'\n", delimitador);
    printf("Número de substrings encontradas: %d\n", num_substrings_encontradas);
    printf("Substrings:\n");

    for (int i = 0; i < num_substrings_encontradas; i++) {
        printf("[%d]: \"%s\"\n", i, resultado_divisao[i]);
    }
    if (resultado_divisao != NULL) {
        for (int i = 0; i < num_substrings_encontradas; i++) {
            free(resultado_divisao[i]);
        }
        free(resultado_divisao);
    }

    return 0;
}
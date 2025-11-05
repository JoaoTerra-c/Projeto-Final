#include <stdio.h>

void converterParaMaiuscula(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32;
        }
        i++;
    }
}

int main() {
    char minhaString[] = "Hello World!";
    
    printf("String original: %s\n", minhaString);
    
    converterParaMaiuscula(minhaString);
    
    printf("String em maiusculas: %s\n", minhaString);
    
    return 0;
}
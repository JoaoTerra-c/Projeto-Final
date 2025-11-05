#include <stdio.h>
#include <string.h>

int ehPalindromo(const char *str) {
    int esquerda = 0;
    int direita = strlen(str) - 1;

    while (direita > esquerda) {
        if (str[esquerda] != str[direita]) {
            return 0;
        }
        esquerda++;
        direita--;
    }
    return 1;
}

int main() {
    char palavra1[] = "arara";
    char palavra2[] = "hello";

    if (ehPalindromo(palavra1)) {
        printf("A string \"%s\" eh um palindromo.\n", palavra1);
    } else {
        printf("A string \"%s\" nao eh um palindromo.\n", palavra1);
    }

    if (ehPalindromo(palavra2)) {
        printf("A string \"%s\" eh um palindromo.\n", palavra2);
    } else {
        printf("A string \"%s\" nao eh um palindromo.\n", palavra2);
    }

    return 0;
}
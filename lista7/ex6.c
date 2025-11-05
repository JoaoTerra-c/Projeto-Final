#include <stdio.h>

// Definição da união com dois tipos básicos
union Numero {
    int inteiro;
    float real;
};

int main() {
    union Numero n;

    // Inicializando o campo inteiro
    n.inteiro = 42;

    // Exibindo o valor do campo inteiro
    printf("Valor do inteiro: %d\n", n.inteiro);

    // Interpretando o mesmo espaço de memória como float
    printf("Interpretando o mesmo espaço de memória como float: %f\n", n.real);

    return 0;
}

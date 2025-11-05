#include <stdio.h>

// Função que realiza a operação matemática
float calcularOperacao(float num1, float num2, char operador) {
    switch (operador) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 != 0)
                return num1 / num2;
            else {
                printf("Erro: divisão por zero!\n");
                return 0; // valor de erro
            }
        default:
            printf("Operador inválido! Use '+', '-', '*' ou '/'.\n");
            return 0; // valor de erro
    }
}

int main() {
    float a, b;
    char op;

    printf("Digite dois números separados por espaço: ");
    scanf("%f %f", &a, &b);

    printf("Digite o operador (+, -, *, /): ");
    scanf(" %c", &op); // espaço antes do %c para ignorar '\n' anterior

    float resultado = calcularOperacao(a, b, op);

    printf("O resultado da operação é: %.2f\n", resultado);

    return 0;
}

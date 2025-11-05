#include <stdio.h>
#include <string.h>

#define TAM_MAX 100

int main() {
    char nome_mercadoria[TAM_MAX];
    float valor_total;
    float desconto_percentual = 0.10f; // 10%
    float valor_desconto;
    float valor_a_pagar;

    printf("12) Calculo de Desconto (10%%)\n");
    
    // Limpa o buffer de entrada antes de ler a string
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
    
    printf("Nome da mercadoria: ");
    if (fgets(nome_mercadoria, TAM_MAX, stdin) == NULL) {
        return 1;
    }
    nome_mercadoria[strcspn(nome_mercadoria, "\n")] = 0;

    printf("Valor total da mercadoria: R$ ");
    if (scanf("%f", &valor_total) != 1 || valor_total < 0) {
        printf("Valor invalido.\n");
        return 1;
    } 

    valor_desconto = valor_total * desconto_percentual;
    valor_a_pagar = valor_total - valor_desconto;

    printf("\n--- Detalhes do Pagamento a Vista ---\n");
    printf("Mercadoria: %s\n", nome_mercadoria);
    printf("Valor Total: R$ %.2f\n", valor_total);
    printf("Valor do Desconto (10%%): R$ %.2f\n", valor_desconto);
    printf("Valor a Pagar a Vista: R$ %.2f\n", valor_a_pagar);
    
    return 0;
}
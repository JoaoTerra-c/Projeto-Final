#include <stdio.h>
#include <string.h>

#define NUM_ATLETAS 5
#define MAX_NOME 50

// Estrutura Atleta
struct Atleta {
    char nome[MAX_NOME];
    char esporte[MAX_NOME];
    int idade;
    float altura;
};

int main() {
    struct Atleta atletas[NUM_ATLETAS];
    int idx_mais_alto = 0;
    int idx_mais_velho = 0;

    printf("--- 9) Atleta: Mais Alto e Mais Velho ---\n");

    // Leitura dos dados
    for (int i = 0; i < NUM_ATLETAS; i++) {
        printf("\n--- Atleta %d ---\n", i + 1);
        
        // Limpar buffer antes de ler nome
        int c; while ((c = getchar()) != '\n' && c != EOF); 
        
        printf("Nome: ");
        fgets(atletas[i].nome, MAX_NOME, stdin);
        atletas[i].nome[strcspn(atletas[i].nome, "\n")] = 0;

        printf("Esporte: ");
        fgets(atletas[i].esporte, MAX_NOME, stdin);
        atletas[i].esporte[strcspn(atletas[i].esporte, "\n")] = 0;
        
        printf("Idade: ");
        scanf("%d", &atletas[i].idade);
        
        printf("Altura (m): ");
        scanf("%f", &atletas[i].altura);
    }
    
    // Encontra o mais alto e o mais velho
    for (int i = 1; i < NUM_ATLETAS; i++) {
        // Mais alto (maior altura)
        if (atletas[i].altura > atletas[idx_mais_alto].altura) {
            idx_mais_alto = i;
        }

        // Mais velho (maior idade)
        if (atletas[i].idade > atletas[idx_mais_velho].idade) {
            idx_mais_velho = i;
        }
    }

    // Exibição dos resultados
    printf("\n====================================\n");
    printf("ATLETA MAIS ALTO:\n");
    printf("Nome: %s | Altura: %.2f m\n", 
           atletas[idx_mais_alto].nome,
           atletas[idx_mais_alto].altura);
    
    printf("\nATLETA MAIS VELHO:\n");
    printf("Nome: %s | Idade: %d anos\n", 
           atletas[idx_mais_velho].nome,
           atletas[idx_mais_velho].idade);
    printf("====================================\n");

    return 0;
}
#include <stdio.h>
void main() {
    int A[3][3], B[3][3], sub[3][3], mult[3][3];
    int i, j, k, opcao;
    int iguais, simetrica;

    printf("\nInforme os valores da matriz A (3x3):\n");
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            printf("A[%d][%d]: ", i+1, j+1);
            scanf("%d", &A[i][j]);
        }
    }
    printf("\nInforme os valores da matriz B (3x3):\n");
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            printf("B[%d][%d]: ", i+1, j+1);
            scanf("%d", &B[i][j]);
        }
    }

    do {
    printf("\n===== MENU =====\n");
    printf("1 - Subtrair A - B\n");
    printf("2 - Multiplicar A * B\n");
    printf("3 - Verificar se as matrizes sao simetricas\n");
    printf("4 - Verificar se as matrizes sao iguais\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

        if(opcao == 1) {
            printf("\nResultado da subtracao (A - B):\n");
            for(i = 0; i < 3; i++) {
                for(j = 0; j < 3; j++) {
                    sub[i][j] = A[i][j] - B[i][j];
                    printf("%4d", sub[i][j]);
                }
                printf("\n");
            }
        }
        else if(opcao == 2) {
            printf("\nResultado da multiplicacao (A * B):\n");
            for(i = 0; i < 3; i++) {
                for(j = 0; j < 3; j++) {
                    mult[i][j] = 0;
                    for(k = 0; k < 3; k++) {
                        mult[i][j] += A[i][k] * B[k][j];
                    }
                    printf("%4d", mult[i][j]);
                }
                printf("\n");
            }
        }
        else if(opcao == 3) {
            simetrica = 1;
            for(i = 0; i < 3; i++) {
                for(j = 0; j < 3; j++) {
                    if(A[i][j] != A[j][i]) simetrica = 0;
                }
            }
            if(simetrica) printf("\nMatriz A eh simetrica.\n");
            else printf("\nMatriz A NAO eh simetrica.\n");

            simetrica = 1;
            for(i = 0; i < 3; i++) {
                for(j = 0; j < 3; j++) {
                    if(B[i][j] != B[j][i]) simetrica = 0;
                }
            }
            if(simetrica) printf("Matriz B eh simetrica.\n");
            else printf("Matriz B NAO eh simetrica.\n");
        }
        else if(opcao == 4) {
            iguais = 1;
            for(i = 0; i < 3; i++) {
                for(j = 0; j < 3; j++) {
                    if(A[i][j] != B[i][j]) iguais = 0;
                }
            }
            if(iguais) printf("\nAs matrizes A e B sao iguais.\n");
            else printf("\nAs matrizes A e B NAO sao iguais.\n");
        }
        else if(opcao == 0) {
            printf("\nEncerrando o programa...\n");
        }
        else {
            printf("\nOpcao invalida!\n");
        }

    } while(opcao != 0);
}
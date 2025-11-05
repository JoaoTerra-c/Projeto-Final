#include <stdio.h>
void main(){
    int A[3][3], b[3][3], Soma[3][3];
    int i, j;
    
    printf("\nInforme o valor dos elementos da matriz A: ");
    for(i=0;i<3;i++){
        for(j=0;j<2;j++){
            printf("\nelemento %d da %d° linha ",i + 1,j + 1);
            scanf("%d",&A[i][j]);
        }
    }
    printf("\nInforme o valor dos elementos da matriz B: ");
    for(i=0;i<3;i++){
        for(j=0;j<2;j++){
            printf("\nelemento %d da %d° linha ",i + 1,j + 1);
            scanf("%d",&b[i][j]);
        }
    }
     for(i=0;i<3;i++){
        for(j=0;j<2;j++){
            Soma[i][j] = A[i][j] + b[i][j];
        }
    }
     printf("\nSoma das matrizes:\n");
    for(i = 0; i < 2; i++) {
        for(j = 0; j < 3; j++) {
            printf("%d ", Soma[i][j]);
        }
        printf("\n");
    }
}
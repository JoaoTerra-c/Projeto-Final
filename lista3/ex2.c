#include <stdio.h>
void main(){
    int matriz[3][2], matrizTrans[3][2];
    int i, j;

    printf("\nInforme o valor dos elementos da matriz");
    for(i=0;i<3;i++){
        for(j=0;j<2;j++){
            printf("\nelemento %d da %d° linha ",i + 1,j + 1);
            scanf("%d",&matriz[i][j]);
        }
    }
    for(i=0;i<3;i++){
        for(j=0;j<2;j++){
         matrizTrans[j][i]=matriz[i][j];
        }
    }
    printf("\nMatriz Transposta (At):\n");
    for(i = 0; i < 2; i++) {
        for(j = 0; j < 3; j++) {
            printf("%d ", matrizTrans[i][j]);
        }
        printf("\n");
    }
}
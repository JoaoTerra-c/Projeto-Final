#include <stdio.h>

void main (){

    //variceis
    int matriz[3][3];
    int i, j;
    int diagonalP, diagonalS;
    printf("\nInforme o valor dos elementos da matriz");
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            printf("\nelemento %d da %d° linha",i + 1,j + 1);
            scanf("%d",&matriz[i][j]);
        }
    }
    diagonalP = matriz[1][1] + matriz[2][2] + matriz[3][3];
    diagonalS = matriz[3][1] + matriz[1][3] + matriz[2][2];
    if(diagonalP == diagonalS){
    printf("A soma dos elementos das diagonais são iguais");
    } else{
    printf("A soma dos elementos das diagonais não é igual\n");
    }
}
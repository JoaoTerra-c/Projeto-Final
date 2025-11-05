#include <stdio.h>
void main(){
    int matriz[3][3];
    int i, j;
    int identidade = 1;
    printf("\nInforme o valor dos elementos da matriz: ");
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            printf("\nelemento %d da %d° linha",i + 1,j + 1);
            scanf("%d",&matriz[i][j]);
        }
    }
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            if(i == j) { 
                if(matriz[i][j] != 1) {
                    identidade = 0;
                }
            } else { 
                if(matriz[i][j] != 0) {
                    identidade = 0;
                }
            }
        }
    }
    if(identidade == 1){
    printf("\n A matriz é identidade");
    }else{ 
    printf("\n A matriz não é identidade ");
    }
}
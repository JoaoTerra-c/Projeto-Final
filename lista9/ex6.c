#include <stdio.h>
#include <stdlib.h>

int Media(int n, int *p){
    int soma = 0;
    for (int i = 0; i < n; i++) {
        soma += p[i];
    }
    return soma/n;
}
void main(){
    int n;
    int result;
    int *p;

    printf("Informe o tamanho do arranjo ");
    scanf("%d",&n);

    printf("\nInforme os números do arranjo:\n ");
    
    p = (int *) malloc( n * sizeof(int));

    for(int i=0;i<n;i++){
        printf("elemento %d°; ",i+1);
        scanf("%d",&p[i]);
    }

    result = Media(n,p);

    printf("A média dos elementos do arranjo será %d\n",result);
}
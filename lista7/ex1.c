#include <stdio.h>

int balança(int x, int y){
    if(x < y){
        return y;
    } else {
        return x;
    }
}
void main(){
    int x, y, maior;

    printf("Informe um número inteiro: ");
    scanf("%d",&x);
    printf("Informe mais um número inteiro: ");
    scanf("%d",&y);

    maior = balança(x,y);

    printf("O maior dos dois números é %d\n",maior);
}
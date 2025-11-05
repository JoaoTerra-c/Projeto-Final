#include <stdio.h>

int Naturais(int n){
    if(n == 0 || n == 1){
        return n;
    } else {
        printf("%d ",n-1);
        Naturais(n-1);
    }
}
void main(){
    int n;

    printf("Digite um número: ");
    scanf("%d",&n);
    
    printf("Os números entre ele e 0 são: ");
    Naturais(n);
}
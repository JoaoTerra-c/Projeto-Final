#include <stdio.h>

void Decrescente(int n){
        if(n > 0){
        printf("%d ",n -1);
        Decrescente(n-1);
    }
}
void main(){
    int n;

    printf("Digite um número: ");
    scanf("%d",&n);
    
    printf("Os números entre ele e 0 (na ordem decrescente) são: ");
    
    Decrescente(n);
}
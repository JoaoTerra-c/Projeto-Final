#include <stdio.h>

int soma(int a, int b){
    return (2*a)+(2*b);
}
void main(){
    int a, b;
    int result;

    printf("Informe um número: ");
    scanf("%d",&a);
    printf("Informe um número: ");
    scanf("%d",&b);
    
    result = soma(a,b);

    printf("A soma do dobro dos dois números será: %d", result);

}
#include <stdio.h>

int SomaAteN(int n){
    int i=1;
    int result =0;
    if(i<=n){
        while (i<=n) { 
            result = result + i;
            i++;
        }
    } else {
            return result;
        }
    }
void main(){
    int n;
    int soma;

    printf("Você deseja saber a soma de 1 até que numero? ");
    scanf("%d",&n);

    soma = SomaAteN(n);

    printf("\n o valor da soma desses elementos será: %d\n", soma);

}

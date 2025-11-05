#include <stdio.h>

int Potencia(int x, int y){
    if(1<y){
        x = x * x;
        y--;
        Potencia(x,y);
    } else if(y==1){
        return x;
    }
}
void main(){
    int x, y;
    int result;

    printf("informe um número: ");
    scanf("%d",&x);
    printf("Informe a qual número esse número será elevado: ");
    scanf("%d",&y);

    result = Potencia(x,y);

    printf("O resultado dessa potêmcia será: %d\n",result);
}
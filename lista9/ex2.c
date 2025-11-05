#include <stdio.h>

int Somatorio(int n){
    if(n==1 || n == 0){
        return 1;
    } else{
        return n + Somatorio(n-1);
    }
}
void main(){
    int n;
    int result;

    printf("Informe o número que deve ser somado ");
    scanf("%d",&n);

    result = Somatorio(n);

    printf("\n O somatório será %d \n",result);
}
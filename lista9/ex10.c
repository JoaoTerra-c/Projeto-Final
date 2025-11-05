#include <stdio.h>

long long int fibo(int n){
    if(n==0){
        return 0;
    } else if (n == 1){
        return 1;
    } else {
        return fibo(n-1) + fibo(n-2);
    }
}
void main(){
    long long int x;
    long long int result;

    printf("informe um número: ");
    scanf("%lld",&x);

    result = fibo(x);

    printf("Essa posição pertence ao número %lld na seuqncia de Fibonacci", result);
}
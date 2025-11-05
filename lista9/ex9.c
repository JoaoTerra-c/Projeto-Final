#include <stdio.h>

long long int fatorial(long long int n){
    if(n == 0 || n==1){
        return 1;
    } else if(n>=2){
        return n * fatorial(n - 1);
    }
}
void main(){
    long long int n;
    long long int result;

    printf("Informe qual número vc deseja saber o fatorial: ");
    scanf("%lld",&n);

    result = fatorial(n);
    
    printf("O fatorial será: %lld\n", result);
}
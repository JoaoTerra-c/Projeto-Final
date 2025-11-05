#include <stdio.h>

typedef struct {

    char nome [50];
    int numero_da_conta;
    float saldo;

} ContaBanco;

void main(){
    ContaBanco Conta1;
    Conta1.saldo =0;
    printf("%.2f\n",Conta1.saldo);
    printf("Qual é o nome da pessoa que deseja acessar a conta");
    scanf("%s",Conta1.nome);

    printf(" ");
}
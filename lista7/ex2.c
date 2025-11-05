#include <stdio.h>

void mensagem(int mes) {
    if(mes == 1){
        printf("O mês é Janeiro\n");
    } else if (mes == 2){
        printf("O mês é Fevereiro\n");
    } else if (mes == 3){
        printf("O mês é Março\n");
    } else if (mes == 4){
        printf("O mês é Abril\n");
    } else if (mes == 5){
        printf("O mês é Maio\n");
    } else if (mes == 6){
        printf("O mês é Junho\n");
    } else if (mes == 7){
        printf("O mês é Julho\n");
    } else if (mes == 8){
        printf("O mês é Agosto\n");
    } else if (mes == 9){
        printf("O mês é Setembro\n");
    } else if (mes == 10){
        printf("O mês é Outubro\n");
    } else if (mes == 11){
        printf("O mês é Novembro\n");
    } else if (mes == 12){
        printf("O mês é Dezembro\n");
    } else {
        printf("Número inválido! Digite de 1 a 12.\n");
    }
}

void main() {
    int mes;
    printf("Informe o número correspondente ao mês: ");
    scanf("%d", &mes);

    mensagem(mes);
}
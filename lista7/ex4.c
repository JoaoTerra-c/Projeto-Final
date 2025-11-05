#include <stdio.h>

#define PI 3.141592

float calculo(float raio, float altura){
    return PI*raio*raio*altura;
}
void main(){
    float raio, altura, volume;

    printf("Informe o valor do raio do cilindro: ");
    scanf("%f",&raio);
    printf("informe o valor da altura do cilindro: ");
    scanf("%f",&altura);

    volume = calculo(raio,altura);

    printf("O volume desse cilindro sera %.2f mL\n",volume);
}
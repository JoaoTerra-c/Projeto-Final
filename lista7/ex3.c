#include <stdio.h>

float convercao(float f){
    return (f -32.0)*(5.0/9.0);
}
void main(){
    float f, celcius;
    
    printf("informe a temperatura em graus fahrenheit a ser convertida para celcius: ");
    scanf("%f",&f);

    celcius = convercao(f);

    printf("A temperatura em gráus Celcius será %.2f\n",celcius);
}
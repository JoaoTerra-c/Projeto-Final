#include <stdio.h>

// Função que converte horas, minutos e segundos em segundos
int horarioEmSegundos(int horas, int minutos, int segundos) {
    return horas * 3600 + minutos * 60 + segundos;
}

int main() {
    int h, m, s;

    printf("Digite as horas, minutos e segundos (separados por espaço): ");
    scanf("%d %d %d", &h, &m, &s);

    int totalSegundos = horarioEmSegundos(h, m, s);

    printf("O horário convertido em segundos é: %d segundos\n", totalSegundos);

    return 0;
}

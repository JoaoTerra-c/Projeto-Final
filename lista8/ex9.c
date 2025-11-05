#include <stdio.h>
#include <string.h>

struct Aluno {
    int matricula;
    char nome[50];
    float notas[3];
};

int encontrarIndiceMaiorMedia(const struct Aluno alunos[], int n) {
    int i, j;
    float media_atual;
    float maior_media = -1.0;
    int indice_maior_media = -1;

    for (i = 0; i < n; i++) {
        float soma_notas = 0.0;
        for (j = 0; j < 3; j++) {
            soma_notas += alunos[i].notas[j];
        }
        media_atual = soma_notas / 3.0;

        if (media_atual > maior_media) {
            maior_media = media_atual;
            indice_maior_media = i;
        }
    }
    return indice_maior_media;
}

int main() {
    struct Aluno turma[5];
    int i;
    int indice_aluno_destaque;

    turma[0].matricula = 101;
    strcpy(turma[0].nome, "Ana Silva");
    turma[0].notas[0] = 7.5;
    turma[0].notas[1] = 8.0;
    turma[0].notas[2] = 9.0;

    turma[1].matricula = 102;
    strcpy(turma[1].nome, "Bruno Costa");
    turma[1].notas[0] = 6.0;
    turma[1].notas[1] = 7.0;
    turma[1].notas[2] = 6.5;

    turma[2].matricula = 103;
    strcpy(turma[2].nome, "Carlos Lima");
    turma[2].notas[0] = 9.5;
    turma[2].notas[1] = 9.0;
    turma[2].notas[2] = 8.5;

    turma[3].matricula = 104;
    strcpy(turma[3].nome, "Diana Rocha");
    turma[3].notas[0] = 8.0;
    turma[3].notas[1] = 8.5;
    turma[3].notas[2] = 8.2;
    
    turma[4].matricula = 105;
    strcpy(turma[4].nome, "Eduardo Souza");
    turma[4].notas[0] = 7.0;
    turma[4].notas[1] = 7.5;
    turma[4].notas[2] = 7.0;

    indice_aluno_destaque = encontrarIndiceMaiorMedia(turma, 5);

    if (indice_aluno_destaque != -1) {
        printf("O aluno com a maior media e: %s\n", turma[indice_aluno_destaque].nome);
        printf("Matricula: %d\n", turma[indice_aluno_destaque].matricula);
        printf("Notas: %.1f, %.1f, %.1f\n", turma[indice_aluno_destaque].notas[0], turma[indice_aluno_destaque].notas[1], turma[indice_aluno_destaque].notas[2]);
    } else {
        printf("Nenhum aluno encontrado.\n");
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>

int main(){
    char nome [50];
    float nota;
    FILE *arq;

    arq = fopen("Alunos.txt!","w+");
    if(arq == NULL){
        printf("Erro ao abrir arquivo");
        return 1;
    } else {
    for(int i=0;i<5;i++){
    printf("informe o nome e no do %d° aluno ",i+1);
    scanf("%s",nome);
    scanf("%f",&nota);
    }
    fscanf(arq,"Aluno %s\n", nome);
    fscanf(arq,"Nota %f",&nota);
    }
    fclose(arq);
    return 0;
}
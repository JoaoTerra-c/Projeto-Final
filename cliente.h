#ifndef CLIENTE_H
#define CLIENTE_H

typedef enum {
    Pessoa_Fisica = 1,
    Pessoa_Juridica = 2
}TipoPessoa;

typedef struct {

    //Identificador único
    int id;

    //Pessoa física ou jurídica
    TipoPessoa tipo;

    //Pessoa física
    char cpf[12]; //11 dígitos + '\0'

    //Pessoa jurídica
    char cnpj[15]; //14 dígitos + '\0'
    char razao_social[100];
    char nome_contato[100];

    //Informações comuns
    char nome[100];
    char endereco[200];
    char telefone[20];
    char email[100];

}Cliente;

#endif

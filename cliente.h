#ifndef CLIENTE_H
#define CLIENTE_H
#include <ncurses.h>

typedef enum {
    Pessoa_Fisica = 1,
    Pessoa_Juridica = 2
}TipoPessoa;

typedef struct {

    //Identificador único
    int id;
    char codigo[32];

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

int cadastro_cpf(const char *cpf_in, char out[12]);
int cadastro_nome(char out[100]);
int cadastro_endereco(char out[200]);
int cadastro_telefone(char out[20]);
int cadastro_email(char out[100]);


int validar_codigo_cliente(const char *codigo);
int cliente_codigo_existe_no_csv(const char *path, const char *codigo);

int cliente_gravar_csv(const Cliente *c);
int cliente_obter_proximo_id_pelo_csv(const char *caminho_csv);

int cadastro_cnpj(const char *cnpj_entrada, char cnpj_saida[15]);

int cliente_cpf_existe_no_csv(const char *caminho_arquivo_csv, const char *cpf_procurado);
int cliente_cnpj_existe_no_csv(const char *caminho_arquivo_csv, const char *cnpj_procurado);

void normaliza_excel_texto(char *s);
void somente_digitos(char *s);

int cliente_buscar_por_codigo(const char *path, const char *codigo, Cliente *out);

int cliente_remover_por_codigo(const char *path, const char *codigo);

int cliente_carregar_todos(const char *path, Cliente **vet, int *n); // aloca e carrega todos
void cliente_libera_lista(Cliente *vet);                             // libera o vetor
int ui_listar_clientes(WINDOW *janela);                              // UI ncurses

#endif

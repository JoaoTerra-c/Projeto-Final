#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cliente.h"


int cliente_obter_proximo_id_pelo_csv(const char *caminho_csv) {
    FILE *arquivo = fopen(caminho_csv, "r");
    if (!arquivo) {
        return 1;
    }

    int maior_id = 0;
    int id_lido = 0;
    char linha[1024];

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (strncmp(linha, "id;", 3) == 0 || strncmp(linha, "ID;", 3) == 0)
            continue;

        if (sscanf(linha, "%d;", &id_lido) == 1) {
            if (id_lido > maior_id) {
                maior_id = id_lido;
            }
        }
    }

    fclose(arquivo);
    return maior_id + 1;
}

int validar_codigo_cliente(const char *codigo) {
    if (!codigo || !*codigo) return 1;              // vazio
    for (const char *p = codigo; *p; ++p) {
        if (*p == ';' || isspace((unsigned char)*p)) // sem ';' e sem espaços
            return 1;
    }
    return 0; // ok
}

int cliente_codigo_existe_no_csv(const char *path, const char *codigo) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;

    char linha[1024];
    // se tiver cabeçalho, você pode consumi-lo aqui com um fgets(linha,...)

    while (fgets(linha, sizeof(linha), f)) {
        // Esperado: id;tipo;codigo;cpf;cnpj;razao_social;nome_contato;nome;endereco;telefone;email
        char *tok = strtok(linha, ";");  // id
        if (!tok) continue;
        tok = strtok(NULL, ";");         // tipo
        if (!tok) continue;
        tok = strtok(NULL, ";");         // codigo
        if (!tok) continue;

        tok[strcspn(tok, "\r\n")] = '\0'; // strip \r\n
        if (strcmp(tok, codigo) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int cliente_gravar_csv(const Cliente *c) {
    FILE *f = fopen("cliente.csv", "a");
    if (!f) { perror("open"); return 6; }

    const char *tipo = (c->tipo == Pessoa_Fisica) ? "PF" : "PJ";

    // id;tipo;codigo;cpf;cnpj;razao_social;nome_contato;nome;endereco;telefone;email
    fprintf(f, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
        c->id,
        tipo,
        c->codigo,                                        // NOVO: código externo único
        (c->tipo == Pessoa_Fisica)    ? c->cpf          : "",
        (c->tipo == Pessoa_Juridica)  ? c->cnpj         : "",
        (c->tipo == Pessoa_Juridica)  ? c->razao_social : "",
        (c->tipo == Pessoa_Juridica)  ? c->nome_contato : "",
        (c->tipo == Pessoa_Fisica)    ? c->nome         : "",
        c->endereco,
        c->telefone,
        c->email
    );

    fclose(f);
    return 0;
}

int cadastro_cpf(const char *cpf_in, char cpf_out[12]) {

    if (!cpf_in) return 1;

     char cpf[16];
    size_t len = 0;
    
    while (cpf_in[len] && cpf_in[len] != '\r' && cpf_in[len] != '\n' && len < sizeof(cpf) - 1) {
        cpf[len] = cpf_in[len];
        len++;
    }

    cpf[len] = '\0';

     if (len == 0) {
        return 1;
    }

    if(len != 11) {
        return 1;
    }

    for (int i = 0; i < 11; i++) {
        if (!isdigit((unsigned char)cpf[i])) {
            return 1;
        }
    }

    int iguais = 1;
    for(int i = 1; i < 11; i++) {
        if(cpf[i] != cpf[0]) {
            iguais = 0;
            break;
        }
    }

    if(iguais) {
        return 1;
    }
    

    int soma = 0;
    for(int i = 0; i < 9; i++) {
        soma += (cpf[i] - '0') * (10 - i);
    }

    int resultado = (soma*10)%11;
    if(resultado == 10) resultado = 0;
    if(resultado != (cpf[9] - '0')) {
        return 1;
    }

    soma = 0;
    for(int i = 0; i < 10; i++) {
        soma += (cpf[i] - '0') * (11 - i);
    }

    resultado = (soma*10)%11;
    if(resultado == 10) resultado = 0;

    if(resultado != (cpf[10] - '0')) {
        return 1;
    }

    memcpy(cpf_out, cpf, 12);
    return 0;
}

int cadastro_nome(char nome[100]) {

    size_t len = strcspn(nome, "\r\n");
    nome[len] = '\0';

    if (len == 0) {
        return 1;
    }

    return 0;
}

int cadastro_endereco(char endereco[200]) {

    size_t len = strcspn(endereco, "\r\n");
    endereco[len] = '\0';

    if (len == 0) {
        return 1;
    }

    return 0;
}

int cadastro_telefone(char telefone[20]) {

    size_t len = strcspn(telefone, "\r\n");
    telefone[len] = '\0';

    if (len == 0) {
        return 1;
    }

    if (len < 8 || len > 11) {
        return 1;
    }

    for (size_t i = 0; i < len; i++) {
        if (!isdigit((unsigned char)telefone[i])) {
            return 1;
        }
    }

    return 0;
}

int cadastro_email(char email[100]) {

    size_t len = strcspn(email, "\r\n");
    email[len] = '\0';

    if (len == 0) {
        return 1;
    }

    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)email[i])) {
            return 1;
        }
    }

    char *arroba = strchr(email, '@');
    if(!arroba) {
        return 1;
    }

    char *ponto = strchr(arroba + 1, '.');
    if(!ponto || ponto == arroba + 1 || ponto[1] == '\0') {
        return 1;
    }

    return 0;
}

int cadastro_pf() {
    Cliente c;

    char linha[64];
    printf("Digite apenas os 11 digitos do seu cpf: ");
    if (!fgets(linha, sizeof(linha), stdin)) return 1;
    if (cadastro_cpf(linha, c.cpf) != 0) return 1;

    if(cadastro_nome(c.nome) != 0) return 2;
    if(cadastro_endereco(c.endereco) != 0) return 3;
    if(cadastro_telefone(c.telefone) != 0) return 4;
    if(cadastro_email(c.email) != 0) return 5;

    c.id = cliente_obter_proximo_id_pelo_csv("cliente.csv");

    cliente_gravar_csv(&c);

    return 0;
}

int cadastro_cnpj(const char *cnpj_entrada, char cnpj_saida[15]) {
    char numero_cnpj_limpo[15];            // 14 dígitos + '\0'
    int  quantidade_digitos = 0;

    if (!cnpj_entrada) return 1;

    // 1) Copiar apenas os dígitos 0-9 para numero_cnpj_limpo
    for (const char *ponteiro = cnpj_entrada; *ponteiro; ++ponteiro) {
        if (*ponteiro >= '0' && *ponteiro <= '9') {
            if (quantidade_digitos >= 14) break; // já temos 14; ignora o restante
            numero_cnpj_limpo[quantidade_digitos++] = *ponteiro;
        }
    }
    numero_cnpj_limpo[quantidade_digitos] = '\0';

    // 2) Precisa ter exatamente 14 dígitos
    if (quantidade_digitos != 14) return 1;

    // 3) Recusar CNPJ com todos dígitos iguais (ex: "000...0", "111...1", etc.)
    int todos_digitos_iguais = 1;
    for (int indice = 1; indice < 14; ++indice) {
        if (numero_cnpj_limpo[indice] != numero_cnpj_limpo[0]) {
            todos_digitos_iguais = 0;
            break;
        }
    }
    if (todos_digitos_iguais) return 1;

    // 4) Calcular 1º dígito verificador (usa os 12 primeiros)
    // Pesos: 5 4 3 2 9 8 7 6 5 4 3 2
    int soma_pesada = 0;
    int pesos_primeiro[12] = {5,4,3,2,9,8,7,6,5,4,3,2};
    for (int indice = 0; indice < 12; ++indice) {
        int algarismo = numero_cnpj_limpo[indice] - '0';
        soma_pesada += algarismo * pesos_primeiro[indice];
    }
    int resto_divisao = soma_pesada % 11;
    int digito_verificador1 = (resto_divisao < 2) ? 0 : (11 - resto_divisao);

    if (digito_verificador1 != (numero_cnpj_limpo[12] - '0')) return 1;

    // 5) Calcular 2º dígito verificador (usa os 12 + 1º DV)
    // Pesos: 6 5 4 3 2 9 8 7 6 5 4 3 2
    soma_pesada = 0;
    int pesos_segundo[13] = {6,5,4,3,2,9,8,7,6,5,4,3,2};
    for (int indice = 0; indice < 13; ++indice) {
        int algarismo = numero_cnpj_limpo[indice] - '0';
        soma_pesada += algarismo * pesos_segundo[indice];
    }
    resto_divisao = soma_pesada % 11;
    int digito_verificador2 = (resto_divisao < 2) ? 0 : (11 - resto_divisao);

    if (digito_verificador2 != (numero_cnpj_limpo[13] - '0')) return 1;

    // 6) Se chegou aqui, é válido → copiar para a saída
    memcpy(cnpj_saida, numero_cnpj_limpo, 15); // 14 dígitos + '\0'
    return 0;
}

void listar_nomes() {
    FILE *arquivo;
    char lista;

    arquivo = fopen("cliente.csv", "r");

    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo.");
        system("pause");
        exit(1);
    }

    while((lista = fgetc(arquivo)) != EOF) {
        printf("%c", lista);
    }

    fclose(arquivo);
    system("pause");
}

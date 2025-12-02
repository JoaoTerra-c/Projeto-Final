#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cliente.h"

//FUNÇÕES DE CADASTRO DE CLIENTE

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

    char cpf_excel[20] = "";
    if (c->tipo == Pessoa_Fisica) snprintf(cpf_excel, sizeof(cpf_excel), "=\"%s\"", c->cpf);

    char cnpj_excel[32] = "";
    if (c->tipo == Pessoa_Juridica) {
        snprintf(cnpj_excel, sizeof(cnpj_excel), "'%s'", c->cnpj);
    }

    char telefone_excel[32];
    snprintf(telefone_excel, sizeof(telefone_excel), "=\"%s\"", c->telefone);

    // id;tipo;codigo;cpf;cnpj;razao_social;nome_contato;nome;endereco;telefone;email
    fprintf(f, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
        c->id,
        tipo,
        c->codigo,                                        
        (c->tipo == Pessoa_Fisica)    ? cpf_excel       : "",
        (c->tipo == Pessoa_Juridica)  ? cnpj_excel      : "",
        (c->tipo == Pessoa_Juridica)  ? c->razao_social : "",
        (c->tipo == Pessoa_Juridica)  ? c->nome_contato : "",
        (c->tipo == Pessoa_Fisica)    ? c->nome         : "",
        c->endereco,
        telefone_excel,
        c->email
    );

    fclose(f);
    return 0;
}

static void remover_quebra_linha_final(char *texto) {
    if (!texto) return;
    texto[strcspn(texto, "\r\n")] = '\0';
}

int cliente_cpf_existe_no_csv(const char *path, const char *cpf_procurado) {
    if (!path || !cpf_procurado || !cpf_procurado[0]) return 0;
    FILE *f = fopen(path, "r");
    if (!f) return -1;

    char linha[4096];
    while (fgets(linha, sizeof(linha), f)) {
        if (strncmp(linha, "id;", 3) == 0 || strncmp(linha, "ID;", 3) == 0) continue;

        // pegar CAMPO 3 (0=id,1=tipo,2=codigo,3=cpf)
        int alvo = 3, atual = 0;
        char *ini = linha, *fim;
        char campo[64] = {0};
        int ok = 0;

        for (;;) {
            fim = strchr(ini, ';');
            if (!fim) { // último
                if (atual == alvo) {
                    size_t n = strcspn(ini, "\r\n");
                    if (n >= sizeof(campo)) n = sizeof(campo)-1;
                    memcpy(campo, ini, n);
                    campo[n] = '\0';
                    ok = 1;
                }
                break;
            } else {
                if (atual == alvo) {
                    size_t n = (size_t)(fim - ini);
                    if (n >= sizeof(campo)) n = sizeof(campo)-1;
                    memcpy(campo, ini, n);
                    campo[n] = '\0';
                    ok = 1;
                    break;
                }
                atual++;
                ini = fim + 1;
            }
        }
        if (!ok || campo[0] == '\0') continue;

        // manter só dígitos
        char digits[16]; int k = 0;
        for (const char *p = campo; *p && k < 11; ++p) {
            if (*p >= '0' && *p <= '9') digits[k++] = *p;
        }
        digits[k] = '\0';
        if (k == 11 && strcmp(digits, cpf_procurado) == 0) {
            fclose(f);
            return 1;
        }
    }
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

int cliente_cnpj_existe_no_csv(const char *caminho_arquivo_csv, const char *cnpj_procurado) {
    if (!caminho_arquivo_csv || !cnpj_procurado || cnpj_procurado[0] == '\0')
        return 0;

    FILE *arquivo_csv = fopen(caminho_arquivo_csv, "r");
    if (!arquivo_csv)
        return -1;

    char linha[2048];
    while (fgets(linha, sizeof(linha), arquivo_csv)) {
        /* pula cabeçalho, se houver */
        if (strncmp(linha, "id;", 3) == 0 || strncmp(linha, "ID;", 3) == 0)
            continue;

        /*pegar exatamente o CAMPO 4 (indexado 0=id,1=tipo,2=codigo,3=cpf,4=cnpj) preservando vazios --- */
        int campo_alvo = 4;           /* queremos o CNPJ */
        int campo_atual = 0;
        char *p = linha;
        char *inicio = p;
        char *fim = NULL;
        char cnpj_campo[128] = {0};
        int achou = 0;

        for (;;) {
            fim = strchr(inicio, ';');
            if (!fim) { /* último campo (sem ';' à direita) */
                if (campo_atual == campo_alvo) {
                    size_t n = strcspn(inicio, "\r\n");
                    if (n >= sizeof(cnpj_campo)) n = sizeof(cnpj_campo)-1;
                    memcpy(cnpj_campo, inicio, n);
                    cnpj_campo[n] = '\0';
                    achou = 1;
                }
                break;
            } else {
                if (campo_atual == campo_alvo) {
                    size_t n = (size_t)(fim - inicio);
                    if (n >= sizeof(cnpj_campo)) n = sizeof(cnpj_campo)-1;
                    memcpy(cnpj_campo, inicio, n);
                    cnpj_campo[n] = '\0';
                    achou = 1;
                    break;
                }
                /* avança para o próximo campo (permitindo campo vazio) */
                campo_atual++;
                inicio = fim + 1;
            }
        }

        if (!achou) continue;             /* linha sem campo 4 (malformada) */
        if (cnpj_campo[0] == '\0') continue; /* PF: sem CNPJ */

        /* normaliza: mantém só dígitos (ignora ' , ="...") */
        char somente_digitos[16];
        int k = 0;
        for (const char *q = cnpj_campo; *q && k < 14; ++q) {
            if (*q >= '0' && *q <= '9') somente_digitos[k++] = *q;
        }
        somente_digitos[k] = '\0';

        if (k == 14 && strcmp(somente_digitos, cnpj_procurado) == 0) {
            fclose(arquivo_csv);
            return 1; /* encontrado */
        }
    }

    fclose(arquivo_csv);
    return 0; /* não encontrado */
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

//FUNÇÕES PARA CONSULTAR CLIENTE

static int csv_get_field(const char *linha, int alvo_idx, char *out, size_t outsz) {
    int idx = 0;
    const char *ini = linha;

    while (1) {
        const char *fim = strchr(ini, ';');
        size_t n;
        if (!fim) { // último campo da linha
            // corta CR/LF
            n = strcspn(ini, "\r\n");
            if (idx == alvo_idx) {
                if (n >= outsz) n = outsz - 1;
                memcpy(out, ini, n);
                out[n] = '\0';
                return 0;
            }
            return 1; // não chegou no alvo
        } else {
            // campo no meio
            if (idx == alvo_idx) {
                n = (size_t)(fim - ini);
                if (n >= outsz) n = outsz - 1;
                memcpy(out, ini, n);
                out[n] = '\0';
                return 0;
            }
            idx++;
            ini = fim + 1; // próximo campo (pode ser vazio)
        }
    }
}

void normaliza_excel_texto(char *s) {
    if (!s) return;

    // corta CR/LF
    s[strcspn(s, "\r\n")] = '\0';

    // loop até parar de mudar
    for (;;) {
        // trim espaços
        while (*s == ' ' || *s == '\t') memmove(s, s+1, strlen(s));
        size_t n = strlen(s);
        while (n > 0 && (s[n-1] == ' ' || s[n-1] == '\t')) s[--n] = '\0';

        size_t oldn = n;

        // remove apóstrofo inicial do Excel: 'texto
        if (n > 0 && s[0] == '\'') { memmove(s, s+1, n); n = strlen(s); }

        // padrão: ="123" (às vezes com aspas duplicadas internas)
        if (n >= 3 && s[0] == '=' && s[1] == '"') {
            // tira o ="
            memmove(s, s+2, n-1); // inclui '\0'
            n = strlen(s);
            // tira aspas finais se houver
            if (n > 0 && s[n-1] == '"') s[--n] = '\0';
        }

        // remove aspas externas "abc" (possível resto)
        n = strlen(s);
        if (n >= 2 && s[0] == '"' && s[n-1] == '"') {
            memmove(s, s+1, n-2);
            s[n-2] = '\0';
        }

        // desescapa aspas duplicadas do Excel: "" -> "
        char *p = s, *q = s;
        while (*p) {
            if (p[0] == '"' && p[1] == '"') { *q++ = '"'; p += 2; }
            else { *q++ = *p++; }
        }
        *q = '\0';

        if (strlen(s) == oldn) break; // nada mais mudou
    }
}

// mantém só dígitos (útil para CPF/CNPJ/telefone)
void somente_digitos(char *s) {
    if (!s) return;
    char *p = s, *q = s;
    while (*p) {
        if (*p >= '0' && *p <= '9') *q++ = *p;
        p++;
    }
    *q = '\0';
}

int cliente_buscar_por_codigo(const char *path, const char *codigo, Cliente *out) {
    if (!path || !codigo || !*codigo || !out) return 2;

    char codigo_norm[128];
    strncpy(codigo_norm, codigo, sizeof(codigo_norm));
    codigo_norm[sizeof(codigo_norm)-1] = '\0';
    normaliza_excel_texto(codigo_norm);

    FILE *f = fopen(path, "r");
    if (!f) return 1;

    char linha[4096];
    while (fgets(linha, sizeof(linha), f)) {
        if (strncmp(linha, "id;", 3) == 0 || strncmp(linha, "ID;", 3) == 0) continue;

        char campo_codigo[128];
        if (csv_get_field(linha, 2, campo_codigo, sizeof(campo_codigo)) != 0) continue;
        normaliza_excel_texto(campo_codigo);
        if (strcmp(campo_codigo, codigo_norm) != 0) continue;

        // bateu o código → preencher 'out'
        memset(out, 0, sizeof(*out));

        // GUARDE o código também
        strncpy(out->codigo, campo_codigo, sizeof(out->codigo));
        out->codigo[sizeof(out->codigo)-1] = '\0';

        char tmp[512];

        if (csv_get_field(linha, 0, tmp, sizeof(tmp)) == 0) out->id = atoi(tmp);

        if (csv_get_field(linha, 1, tmp, sizeof(tmp)) == 0) {
            normaliza_excel_texto(tmp);
            out->tipo = (strcmp(tmp,"PF")==0) ? Pessoa_Fisica : Pessoa_Juridica;
        }

        // CPF
        {
            char tmpcpf[128];
            if (csv_get_field(linha, 3, tmpcpf, sizeof(tmpcpf)) == 0) {
                normaliza_excel_texto(tmpcpf);
                somente_digitos(tmpcpf);
                strncpy(out->cpf, tmpcpf, sizeof(out->cpf));
                out->cpf[sizeof(out->cpf)-1] = '\0';
            }
        }

        // CNPJ
        {
            char tmpcnpj[128];
            if (csv_get_field(linha, 4, tmpcnpj, sizeof(tmpcnpj)) == 0) {
                normaliza_excel_texto(tmpcnpj);
                somente_digitos(tmpcnpj);
                strncpy(out->cnpj, tmpcnpj, sizeof(out->cnpj));
                out->cnpj[sizeof(out->cnpj)-1] = '\0';
            }
        }

        // Demais campos
        csv_get_field(linha, 5, out->razao_social, sizeof(out->razao_social));
        csv_get_field(linha, 6, out->nome_contato, sizeof(out->nome_contato));
        csv_get_field(linha, 7, out->nome,         sizeof(out->nome));
        csv_get_field(linha, 8, out->endereco,     sizeof(out->endereco));

        // Telefone
        {
            char tmptel[128];
            if (csv_get_field(linha, 9, tmptel, sizeof(tmptel)) == 0) {
                normaliza_excel_texto(tmptel);
                somente_digitos(tmptel);
                strncpy(out->telefone, tmptel, sizeof(out->telefone));
                out->telefone[sizeof(out->telefone)-1] = '\0';
            }
        }

        csv_get_field(linha, 10, out->email, sizeof(out->email));

        // Normaliza campos suscetíveis a ="..."/aspas/espaços
        normaliza_excel_texto(out->razao_social);
        normaliza_excel_texto(out->nome_contato);
        normaliza_excel_texto(out->nome);
        normaliza_excel_texto(out->endereco);
        normaliza_excel_texto(out->email);

        fclose(f);
        return 0; // achou
    }

    fclose(f);
    return 2; // não achou
}

//FUNÇÃO PARA REMOVER CLIENTE

int cliente_remover_por_codigo(const char *path, const char *codigo) {
    if (!path || !codigo || !*codigo) return 2;

    char codigo_norm[128];
    strncpy(codigo_norm, codigo, sizeof(codigo_norm));
    codigo_norm[sizeof(codigo_norm)-1] = '\0';
    normaliza_excel_texto(codigo_norm);

    FILE *in = fopen(path, "r");
    if (!in) return 1;
    FILE *out = fopen("cliente.tmp", "w");
    if (!out) { fclose(in); return 1; }

    char linha[4096];
    int removido = 0;

    while (fgets(linha, sizeof(linha), in)) {
        if (strncmp(linha, "id;", 3) == 0 || strncmp(linha, "ID;", 3) == 0) {
            fputs(linha, out);
            continue;
        }

        char campo_codigo[128];
        if (csv_get_field(linha, 2, campo_codigo, sizeof(campo_codigo)) == 0) {
            normaliza_excel_texto(campo_codigo); // CSV → normalizado
            if (strcmp(campo_codigo, codigo_norm) == 0) { // input → normalizado
                removido = 1;
                continue; 
            }
        }
        fputs(linha, out);
    }

    fclose(in);
    if (fclose(out) != 0) { remove("cliente.tmp"); return 3; }
    if (!removido) { remove("cliente.tmp"); return 2; }

    if (remove(path) != 0) { remove("cliente.tmp"); return 3; }
    if (rename("cliente.tmp", path) != 0) { return 3; }
    return 0;
}

//FUNÇÃO PARA LISTAR CLIENTES

int cliente_carregar_todos(const char *path, Cliente **vet, int *n) {
    if (!path || !vet || !n) return 2;
    *vet = NULL; *n = 0;

    FILE *f = fopen(path, "r");
    if (!f) return 1;

    int cap = 0;
    char linha[4096];

    while (fgets(linha, sizeof(linha), f)) {
        if (strncmp(linha, "id;", 3) == 0 || strncmp(linha, "ID;", 3) == 0) continue;

        if (*n >= cap) {
            cap = cap ? cap*2 : 64;
            Cliente *tmp = realloc(*vet, (size_t)cap * sizeof(Cliente));
            if (!tmp) { fclose(f); free(*vet); *vet=NULL; *n=0; return 3; }
            *vet = tmp;
        }

        Cliente *c = &(*vet)[*n];
        memset(c, 0, sizeof(*c));

        char tmp[512];

        // id
        if (csv_get_field(linha, 0, tmp, sizeof(tmp)) == 0) c->id = atoi(tmp);

        // tipo
        if (csv_get_field(linha, 1, tmp, sizeof(tmp)) == 0) {
            normaliza_excel_texto(tmp);
            c->tipo = (strcmp(tmp,"PF")==0) ? Pessoa_Fisica : Pessoa_Juridica;
        }

        // codigo (apenas normaliza, não tira dígitos)
        {
            char t[128] = {0};
            if (csv_get_field(linha, 2, t, sizeof(t)) == 0) {
                normaliza_excel_texto(t);
                strncpy(c->codigo, t, sizeof(c->codigo)-1);
                c->codigo[sizeof(c->codigo)-1] = '\0';
            }
        }

        // cpf (limpa ="..." e deixa só dígitos ANTES de copiar para a struct)
        {
            char t[128] = {0};
            if (csv_get_field(linha, 3, t, sizeof(t)) == 0) {
                normaliza_excel_texto(t);
                somente_digitos(t);
                strncpy(c->cpf, t, sizeof(c->cpf)-1);      // 11 + '\0'
                c->cpf[sizeof(c->cpf)-1] = '\0';
            }
        }

        // cnpj (idem)
        {
            char t[128] = {0};
            if (csv_get_field(linha, 4, t, sizeof(t)) == 0) {
                normaliza_excel_texto(t);
                somente_digitos(t);
                strncpy(c->cnpj, t, sizeof(c->cnpj)-1);    // 14 + '\0'
                c->cnpj[sizeof(c->cnpj)-1] = '\0';
            }
        }

        // razao_social
        {
            char t[256] = {0};
            if (csv_get_field(linha, 5, t, sizeof(t)) == 0) {
                normaliza_excel_texto(t);
                strncpy(c->razao_social, t, sizeof(c->razao_social)-1);
                c->razao_social[sizeof(c->razao_social)-1] = '\0';
            }
        }

        // nome_contato
        {
            char t[256] = {0};
            if (csv_get_field(linha, 6, t, sizeof(t)) == 0) {
                normaliza_excel_texto(t);
                strncpy(c->nome_contato, t, sizeof(c->nome_contato)-1);
                c->nome_contato[sizeof(c->nome_contato)-1] = '\0';
            }
        }

        // nome
        {
            char t[256] = {0};
            if (csv_get_field(linha, 7, t, sizeof(t)) == 0) {
                normaliza_excel_texto(t);
                strncpy(c->nome, t, sizeof(c->nome)-1);
                c->nome[sizeof(c->nome)-1] = '\0';
            }
        }

        // endereco
        {
            char t[256] = {0};
            if (csv_get_field(linha, 8, t, sizeof(t)) == 0) {
                normaliza_excel_texto(t);
                strncpy(c->endereco, t, sizeof(c->endereco)-1);
                c->endereco[sizeof(c->endereco)-1] = '\0';
            }
        }

        // telefone (limpa e deixa só dígitos ANTES de copiar)
        {
            char t[128] = {0};
            if (csv_get_field(linha, 9, t, sizeof(t)) == 0) {
                normaliza_excel_texto(t);
                somente_digitos(t);
                strncpy(c->telefone, t, sizeof(c->telefone)-1);
                c->telefone[sizeof(c->telefone)-1] = '\0';
            }
        }

        // email
        {
            char t[256] = {0};
            if (csv_get_field(linha, 10, t, sizeof(t)) == 0) {
                normaliza_excel_texto(t);
                strncpy(c->email, t, sizeof(c->email)-1);
                c->email[sizeof(c->email)-1] = '\0';
            }
        }

        (*n)++;
    }

    fclose(f);
    return 0;
}

void cliente_libera_lista(Cliente *vet) {
    free(vet);
}

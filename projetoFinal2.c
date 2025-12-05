#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include <ctype.h>

#ifndef CLIENTE_H
#define CLIENTE_H

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

#define MAX_PRODUTOS 100
#define MAX_LINHA 1024
#define PRODUTOS_CSV "Produtos.csv"
#define PEDIDOS_CSV "Pedidos.csv"

#ifndef PEDIDO_H
#define PEDIDO_H

#define maxDescricao 200
#define maxData 20
#define maxStatus 30

#define ARQUIVO_PEDIDOS   "Pedidos.csv"
#define ARQUIVO_CLIENTES  "Clientes.csv"

typedef struct {
    int codigoPedido;
    int codigoCliente;
    int codigoProduto;
    int quantidade;
    char data[maxData];
    char descricao[maxDescricao];
    double valorTotal;
    char status[maxStatus];
} Pedido;

/* Funções utilitárias e verificações */
void obterDataAtual(char *destino, int tamanho);
int codigoClienteJaExiste(const char *arquivoClientes, int code);
int codigoProdutoJaExiste(const char *arquivoProdutos, int code);
int codigoPedidoJaExiste(FILE* fpPe, int code);
int obterDadosProduto(const char *arquivoProdutos, int codigoProduto,
                      char *descricaoDestino, double *precoDestino);

/* Operações com pedidos */
void cadastrarPedido(FILE *fpPe, const char *arquivoClientes, const char *arquivoProdutos);
void listarPedidos(FILE *fpPe);
int consultarPedido(FILE *fpPe, const char *arquivoClientes);
FILE* atualizarPedido(FILE *fpPe, const char *arquivoClientes, const char *arquivoProdutos);
FILE* deletarPedido(FILE *fpPe, const char *arquivoClientes);

#endif

// strutc do produto
typedef struct {
    int id;
    char descricao[100];
    double preco;
    int estoque;
} Produto;

Produto produtos[MAX_PRODUTOS];
int totalProdutos = 0;

// precarregar as funções
void carregarProdutosCSV();
void salvarProdutosCSV();

int analisarProduto(int id); // retorna índice ou -1
int produtoExisteEmPedidos(int produtoId);

void inserirProduto();      // cadastra 
void listarProdutos();      // lista 
void removerProduto();      // remove 
void consultarProduto();    // consulta 

WINDOW *criar_janela_centralizada(int linhas, int colunas);
void submenu_produto(WINDOW *janela_pai);

// quebrar as linhas do fgets por precaução
static void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n == 0) return;
    if (s[n-1] == '\n') s[n-1] = '\0';
    if (n>1 && s[n-2] == '\r') s[n-2] = '\0';
}

// Implementaçao dos arquivos do produto
void carregarProdutosCSV() {
    FILE *f = fopen(PRODUTOS_CSV, "r");
    if (!f) {
        // arquivo começa vazio
        totalProdutos = 0;
        return;
    }

    char linha[MAX_LINHA];
    totalProdutos = 0;

    // Pular cabeçalho se existir
    if (fgets(linha, sizeof(linha), f)) {
        // Verifica se é cabeçalho
        if (strstr(linha, "id") != NULL || strstr(linha, "ID") != NULL) {
            // É cabeçalho, já pulamos
        } else {
            // Não é cabeçalho, voltar para o início
            rewind(f);
        }
    }

    while (fgets(linha, sizeof(linha), f) && totalProdutos < MAX_PRODUTOS) {
        trim_newline(linha);
        if (strlen(linha) == 0) continue;

        char *token;
        token = strtok(linha, ";");
        if (!token) continue;
        int id = atoi(token);

        token = strtok(NULL, ";");
        if (!token) continue;
        char descricao[100];
        strncpy(descricao, token, sizeof(descricao));
        descricao[sizeof(descricao)-1] = '\0';

        token = strtok(NULL, ";");
        if (!token) continue;
        double preco = atof(token);

        token = strtok(NULL, ";");
        if (!token) continue;
        int estoque = atoi(token);

        produtos[totalProdutos].id = id;
        strncpy(produtos[totalProdutos].descricao, descricao, sizeof(produtos[totalProdutos].descricao));
        produtos[totalProdutos].preco = preco;
        produtos[totalProdutos].estoque = estoque;
        totalProdutos++;
    }

    fclose(f);
}

void salvarProdutosCSV() {
    FILE *f = fopen(PRODUTOS_CSV, "w");
    if (!f) {
        // erro ao salvar
        return;
    }

    // Adiciona cabeçalho
    fprintf(f, "id;descricao;preco;estoque\n");
    
    // grava no arquivo do produto as coisas necessarias
    for (int i = 0; i < totalProdutos; i++) {
        fprintf(f, "%d;%s;%.2f;%d\n",
            produtos[i].id,
            produtos[i].descricao,
            produtos[i].preco,
            produtos[i].estoque);
    }

    fclose(f);
}

// funções pra dar um help (ultilitarias)
int analisarProduto(int id) {
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].id == id) return i;
    }
    return -1;
}

// verifica se produto aparece em algum pedido no arquivo 
int produtoExisteEmPedidos(int produtoId) {
    FILE *f = fopen(PEDIDOS_CSV, "r");
    if (!f) return 0; // se arquivo não existir, nenhum pedido vai ser false

    char linha[MAX_LINHA];
    char busc[64];
    // procuraremos por padrão "produtoId:" dentro do campo de itens
    snprintf(busc, sizeof(busc), "%d:", produtoId);

    while (fgets(linha, sizeof(linha), f)) {
        // formato esperado: id;clienteId;data;total;produtoId1:qt|produtoId2:qt|...
        // buscamos apenas se a substring busc aparecer na linha
        if (strstr(linha, busc) != NULL) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

/* Carrega produtos para verificação - NOVA FUNÇÃO */
int carregarProdutosParaPedido() {
    FILE *f = fopen("Produtos.csv", "r");
    if (!f) {
        // Tenta criar arquivo vazio se não existir
        f = fopen("Produtos.csv", "w");
        if (f) {
            fprintf(f, "id;descricao;preco;estoque\n");
            fclose(f);
        }
        return 0;
    }
    fclose(f);
    return 1;
}

// cadastrar
void inserirProduto() {
    clear();
    echo();

    mvprintw(1, 2, "===== CADASTRAR PRODUTO =====");
    mvprintw(3, 2, "ID do produto: ");
    int id;
    scanw("%d", &id);

    if (analisarProduto(id) != -1) {
        mvprintw(5, 2, "ERRO: Codigo ja existe!");
        mvprintw(7, 2, "Pressione qualquer tecla para voltar...");
        noecho();
        getch();
        return;
    }

    Produto p;
    p.id = id;

    mvprintw(4, 2, "Descricao: ");
    // limpar buffer antes
    int y,x; getyx(stdscr,y,x);
    move(4, 13);
    char buf[100];
    getnstr(buf, sizeof(buf)-1);
    trim_newline(buf);
    strncpy(p.descricao, buf, sizeof(p.descricao));
    p.descricao[sizeof(p.descricao)-1] = '\0';

    mvprintw(5, 2, "Preco: ");
    scanw("%lf", &p.preco);

    mvprintw(6, 2, "Estoque: ");
    scanw("%d", &p.estoque);

    if (totalProdutos < MAX_PRODUTOS) {
        produtos[totalProdutos++] = p;
        
        // ADICIONAR ESTA LINHA: SALVAR NO ARQUIVO CSV
        salvarProdutosCSV();
        
        mvprintw(8, 2, "Produto cadastrado com sucesso!");
    } else {
        mvprintw(8, 2, "ERRO: capacidade maxima de produtos atingida.");
    }

    noecho();
    mvprintw(10, 2, "Pressione qualquer tecla para continuar...");
    getch();
}

// lista de produtos
void listarProdutos() {
    clear();
    mvprintw(1, 2, "===== LISTA DE PRODUTOS =====");

    if (totalProdutos == 0) {
        mvprintw(3, 2, "Nenhum produto cadastrado.");
        mvprintw(5, 2, "Pressione qualquer tecla...");
        getch();
        return;
    }

    int linha = 3;
    for (int i = 0; i < totalProdutos; i++) {
        mvprintw(linha++, 2, "ID: %d | %s | Preco: R$ %.2f | Estoque: %d",
            produtos[i].id, produtos[i].descricao, produtos[i].preco, produtos[i].estoque);

        // listinha de produtos com paginaçãozinha
        if (linha >= LINES - 2) {
            mvprintw(LINES-2, 2, "-- Mais (pressione tecla) --");
            getch();
            clear();
            mvprintw(1,2,"===== LISTA DE PRODUTOS =====");
            linha = 3;
        }
    }
    mvprintw(LINES-2, 2, "Pressione qualquer tecla...");
    getch();
}

// consultarProduto 
void consultarProduto() {
    clear();
    echo();

    mvprintw(1, 2, "===== CONSULTAR PRODUTO =====");
    mvprintw(3, 2, "ID do produto: ");
    int id;
    scanw("%d", &id);

    int pos = analisarProduto(id);
    if (pos == -1) {
        mvprintw(5, 2, "ERRO: Produto nao existe!");
        mvprintw(7, 2, "Pressione qualquer tecla...");
        noecho();
        getch();
        return;
    }

    Produto p = produtos[pos];
    mvprintw(5, 2, "ID      : %d", p.id);
    mvprintw(6, 2, "Descricao: %s", p.descricao);
    mvprintw(7, 2, "Preco   : R$ %.2f", p.preco);
    mvprintw(8, 2, "Estoque : %d", p.estoque);

    noecho();
    mvprintw(10, 2, "Pressione qualquer tecla...");
    getch();
}

// removerProduto 
void removerProduto() {
    clear();
    echo();

    mvprintw(1, 2, "===== REMOVER PRODUTO =====");
    mvprintw(3, 2, "ID do produto: ");
    int id;
    scanw("%d", &id);

    int pos = analisarProduto(id);
    if (pos == -1) {
        mvprintw(5, 2, "ERRO: Produto nao existe!");
        mvprintw(7, 2, "Pressione qualquer tecla...");
        noecho();
        getch();
        return;
    }

    if (produtoExisteEmPedidos(id)) {
        mvprintw(5, 2, "ERRO: Produto consta em pedidos! Nao pode ser removido.");
        mvprintw(7, 2, "Pressione qualquer tecla...");
        noecho();
        getch();
        return;
    }

    mvprintw(5, 2, "Confirmar exclusao? (1 - Sim / 0 - Nao): ");
    int opc;
    scanw("%d", &opc);

    if (opc == 1) {
        for (int i = pos; i < totalProdutos - 1; i++) produtos[i] = produtos[i+1];
        totalProdutos--;
        
        // ADICIONAR ESTA LINHA: SALVAR NO ARQUIVO CSV
        salvarProdutosCSV();
        
        mvprintw(7, 2, "Produto removido com sucesso.");
    } else {
        mvprintw(7, 2, "Remocao cancelada.");
    }

    noecho();
    mvprintw(9, 2, "Pressione qualquer tecla...");
    getch();
}

WINDOW *criar_janela_centralizada(int linhas, int colunas) {
    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);
    int starty = (ymax - linhas) / 2;
    int startx = (xmax - colunas) / 2;
    WINDOW *w = newwin(linhas, colunas, starty, startx);
    keypad(w, TRUE);
    return w;
}

void submenu_produto(WINDOW *janela_pai) {
    // ADICIONAR ESTA LINHA: CARREGAR PRODUTOS DO ARQUIVO
    carregarProdutosCSV();
    
    char opcoes[4][20] = {
        "Cadastrar Produto",
        "Listar Produtos",
        "Remover Produto",
        "Consultar Produto"
    };

    char titulo[100] = "====== Menu Produto ======";
    int tecla, seletor = 0;
    int xmax, ymax;
    getmaxyx(stdscr, ymax, xmax);

    WINDOW *janela = criar_janela_centralizada(10, xmax - 60);
    box(janela, 0, 0);

    start_color();
    init_pair(1, COLOR_CYAN, COLOR_WHITE);    
    init_pair(2, COLOR_WHITE, COLOR_CYAN);     

    wbkgd(janela, COLOR_PAIR(1));
    keypad(janela, TRUE);

    while(1) {
        werase(janela);
        box(janela, 0, 0);

        wattron(janela, COLOR_PAIR(2) | A_BOLD);
        mvwprintw(janela, 1, (xmax-60)/2 - 14, "%s", titulo);
        wattroff(janela, COLOR_PAIR(2) | A_BOLD);

        for(int i = 0; i < 4; i++) {
            if(i == seletor) {
                wattron(janela, COLOR_PAIR(2) | A_BOLD); 
                mvwprintw(janela, i + 3, 5, "> %s", opcoes[i]);
                wattroff(janela, COLOR_PAIR(2) | A_BOLD);
            } else {
                wattron(janela, COLOR_PAIR(1)); 
                mvwprintw(janela, i + 3, 7, "%s", opcoes[i]);
                wattroff(janela, COLOR_PAIR(1));
            }
        }

        // Instruções - mesma cor que o menu principal
        wattron(janela, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(janela, 8, 5, "ENTER: Selecionar | ESC: Voltar");
        wattroff(janela, COLOR_PAIR(1) | A_BOLD);

        wrefresh(janela);
        tecla = wgetch(janela);

        switch(tecla) {
            case KEY_UP:
                seletor = (seletor - 1 + 4) % 4;
                break;
            case KEY_DOWN:
                seletor = (seletor + 1) % 4;
                break;
            case '\n':
            case KEY_ENTER:
                // chamamos diretamente as funções ncurses implementadas
                if (seletor == 0) {
                    // Cadastrar
                    delwin(janela);
                    inserirProduto();
                    
                    // ADICIONAR ESTA LINHA: RECARREGAR PRODUTOS APÓS CADASTRAR
                    carregarProdutosCSV();
                    
                    janela = criar_janela_centralizada(10, xmax - 60);
                    box(janela,0,0);
                    wbkgd(janela, COLOR_PAIR(1));
                    keypad(janela, TRUE);
                } else if (seletor == 1) {
                    // Listar
                    delwin(janela);
                    listarProdutos();
                    janela = criar_janela_centralizada(10, xmax - 60);
                    box(janela,0,0);
                    wbkgd(janela, COLOR_PAIR(1));
                    keypad(janela, TRUE);
                } else if (seletor == 2) {
                    // Remover
                    delwin(janela);
                    removerProduto();
                    
                    // ADICIONAR ESTA LINHA: RECARREGAR PRODUTOS APÓS REMOVER
                    carregarProdutosCSV();
                    
                    janela = criar_janela_centralizada(10, xmax - 60);
                    box(janela,0,0);
                    wbkgd(janela, COLOR_PAIR(1));
                    keypad(janela, TRUE);
                } else if (seletor == 3) {
                    // Consultar
                    delwin(janela);
                    consultarProduto();
                    janela = criar_janela_centralizada(10, xmax - 60);
                    box(janela,0,0);
                    wbkgd(janela, COLOR_PAIR(1));
                    keypad(janela, TRUE);
                }
                break;
            case 27: // ESC
                // ADICIONAR ESTA LINHA: SALVAR ANTES DE SAIR
                salvarProdutosCSV();
                delwin(janela);
                touchwin(janela_pai);
                wrefresh(janela_pai);
                return;
        }
    }
    delwin(janela);
}

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

/* Funções */
void obterDataAtual(char *destino, int tamanho){
    time_t agora = time(NULL);
    struct tm *d = localtime(&agora);

    // Informar Data
    sprintf(destino, "%d/%d/%d",
            d->tm_mday,
            d->tm_mon + 1,
            d->tm_year + 1900);
}

/* Verifica se um código de cliente existe */
int codigoClienteJaExiste(const char *arquivoClientes, int code){
    FILE *fp = fopen(arquivoClientes, "r");
    if(!fp) return 0;

    char linha[512];
    while(fgets(linha, sizeof(linha), fp)){
        int id;
        if(sscanf(linha, "%d ;", &id) == 1){
            if(id == code){
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

/*  Se um código de produto já existe */
/* Se um código de produto já existe - CORRIGIDA */
int codigoProdutoJaExiste(const char *arquivoProdutos, int code){
    FILE *fp = fopen(arquivoProdutos, "r");
    if(!fp) return 0;

    char linha[512];
    while(fgets(linha, sizeof(linha), fp)){
        int id;
        // Formato CORRETO: id;descricao;preco;estoque (SEM espaços)
        if(sscanf(linha, "%d;", &id) == 1){  // Mudado de "%d ;" para "%d;"
            if(id == code){
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

/* Obtém descrição e preço de um produto pelo arquivo de produtos.
  
     codigoProduto ; descricao ; preco ; estoque
*/
/* Obtém descrição e preço de um produto - CORRIGIDA */
int obterDadosProduto(const char *arquivoProdutos, int codigoProduto, 
                      char *descricaoDestino, double *precoDestino){
    FILE *fp = fopen(arquivoProdutos, "r");
    if(!fp) return 0;

    char linha[1024];
    while(fgets(linha, sizeof(linha), fp)){
        int id;
        char desc[200];
        double preco;
        int est;

        // Formato CORRETO: id;descricao;preco;estoque (SEM espaços)
        if(sscanf(linha, "%d;%[^;];%lf;%d", &id, desc, &preco, &est) == 4){ // Mudado
            if(id == codigoProduto){
                strcpy(descricaoDestino, desc);
                *precoDestino = preco;
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}

/* Verifica se já tem um codigo com o produto.

     codigoPedido ; codigoCliente ; codigoProduto ; quantidade ; data ; descricao ; valorTotal ; status
*/
int codigoPedidoJaExiste(FILE* fpPe, int code){
    if (!fpPe) return 0;
    rewind(fpPe);
    char linha[1024];
    while(fgets(linha, sizeof(linha), fpPe)){
        int id;
        if(sscanf(linha, "%d ;", &id) == 1){
            if(id == code) return 1;
        }
    }
    return 0;
}

/* CADASTRAR PEDIDO */
void cadastrarPedido(FILE *fpPe, const char *arquivoClientes, const char *arquivoProdutos){

    clear();
    echo();

    Pedido p;
    double precoUnit;

    mvprintw(0,0,"===== CADASTRO DE PEDIDO =====\n");

    mvprintw(2,0,"Código do Pedido: ");
    scanw("%d", &p.codigoPedido);

    if(codigoPedidoJaExiste(fpPe, p.codigoPedido)){
        mvprintw(5,0,"Erro: código já existe!");
        getch();
        return;
    }

    mvprintw(3,0,"Código do Cliente: ");
    scanw("%d", &p.codigoCliente);

    if(!codigoClienteJaExiste(arquivoClientes, p.codigoCliente)){
        mvprintw(6,0,"Erro: cliente não existe!");
        getch();
        return;
    }

    mvprintw(4,0,"Código do Produto: ");
    scanw("%d", &p.codigoProduto);

    if(!codigoProdutoJaExiste(arquivoProdutos, p.codigoProduto)){
        mvprintw(7,0,"Erro: produto não existe!");
        getch();
        return;
    }

    mvprintw(5,0,"Quantidade: ");
    scanw("%d", &p.quantidade);

    /* obtém descrição e preço do produto */
    if (!obterDadosProduto(arquivoProdutos, p.codigoProduto, p.descricao, &precoUnit)) {
        mvprintw(8,0,"Erro: falha ao obter dados do produto!");
        getch();
        return;
    }

    p.valorTotal = p.quantidade * precoUnit;
    obterDataAtual(p.data, maxData);
    strcpy(p.status, "Pendente");

    /* grava no arquivo formato lido por listar/consultar/atualizar */
    fprintf(fpPe, "%d ; %d ; %d ; %d ; %s ; %s ; %.2f ; %s\n",
            p.codigoPedido, p.codigoCliente, p.codigoProduto, p.quantidade,
            p.data, p.descricao, p.valorTotal, p.status);

    fflush(fpPe);

    mvprintw(10,0,"Pedido cadastrado com sucesso!");
    getch();
}

/* LISTAR */
void listarPedidos(FILE *fpPe){
    clear();
    rewind(fpPe);

    Pedido p;
    printw("===== LISTA DE PEDIDOS =====\n\n");

    /* leitura com fscanf usando %[^;] para com espaços; s */
    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %s\n",
                 &p.codigoPedido, &p.codigoCliente, &p.codigoProduto, &p.quantidade,
                 p.data, p.descricao, &p.valorTotal, p.status) == 8){

        printw("Cod: %d | Cliente: %d | Prod: %d | Qtd: %d | Total: %.2f | Status: %s\n",
               p.codigoPedido, p.codigoCliente, p.codigoProduto,
               p.quantidade, p.valorTotal, p.status);
    }

    getch();
}

/* CONSULTAR */
int consultarPedido(FILE *fpPe, const char *arquivoClientes){
    clear();
    echo();

    int codigoCliente;
    mvprintw(0,0,"Código do Cliente: ");
    scanw("%d", &codigoCliente);

    if(!codigoClienteJaExiste(arquivoClientes, codigoCliente)){
        mvprintw(3,0,"Cliente não existe!");
        getch();
        return 0;
    }

    rewind(fpPe);

    Pedido p;
    printw("\nPedidos do cliente:\n\n");

    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %s\n",
                 &p.codigoPedido, &p.codigoCliente, &p.codigoProduto, &p.quantidade,
                 p.data, p.descricao, &p.valorTotal, p.status) == 8){
        if(p.codigoCliente == codigoCliente){
            printw("Pedido %d | Produto %d | Total %.2f\n",
                   p.codigoPedido, p.codigoProduto, p.valorTotal);
        }
    }

    getch();
    return 1;
}

/* ATUALIZAR */
FILE* atualizarPedido(FILE *fpPe, const char *arquivoClientes, const char *arquivoProdutos){

    clear();
    echo();

    int codigoCliente, codigoPedido;
    mvprintw(0,0,"Código do Cliente: ");
    scanw("%d", &codigoCliente);

    if(!codigoClienteJaExiste(arquivoClientes, codigoCliente)){
        mvprintw(3,0,"Cliente não existe!");
        getch();
        return fpPe;
    }

    mvprintw(1,0,"Código do Pedido: ");
    scanw("%d", &codigoPedido);

    FILE *temp = fopen("Pedidos_temp.csv", "w");
    if(!temp){
        mvprintw(3,0,"Erro: não foi possível criar arquivo temporário!");
        getch();
        return fpPe;
    }

    rewind(fpPe);

    Pedido p;
    int encontrado = 0;

    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %s\n",
                 &p.codigoPedido, &p.codigoCliente, &p.codigoProduto, &p.quantidade,
                 p.data, p.descricao, &p.valorTotal, p.status) == 8){

        if(p.codigoPedido == codigoPedido && p.codigoCliente == codigoCliente){

            encontrado = 1;
            clear();

            int opc;
            mvprintw(0,0,"1) Alterar Produto");
            mvprintw(1,0,"2) Alterar Quantidade");
            mvprintw(2,0,"3) Alterar Status");
            mvprintw(3,0,"-> ");
            scanw("%d",&opc);

            double precoU;

            if(opc == 1){
                mvprintw(5,0,"Novo codigo produto: "); scanw("%d",&p.codigoProduto);
                if (!obterDadosProduto(arquivoProdutos, p.codigoProduto, p.descricao, &precoU)) {
                    mvprintw(6,0,"Produto inválido!");
                    getch();
                } else {
                    p.valorTotal = p.quantidade * precoU;
                }
            }
            else if(opc == 2){
                mvprintw(5,0,"Nova quantidade: "); scanw("%d",&p.quantidade);
                if (!obterDadosProduto(arquivoProdutos, p.codigoProduto, p.descricao, &precoU)) {
                    mvprintw(6,0,"Erro ao obter dados do produto!");
                    getch();
                } else {
                    p.valorTotal = p.quantidade * precoU;
                }
            }
            else if(opc == 3){
                mvprintw(5,0,"Novo status: "); scanw("%s", p.status);
            }

            obterDataAtual(p.data, maxData);
        }

        /* escreve (alterado ou não) na linha temporária */
        fprintf(temp,"%d ; %d ; %d ; %d ; %s ; %s ; %.2f ; %s\n",
                p.codigoPedido,p.codigoCliente,p.codigoProduto,p.quantidade,
                p.data,p.descricao,p.valorTotal,p.status);
    }

    fclose(fpPe);
    fclose(temp);

    /* substitui o arquivo original */
    remove(ARQUIVO_PEDIDOS);
    rename("Pedidos_temp.csv", ARQUIVO_PEDIDOS);

    fpPe = fopen(ARQUIVO_PEDIDOS, "a+");
    if(!fpPe){
        /* se falhar, avise e retorne NULL (ou NULL-like comportamento) */
        clear();
        mvprintw(1,0,"Erro ao reabrir arquivo de pedidos!");
        getch();
        return NULL;
    }

    clear();
    if(encontrado)
        mvprintw(1,0,"Pedido atualizado!");
    else
        mvprintw(1,0,"Pedido não encontrado!");

    getch();
    return fpPe;
}

/* DELETAR */
FILE* deletarPedido(FILE *fpPe, const char *arquivoClientes){

    clear();
    echo();

    int codigoCliente, codigoPedido;

    mvprintw(0,0,"Código do Cliente: ");
    scanw("%d",&codigoCliente);

    if(!codigoClienteJaExiste(arquivoClientes, codigoCliente)){
        mvprintw(3,0,"Cliente não existe!");
        getch();
        return fpPe;
    }

    mvprintw(1,0,"Código do Pedido: ");
    scanw("%d",&codigoPedido);

    FILE *temp = fopen("Pedidos_temp.csv","w");
    if(!temp){
        mvprintw(3,0,"Erro: não foi possível criar arquivo temporário!");
        getch();
        return fpPe;
    }

    rewind(fpPe);
    Pedido p;
    int deletado = 0;

    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %s\n",
                 &p.codigoPedido, &p.codigoCliente, &p.codigoProduto, &p.quantidade,
                 p.data,p.descricao,&p.valorTotal,p.status) == 8){

        if(p.codigoPedido == codigoPedido && p.codigoCliente == codigoCliente){
            deletado = 1;
        } else {
            fprintf(temp,"%d ; %d ; %d ; %d ; %s ; %s ; %.2f ; %s\n",
                p.codigoPedido,p.codigoCliente,p.codigoProduto,p.quantidade,
                p.data,p.descricao,p.valorTotal,p.status);
        }
    }

    fclose(fpPe);
    fclose(temp);

    remove(ARQUIVO_PEDIDOS);
    rename("Pedidos_temp.csv", ARQUIVO_PEDIDOS);

    fpPe = fopen(ARQUIVO_PEDIDOS, "a+");
    if(!fpPe){
        clear();
        mvprintw(1,0,"Erro ao reabrir arquivo de pedidos!");
        getch();
        return NULL;
    }

    clear();
    if(deletado)
        mvprintw(2,0,"Pedido apagado com sucesso!");
    else
        mvprintw(2,0,"Pedido não encontrado!");

    getch();
    return fpPe;
}

void submenu_pedido() {
    // VERIFICAR SE ARQUIVOS EXISTEM ANTES DE ABRIR
    FILE *fp_produtos = fopen("Produtos.csv", "r");
    if (!fp_produtos) {
        clear();
        mvprintw(0, 0, "AVISO: Arquivo de produtos nao encontrado!");
        mvprintw(1, 0, "Cadastre produtos primeiro.");
        mvprintw(2, 0, "Pressione qualquer tecla...");
        getch();
        return;
    }
    fclose(fp_produtos);
    
    FILE *fp_clientes = fopen("cliente.csv", "r");
    if (!fp_clientes) {
        clear();
        mvprintw(0, 0, "AVISO: Arquivo de clientes nao encontrado!");
        mvprintw(1, 0, "Cadastre clientes primeiro.");
        mvprintw(2, 0, "Pressione qualquer tecla...");
        getch();
        return;
    }
    fclose(fp_clientes);

    FILE *fpPe = fopen("Pedidos.csv", "a+");
    if(!fpPe){
        clear();
        mvprintw(0,0,"Erro ao abrir arquivo de pedidos!");
        getch();
        return;
    }

    char opcoes[5][30] = {
        "Cadastrar Pedido",
        "Listar Pedidos",
        "Remover Pedido",
        "Consultar Pedido",
        "Voltar"
    };

    int xmax, ymax;
    char titulo[1][100] = {"====== Menu Pedido ======"};
    int tecla, seletor = 0;
    getmaxyx(stdscr, ymax, xmax);

    WINDOW *jan = newwin(20, xmax - 10, ymax - 25, 5);
    box(jan, 0, 0);

    start_color();
    init_pair(1, COLOR_CYAN, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_CYAN);

    wbkgd(jan, COLOR_PAIR(1));
    keypad(stdscr, TRUE);

    while(1) {
        werase(jan);
        box(jan, 0, 0);

        wattron(jan, COLOR_PAIR(1));
        mvwprintw(jan, 1, 40, "%s", titulo[0]);
        wattroff(jan, COLOR_PAIR(1));

        for(int i = 0; i < 5; i++) {
            if(i == seletor) {
                wattron(jan, COLOR_PAIR(2));
                mvwprintw(jan, i + 3, 3, "%s", opcoes[i]);
                wattroff(jan, COLOR_PAIR(2));
            } else {
                wattron(jan, COLOR_PAIR(1));
                mvwprintw(jan, i + 3, 3, "%s", opcoes[i]);
                wattroff(jan, COLOR_PAIR(1));
            }
        }

        wrefresh(jan);
        tecla = getch();

        switch(tecla) {
            case KEY_UP:
                seletor--;
                if(seletor < 0) seletor = 4;
                break;

            case KEY_DOWN:
                seletor++;
                if(seletor > 4) seletor = 0;
                break;

            case '\n':
            case KEY_ENTER:
                if(seletor == 0) {
                    clear();
                    // NOME CORRETO DO ARQUIVO: "Produtos.csv" (com P maiúsculo)
                    cadastrarPedido(fpPe, "cliente.csv", "Produtos.csv");
                }

                if(seletor == 1) {
                    clear();
                    listarPedidos(fpPe);
                }

                if(seletor == 2) {
                    clear();
                    fpPe = deletarPedido(fpPe, "cliente.csv");
                }

                if(seletor == 3) {
                    clear();
                    consultarPedido(fpPe, "cliente.csv");
                }

                if(seletor == 4) {
                    delwin(jan);
                    fclose(fpPe);
                    return;
                }
                break;
        }
    }
}

//INTERFACES PARA CLIENTE
static int ui_getline_or_esc(WINDOW *win, int y, int x, char *buf, int maxlen) {
    int ch;
    wmove(win, y, x);
    wrefresh(win);

    noecho();
    ch = wgetch(win);
    if (ch == 27) {
        return -1;
    }
    ungetch(ch);
    echo();

    wgetnstr(win, buf, maxlen-1);

    size_t len = strcspn(buf, "\r\n");
    buf[len] = '\0';
    return 0;
}

int ui_cadastrar_cliente(WINDOW *janela) {

    int xmax,ymax;
    getmaxyx(janela, ymax, xmax);

    Cliente c = (Cliente){0};

    // buffers do registro
    char cpf[12], cnpj_digitado[64], cnpj_validado[15], nome[100], end[200], tel[20], email[100], razao_social[100],
         nome_contato[100];

    // desenha a tela “limpa”
    werase(janela);
    box(janela, 0, 0);
    mvwprintw(janela, 1, 3, "====== Cadastrar Cliente ======");
    mvwprintw(janela, ymax-2, 3, "[ESC] cancelar");
    wrefresh(janela);

    // vamos digitar no modo eco + cursor visível
    echo();
    curs_set(1);
    keypad(janela, TRUE);

    // PEDIR E VALIDAR CÓDIGO — ANTES DO CPF/CNPJ
    for (;;) {
        mvwprintw(janela, 3, 3, "Codigo do cliente: %-30s", "");
        if (ui_getline_or_esc(janela, 3, 22, c.codigo, sizeof(c.codigo)) < 0) {
            noecho(); curs_set(0);
            werase(janela); box(janela,0,0);
            mvwprintw(janela, 3, 3, "Cadastro cancelado.");
            wrefresh(janela); wgetch(janela);
            return 9;
        }

        if (validar_codigo_cliente(c.codigo) != 0) {
            mvwprintw(janela, 4, 3, "Codigo invalido (vazio, com espacos ou ';').");
            wrefresh(janela); wgetch(janela);
            mvwprintw(janela, 4, 3, "%-70s", "");
            continue;
        }

        int existe = cliente_codigo_existe_no_csv("cliente.csv", c.codigo);
        if (existe < 0) {
            mvwprintw(janela, 4, 3, "Erro ao abrir arquivo.");
            wrefresh(janela); wgetch(janela);
            return 6;
        }
        if (existe == 1) {
            mvwprintw(janela, 4, 3, "Codigo ja existe. Tente outro.");
            wrefresh(janela); wgetch(janela);
            mvwprintw(janela, 4, 3, "%-70s", "");
            continue;
        }
        break; // código válido e único
    }

    /* === Escolher tipo de cliente (PF/PJ) === */
    int seletor_tipo = 0; // 0 = PF, 1 = PJ
    for (;;) {
        // desenha as opções
        mvwprintw(janela, 5, 3, "Tipo: ");
        if (seletor_tipo == 0) {
            wattron(janela, A_REVERSE); mvwprintw(janela, 5, 10, " PF "); wattroff(janela, A_REVERSE);
            mvwprintw(janela, 5, 15, " PJ ");
        } else {
            mvwprintw(janela, 5, 10, " PF ");
            wattron(janela, A_REVERSE); mvwprintw(janela, 5, 15, " PJ "); wattroff(janela, A_REVERSE);
        }
        wrefresh(janela);

        int t = wgetch(janela);
        if (t == KEY_LEFT || t == KEY_RIGHT) {
            seletor_tipo = 1 - seletor_tipo; // alterna
        } else if (t == '\n' || t == KEY_ENTER) {
            c.tipo = (seletor_tipo == 0) ? Pessoa_Fisica : Pessoa_Juridica;
            break;
        } else if (t == 27) { // ESC
            noecho(); curs_set(0);
            werase(janela); box(janela,0,0);
            mvwprintw(janela, 3, 3, "Cadastro cancelado.");
            wrefresh(janela); wgetch(janela);
            return 9;
        }
    }

    werase(janela);
    box(janela, 0, 0);
    mvwprintw(janela, 1, 3, "====== Cadastrar Cliente ======");
    mvwprintw(janela, ymax-2, 3, "[ESC] cancelar");
    wrefresh(janela);
    
    /* === Documento conforme o tipo === */
    if (c.tipo == Pessoa_Fisica) {
        // CPF (loop até ficar válido)
        for (;;) {
            mvwprintw(janela, 7, 3, "CPF (11 digitos): %-20s", "");
            if (ui_getline_or_esc(janela, 7, 22, cpf, sizeof(cpf)) < 0) {
                noecho(); curs_set(0);
                werase(janela); box(janela,0,0);
                mvwprintw(janela, 3, 3, "Cadastro cancelado.");
                wrefresh(janela); wgetch(janela);
                return 9;
            }
        
            if (cadastro_cpf(cpf, cpf) != 0) {
            mvwprintw(janela, 8, 3, "CPF invalido. Tente novamente.");
            wrefresh(janela); wgetch(janela);
            mvwprintw(janela, 8, 3, "%-40s", "");
            continue;
            }

            /* >>> NOVO: checar duplicidade no CSV */
            int existe = cliente_cpf_existe_no_csv("cliente.csv", cpf);
            if (existe < 0) {
                mvwprintw(janela, 8, 3, "Erro ao abrir arquivo.");
                wrefresh(janela); wgetch(janela);
                return 6;
            }

            if (existe == 1) {
                mvwprintw(janela, 8, 3, "CPF ja cadastrado. Digite outro.");
                wrefresh(janela); wgetch(janela);
                mvwprintw(janela, 8, 3, "%-40s", "");
                continue;  // volta ao mesmo campo, como pede o requisito
            }

            strcpy(c.cpf, cpf);
            break;
        }
            
    } else {
        // CNPJ (valida CNPJ)
        for (;;) {
            mvwprintw(janela, 7, 3, "CNPJ (14 digitos): %-20s", "");
            if (ui_getline_or_esc(janela, 7, 24, cnpj_digitado, sizeof(cnpj_digitado)) < 0) {
                noecho(); curs_set(0);
                werase(janela); box(janela,0,0);
                mvwprintw(janela, 3, 3, "Cadastro cancelado.");
                wrefresh(janela); wgetch(janela);
                return 9;
            }

            if (cadastro_cnpj(cnpj_digitado, cnpj_validado) != 0) {
            mvwprintw(janela, 8, 3, "CNPJ invalido. Tente novamente.");
            wrefresh(janela); wgetch(janela);
            mvwprintw(janela, 8, 3, "%-40s", "");
            continue;
            }

            /* >>> NOVO: checar duplicidade no CSV */
            int existe = cliente_cnpj_existe_no_csv("cliente.csv", cnpj_validado);

            if (existe < 0) {
                mvwprintw(janela, 8, 3, "Erro ao abrir arquivo.");
                wrefresh(janela); wgetch(janela);
                return 6;
            }

            if (existe == 1) {
                mvwprintw(janela, 8, 3, "CNPJ ja cadastrado. Digite outro.");
                wrefresh(janela); wgetch(janela);
                mvwprintw(janela, 8, 3, "%-40s", "");
                continue;  // volta ao mesmo campo
            }

            strcpy(c.cnpj, cnpj_validado);
            break;
        }
    }

    // Nome
    if(c.tipo == Pessoa_Fisica) {
        for (;;) {
            mvwprintw(janela, 8, 3, "Nome: %-60s", "");
            if (ui_getline_or_esc(janela, 8, 9, nome, sizeof(nome)) < 0) { // << ESC
                noecho(); curs_set(0);
                werase(janela); box(janela,0,0);
                mvwprintw(janela, 3, 3, "Cadastro cancelado.");
                wrefresh(janela); wgetch(janela);
                return 9;
            }

            if (cadastro_nome(nome) == 0) break;
            mvwprintw(janela, 9, 3, "Nome vazio/invalido.");
            wrefresh(janela);
            wgetch(janela);
            mvwprintw(janela, 9, 3, "%-40s", "");
        }
    } else {
            for (;;) {
                mvwprintw(janela, 8, 3, "Razão social: %-60s", "");
                if (ui_getline_or_esc(janela, 8, 18, razao_social, sizeof(razao_social)) < 0) { // << ESC
                    noecho(); curs_set(0);
                    werase(janela); box(janela,0,0);
                    mvwprintw(janela, 3, 3, "Cadastro cancelado.");
                    wrefresh(janela); wgetch(janela);
                    return 9;
                }

                if (cadastro_nome(razao_social) == 0) break;
                mvwprintw(janela, 9, 3, "Nome vazio/invalido.");
                wrefresh(janela);
                wgetch(janela);
                mvwprintw(janela, 9, 3, "%-40s", "");
        }

            for (;;) {
                mvwprintw(janela, 10, 3, "Nome de contato: %-60s", "");
                if (ui_getline_or_esc(janela, 10, 20, nome_contato, sizeof(nome_contato)) < 0) { // ESC
                    noecho(); curs_set(0);
                    werase(janela); box(janela,0,0);
                    mvwprintw(janela, 3, 3, "Cadastro cancelado.");
                    wrefresh(janela); wgetch(janela);
                    return 9;
                }
                if (cadastro_nome(nome_contato) == 0) break;   // mesma validação
                mvwprintw(janela, 11, 3, "Nome de contato invalido.");
                wrefresh(janela); wgetch(janela);
                mvwprintw(janela, 11, 3, "%-40s", "");
        }
    }

    // Endereco
    for (;;) {
        mvwprintw(janela, 11, 3, "Endereco: %-60s", "");
        if (ui_getline_or_esc(janela, 11, 13, end, sizeof(end)) < 0) {  // << ESC
            noecho(); curs_set(0);
            werase(janela); box(janela,0,0);
            mvwprintw(janela, 3, 3, "Cadastro cancelado.");
            wrefresh(janela); wgetch(janela);
            return 9;
        }

        if (cadastro_endereco(end) == 0) break;
        mvwprintw(janela,12, 3, "Endereco vazio/invalido.");
        wrefresh(janela);
        wgetch(janela);
        mvwprintw(janela,12, 3, "%-40s", "");
    }

    // Telefone
    for (;;) {
        mvwprintw(janela,14, 3, "Telefone: %-20s", "");
        if (ui_getline_or_esc(janela,14, 13, tel, sizeof(tel)) < 0) {  // << ESC
            noecho(); curs_set(0);
            werase(janela); box(janela,0,0);
            mvwprintw(janela, 3, 3, "Cadastro cancelado.");
            wrefresh(janela); wgetch(janela);
            return 9;
        }

        if (cadastro_telefone(tel) == 0) break;
        mvwprintw(janela,15, 3, "Telefone invalido.");
        wrefresh(janela);
        wgetch(janela);
        mvwprintw(janela,15, 3, "%-40s", "");
    }

    // Email
    for (;;) {
        mvwprintw(janela,17, 3, "Email: %-40s", "");
        if (ui_getline_or_esc(janela,17, 10, email, sizeof(email)) < 0) { // << ESC
            noecho(); curs_set(0);
            werase(janela); box(janela,0,0);
            mvwprintw(janela, 3, 3, "Cadastro cancelado.");
            wrefresh(janela); wgetch(janela);
            return 9;
        }

        if (cadastro_email(email) == 0) break;
        mvwprintw(janela,18, 3, "Email invalido.");
        wrefresh(janela);
        wgetch(janela);
        mvwprintw(janela,18, 3, "%-40s", "");
    }

    noecho();
    curs_set(0);

    //COPIAR PARA A STRUCT CONFORME O TIPO
    if (c.tipo == Pessoa_Fisica) {
        strcpy(c.cpf, cpf);
        strcpy(c.nome, nome);
        c.cnpj[0] = '\0';
        c.razao_social[0] = '\0';
        c.nome_contato[0] = '\0';
        strcpy(c.endereco, end);
        strcpy(c.telefone, tel);
        strcpy(c.email, email);
    } else {
        /* ALTERE AQUI: use o buffer validado do CNPJ */
        strcpy(c.cnpj, cnpj_validado);            // << antes estava cnpj (inexistente)
        strcpy(c.razao_social, razao_social);
        strcpy(c.nome_contato, nome_contato);
        c.cpf[0] = '\0';
        c.nome[0] = '\0';
        strcpy(c.endereco, end);
        strcpy(c.telefone, tel);
        strcpy(c.email, email);
    }

    c.id = cliente_obter_proximo_id_pelo_csv("cliente.csv");

    int rc = cliente_gravar_csv(&c);

    // feedback
    werase(janela);
    box(janela, 0, 0);
    if (rc == 0)
        mvwprintw(janela, 3, 3, "Cliente salvo com sucesso. Pressione uma tecla.");
    else
        mvwprintw(janela, 3, 3, "Erro ao salvar (cod %d). Pressione uma tecla.", rc);
    wrefresh(janela);
    wgetch(janela);

    // ao retornar, o submenu redesenha sua lista normalmente
    return rc;
}

int ui_consultar_cliente(WINDOW *janela) {
    int xmax, ymax;
    getmaxyx(janela, ymax, xmax);

    werase(janela); box(janela, 0, 0);
    mvwprintw(janela, 1, 3, "====== Consultar Cliente ======");
    mvwprintw(janela, ymax-2, 3, "[ESC] voltar");
    wrefresh(janela);

    echo(); curs_set(1);
    char codigo[64];

    for (;;) {
        mvwprintw(janela, 3, 3, "Codigo do cliente: %-20s", "");
        if (ui_getline_or_esc(janela, 3, 22, codigo, sizeof(codigo)) < 0) {
            noecho(); curs_set(0);
            return 9; // cancelou
        }

        normaliza_excel_texto(codigo);

        Cliente c = {0};
        int r = cliente_buscar_por_codigo("cliente.csv", codigo, &c);
        if (r == 1) {
            mvwprintw(janela, 5, 3, "Erro ao abrir arquivo.");
            wrefresh(janela); wgetch(janela);
            noecho(); curs_set(0);
            return 6;
        }
        if (r == 2) {
            mvwprintw(janela, 5, 3, "Cliente nao encontrado. Tente outro codigo.");
            wrefresh(janela); wgetch(janela);
            // limpar mensagens e repetir
            mvwprintw(janela, 5, 3, "%-60s", "");
            mvwprintw(janela, 3, 3, "Codigo do cliente: %-20s", "");
            wmove(janela, 3, 22);
            continue;
        }

        // Encontrado → mostrar
        werase(janela); box(janela, 0, 0);
        mvwprintw(janela, 1, 3, "====== Dados do Cliente ======");
        mvwprintw(janela, 3, 3,  "ID: %d", c.id);
        mvwprintw(janela, 4, 3,  "Tipo: %s", (c.tipo == Pessoa_Fisica ? "PF" : "PJ"));
        mvwprintw(janela, 5, 3,  "Codigo: %s", codigo);

        if (c.tipo == Pessoa_Fisica) {
            mvwprintw(janela, 6, 3,  "CPF: %s", c.cpf);
            mvwprintw(janela, 7, 3,  "Nome: %s", c.nome);
        } else {
            mvwprintw(janela, 6, 3,  "CNPJ: %s", c.cnpj);
            mvwprintw(janela, 7, 3,  "Razao social: %s", c.razao_social);
            mvwprintw(janela, 8, 3,  "Contato: %s", c.nome_contato);
        }

        mvwprintw(janela, 10, 3, "Endereco: %s", c.endereco);
        mvwprintw(janela, 11, 3, "Telefone: %s", c.telefone);
        mvwprintw(janela, 12, 3, "Email: %s", c.email);
        mvwprintw(janela, ymax-2, 3, "[qualquer tecla] voltar");
        wrefresh(janela);
        wgetch(janela);

        noecho(); curs_set(0);
        return 0;
    }
}

int ui_remover_cliente(WINDOW *janela) {
    int xmax,ymax; getmaxyx(janela, ymax, xmax);
    char codigo[64];

    werase(janela); box(janela,0,0);
    mvwprintw(janela,1,3,"====== Remover Cliente ======");
    mvwprintw(janela,ymax-2,3,"[ESC] cancelar");
    wrefresh(janela);
    echo(); curs_set(1);

    // 1) Ler código
    mvwprintw(janela,3,3,"Codigo do cliente: %-30s","");
    if (ui_getline_or_esc(janela, 3, 22, codigo, sizeof(codigo)) < 0) {
        noecho(); curs_set(0);
        return 9;
    }
    normaliza_excel_texto(codigo);
    noecho(); curs_set(0);

    // 2) Buscar para mostrar resumo
    Cliente c = {0};
    int br = cliente_buscar_por_codigo("cliente.csv", codigo, &c);
    if (br != 0) {
        mvwprintw(janela,5,3,"Cliente nao existe.");
        wrefresh(janela); wgetch(janela);
        return 2;
    }

    // 3) Mostrar um resumo (conforme tipo) + confirmar
    werase(janela); box(janela,0,0);
    mvwprintw(janela,1,3,"====== Confirmar Remocao ======");
    mvwprintw(janela,3,3,"Codigo: %s", c.codigo);
    mvwprintw(janela,4,3,"Tipo: %s", (c.tipo==Pessoa_Fisica)?"PF":"PJ");
    if (c.tipo==Pessoa_Fisica) {
        mvwprintw(janela,5,3,"CPF: %s", c.cpf);
        mvwprintw(janela,6,3,"Nome: %s", c.nome);
    } else {
        mvwprintw(janela,5,3,"CNPJ: %s", c.cnpj);
        mvwprintw(janela,6,3,"Razao Social: %s", c.razao_social);
        mvwprintw(janela,7,3,"Contato: %s", c.nome_contato);
    }
    mvwprintw(janela,9,3,"Endereco: %s", c.endereco);
    mvwprintw(janela,10,3,"Telefone: %s", c.telefone);
    mvwprintw(janela,11,3,"Email: %s", c.email);

    mvwprintw(janela,ymax-3,3,"Confirmar exclusao? [Y] Sim  [N] Nao");
    wrefresh(janela);

    int ch;
    for (;;) {
        ch = wgetch(janela);
        if (ch=='y' || ch=='Y' || ch=='s' || ch=='S') break;
        if (ch=='n' || ch=='N' || ch==27) {
            mvwprintw(janela,ymax-2,3,"Operacao cancelada.");
            wrefresh(janela); wgetch(janela);
            return 9;
        }
    }

    // 4) Remover
    int rr = cliente_remover_por_codigo("cliente.csv", codigo);
    werase(janela); box(janela,0,0);
    if (rr==0) {
        mvwprintw(janela,3,3,"Cliente removido com sucesso.");
    } else if (rr==2) {
        mvwprintw(janela,3,3,"Cliente nao encontrado (talvez alterado).");
    } else {
        mvwprintw(janela,3,3,"Erro ao remover (cod %d).", rr);
    }
    wrefresh(janela); wgetch(janela);
    return rr;
}

static void desenha_linha_cliente(WINDOW *w, int y, const Cliente *c, int xmax) {
    char doc[32] = {0};
    char tel[32] = {0};
    const char *nome_ou_razao = (c->tipo == Pessoa_Fisica) ? c->nome : c->razao_social;

    // DOC: copia CPF ou CNPJ, normaliza e deixa só dígitos
    if (c->tipo == Pessoa_Fisica) {
        strncpy(doc, c->cpf, sizeof(doc)-1);
    } else {
        strncpy(doc, c->cnpj, sizeof(doc)-1);
    }
    normaliza_excel_texto(doc);
    somente_digitos(doc);

    // Telefone: normaliza e deixa só dígitos
    strncpy(tel, c->telefone, sizeof(tel)-1);
    normaliza_excel_texto(tel);
    somente_digitos(tel);

    // Imprime (ajuste as larguras conforme seu layout)
    mvwprintw(w, y, 2,
              "%4d  %2s  %-6s  %-14.14s  %-22.22s  %-12.12s  %-24.24s",
              c->id,
              (c->tipo == Pessoa_Fisica ? "PF" : "PJ"),
              c->codigo,
              doc,
              nome_ou_razao,
              tel,
              c->email);
}

int ui_listar_clientes(WINDOW *janela) {
    int xmax, ymax;
    getmaxyx(janela, ymax, xmax);

    Cliente *vet = NULL;
    int n = 0;
    int rc = cliente_carregar_todos("cliente.csv", &vet, &n);
    if (rc == 1) { // erro abrir
        werase(janela); box(janela,0,0);
        mvwprintw(janela, 3, 3, "Erro ao abrir cliente.csv");
        wrefresh(janela); wgetch(janela);
        return 6;
    }
    if (rc != 0) {
        werase(janela); box(janela,0,0);
        mvwprintw(janela, 3, 3, "Falha ao carregar lista (cod %d)", rc);
        wrefresh(janela); wgetch(janela);
        cliente_libera_lista(vet);
        return 6;
    }

    int topo = 0;                         // índice da primeira linha visível
    int visiveis = ymax - 6;              // cabeçalho + rodapé
    if (visiveis < 1) visiveis = 1;

    keypad(janela, TRUE);
    curs_set(0);
    noecho();

    for (;;) {
        werase(janela); box(janela, 0, 0);
        mvwprintw(janela, 1, 3, "====== Lista de Clientes (%d registros) ======", n);
        mvwprintw(janela, 3, 2, " ID  T  COD      DOC(11/14)     NOME/RAZAO              TELEFONE    EMAIL");
        mvwhline(janela, 4, 1, 0, xmax-2);

        for (int i = 0; i < visiveis && (topo + i) < n; i++) {
            desenha_linha_cliente(janela, 5 + i, &vet[topo + i], xmax);
        }

        mvwprintw(janela, ymax-2, 2, "[UP/DOWN] rolar  [PgUp/PgDn] pular  [ESC] voltar");
        wrefresh(janela);

        int ch = wgetch(janela);
        if (ch == 27 /* ESC */) break;
        else if (ch == KEY_UP)   { if (topo > 0) topo--; }
        else if (ch == KEY_DOWN) { if (topo + visiveis < n) topo++; }
        else if (ch == KEY_PPAGE){ topo -= visiveis; if (topo < 0) topo = 0; }
        else if (ch == KEY_NPAGE){ topo += visiveis; if (topo + visiveis > n) topo = (n>visiveis ? n - visiveis : 0); }
        else if (ch == KEY_HOME) { topo = 0; }
        else if (ch == KEY_END)  { topo = (n>visiveis ? n - visiveis : 0); }
    }

    cliente_libera_lista(vet);
    return 0;
}

void submenu_cliente() {
    

    char opcoes[5][20] = {
        "Cadastrar Cliente",
        "Consultar Cliente",
        "Remover Cliente",
        "Listar Clientes",
        "Voltar"
    };

    int xmax, ymax;
    char titulo[1][100] = {"====== Menu Cliente ======"};
    int tecla, seletor = 2;
    getmaxyx(stdscr, ymax, xmax);

    WINDOW *janela = newwin(20, xmax - 10, ymax - 25, 5);
    box(janela, 0, 0);

    start_color();
    init_pair(1, COLOR_CYAN, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_CYAN);

    wbkgd(janela, COLOR_PAIR(1));

    keypad(stdscr, TRUE);

    while(1) {
        werase(janela);
        box(janela, 0, 0);

        wattron(janela, COLOR_PAIR(1));
        mvwprintw(janela, 1, 40, "%s", titulo[0]);
        wattroff(janela, COLOR_PAIR(1));

        for(int i = 0; i < 5; i++) {

            if(i == seletor) {
                wattron(janela, COLOR_PAIR(2));
                mvwprintw(janela, i + 3, 3, "%s", opcoes[i]);
                wattroff(janela, COLOR_PAIR(2));
            } else {
                wattron(janela, COLOR_PAIR(1));
                mvwprintw(janela, i + 3, 3, "%s", opcoes[i]);
                wattroff(janela, COLOR_PAIR(1));
            }

        }

        refresh();
        wrefresh(janela);

        tecla = getch();

        switch(tecla) {
            case KEY_UP:
                seletor--;
                if(seletor < 0) {
                    seletor = 4;
                }
            break;
            case KEY_DOWN:
                seletor++;
                if(seletor > 4) {
                    seletor = 0;
                }
            break;

            case '\n':
                case KEY_ENTER:
                    if(seletor == 0) {
                        ui_cadastrar_cliente(janela);
                    }

                    if(seletor == 1) {
                        ui_consultar_cliente(janela);
                    }

                    if(seletor == 2) {
                        ui_remover_cliente(janela);
                    }

                    if(seletor == 3) {
                        ui_listar_clientes(janela);
                    }

                    if(seletor == 4) {
                         delwin(janela);
                        return;
                    }
            break;
        }

    }
    
}

int main() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    
    // Primeiro refresh para limpar a tela
    refresh();

    // VERIFICAR/CRIAR ARQUIVOS NECESSÁRIOS
    FILE *teste;
    
    // Verificar arquivo de produtos
    teste = fopen("Produtos.csv", "r");
    if (!teste) {
        // Criar arquivo com cabeçalho
        teste = fopen("Produtos.csv", "w");
        if (teste) {
            fprintf(teste, "id;descricao;preco;estoque\n");
            fclose(teste);
        }
    } else {
        fclose(teste);
    }
    
    // Verificar arquivo de clientes
    teste = fopen("cliente.csv", "r");
    if (!teste) {
        // Criar arquivo com cabeçalho
        teste = fopen("cliente.csv", "w");
        if (teste) {
            fprintf(teste, "id;tipo;codigo;cpf;cnpj;razao_social;nome_contato;nome;endereco;telefone;email\n");
            fclose(teste);
        }
    } else {
        fclose(teste);
    }
    
    // Verificar arquivo de pedidos
    teste = fopen("Pedidos.csv", "r");
    if (!teste) {
        // Criar arquivo com cabeçalho
        teste = fopen("Pedidos.csv", "w");
        if (teste) {
            fprintf(teste, "codigoPedido;codigoCliente;codigoProduto;quantidade;data;descricao;valorTotal;status\n");
            fclose(teste);
        }
    } else {
        fclose(teste);
    }
    
    int xmax, ymax;
    char menu_principal[3][20] = {"Cliente", "Produto", "Pedido "};
    char titulo[100] = "====== Menu Principal ======";
    int tecla, seletor = 0;
    getmaxyx(stdscr, ymax, xmax);

    WINDOW *janela = newwin(8, xmax - 60, ymax/2 - 4, xmax/2 - (xmax-60)/2);
    box(janela, 0, 0);

    start_color();
    init_pair(1, COLOR_CYAN, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_CYAN);

    wbkgd(janela, COLOR_PAIR(1));
    keypad(janela, TRUE);

    while(1) {
        werase(janela);
        box(janela, 0, 0);

        wattron(janela, COLOR_PAIR(2) | A_BOLD);
        mvwprintw(janela, 1, (xmax-60)/2 - 15, "%s", titulo);
        wattroff(janela, COLOR_PAIR(2) | A_BOLD);

        for(int i = 0; i < 3; i++) {
            if(i == seletor) {
                wattron(janela, COLOR_PAIR(2) | A_BOLD);
                mvwprintw(janela, i + 3, 5, "> %s", menu_principal[i]);
                wattroff(janela, COLOR_PAIR(2) | A_BOLD);
            } else {
                wattron(janela, COLOR_PAIR(1));
                mvwprintw(janela, i + 3, 7, "%s", menu_principal[i]);
                wattroff(janela, COLOR_PAIR(1));
            }
        }

        // Instruções
        wattron(janela, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(janela, 7, 5, "ENTER: Selecionar | ESC: Sair");
        wattroff(janela, COLOR_PAIR(1) | A_BOLD);

        wrefresh(janela);

        tecla = wgetch(janela);

        switch(tecla) {
            case KEY_UP:
                seletor--;
                if(seletor < 0) seletor = 2;
                break;
            case KEY_DOWN:
                seletor++;
                if(seletor > 2) seletor = 0;
                break;
            case '\n':
            case KEY_ENTER:
                if(seletor == 0) {
                    submenu_cliente(janela);
                } else if(seletor == 1) {
                    submenu_produto(janela); 
                } else if(seletor ==2) {
                    submenu_pedido(janela);
                } else {
                    werase(janela);
                    box(janela, 0, 0);
                    wattron(janela, COLOR_PAIR(1));
                    mvwprintw(janela, 3, 5, "Voce entrou em: %s", menu_principal[seletor]);
                    mvwprintw(janela, 5, 5, "Pressione qualquer tecla...");
                    wattroff(janela, COLOR_PAIR(1));
                    wrefresh(janela);
                    wgetch(janela);
                }
                break;
            case 27: // ESC
                delwin(janela);
                endwin();
                return 0;
        }
    }

    delwin(janela);
    endwin();
    return 0;
}
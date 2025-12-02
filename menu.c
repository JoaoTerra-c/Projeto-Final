#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "cliente.h"

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


//SUBMENUS DE CLIENTE, PRODUTO E PEDIDO

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

void submenu_produto() {
    

    char opcoes[5][20] = {
        "Cadastrar Produto",
        "Listar Produtos",
        "Remover Produto",
        "Consultar Produto",
        "Voltar"
    };

    int xmax, ymax;
    char titulo[1][100] = {"====== Menu Produto ======"};
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
        }

        if(tecla == '\n') {
            break;
        }

    }
    
}

void submenu_pedido() {
    

    char opcoes[5][20] = {
        "Cadastrar Pedido",
        "Listar Pedidos",
        "Remover Pedido",
        "Consultar Pedido",
        "Voltar"
    };

    int xmax, ymax;
    char titulo[1][100] = {"====== Menu Pedido ======"};
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
        }

        if(tecla == '\n') {
            break;
        }

    }
    
}

int main() {

    initscr();
    noecho();
    cbreak();
    curs_set(0);

    int xmax, ymax;
    char menu_principal[4][20] = {"Manter Cliente", "Manter Produto", "Manter Pedido", "Sair"};
    char titulo[1][100] = {"====== Menu Principal ======"};
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

        for(int i = 0; i < 4; i++) {

            if(i == seletor) {
                wattron(janela, COLOR_PAIR(2));
                mvwprintw(janela, i + 3, 3, "%s", menu_principal[i]);
                wattroff(janela, COLOR_PAIR(2));
            } else {
                wattron(janela, COLOR_PAIR(1));
                mvwprintw(janela, i + 3, 3, "%s", menu_principal[i]);
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
                    seletor = 3;
                }
            break;
            case KEY_DOWN:
                seletor++;
                if(seletor > 3) {
                    seletor = 0;
                }
            break;

            case '\n':
                case KEY_ENTER:
                    if(seletor == 0) {
                        submenu_cliente();
                    } else if(seletor == 1) {
                        submenu_produto(janela);
                    } else if(seletor == 2) {
                        submenu_pedido();
                    } 
                    else{
                        delwin(janela);   // libera a janela
                        endwin();         // encerra o ncurses (restaura o terminal)
                        return 0;         // sai do main
                    }
            break;
        }


    }

    endwin();

    return 0;
}

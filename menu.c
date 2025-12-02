#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "cliente.h"

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
    char cpf[12], cnpj_digitado[64], cnpj_validado[15], nome[100], end[200], tel[20], email[100];

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
            if (cadastro_cpf(cpf, cpf) == 0) { strcpy(c.cpf, cpf); break; }
            mvwprintw(janela, 8, 3, "CPF invalido. Tente novamente.");
            wrefresh(janela); wgetch(janela);
            mvwprintw(janela, 8, 3, "%-40s", "");
        }
    } else {
        // CNPJ (precisa ter sua funcao de validacao similar ao CPF)
        char cnpj[15];
        for (;;) {
            mvwprintw(janela, 7, 3, "CNPJ (14 digitos): %-20s", "");
            if (ui_getline_or_esc(janela, 7, 24, cnpj, sizeof(cnpj)) < 0) {
                noecho(); curs_set(0);
                werase(janela); box(janela,0,0);
                mvwprintw(janela, 3, 3, "Cadastro cancelado.");
                wrefresh(janela); wgetch(janela);
                return 9;
            }
            if (cadastro_cnpj(cnpj, cnpj) == 0) { strcpy(c.cnpj, cnpj); break; } // implemente cadastro_cnpj
            mvwprintw(janela, 8, 3, "CNPJ invalido. Tente novamente.");
            wrefresh(janela); wgetch(janela);
            mvwprintw(janela, 8, 3, "%-40s", "");
        }
    }

    // CPF (loop até ficar válido)
    for (;;) {
        // limpa linha de entrada e mensagem
        mvwprintw(janela, 5, 3, "CPF (11 digitos): %-20s", "");  // apaga área
        if (ui_getline_or_esc(janela, 5, 22, cpf, sizeof(cpf)) < 0) {  // << usa helper e checa ESC
            noecho(); curs_set(0);
            werase(janela); box(janela,0,0);
            mvwprintw(janela, 3, 3, "Cadastro cancelado.");
            wrefresh(janela); wgetch(janela);
            return 9; // cancelado
        }

        // chame SUA validacao (ideal: função que só valida a string)
        if (cadastro_cpf(cpf, cpf) == 0) break;

        // mensagem de erro e espera tecla
        mvwprintw(janela, 6, 3, "CPF invalido. Tente novamente.");
        wrefresh(janela);
        wgetch(janela);
        // apaga mensagem
        mvwprintw(janela, 6, 3, "%-40s", "");
    }

    // Nome
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

    // Se chegou aqui, todos campos ok → grava de uma vez
    strcpy(c.cpf, cpf);
    strcpy(c.nome, nome);
    strcpy(c.endereco, end);
    strcpy(c.telefone, tel);
    strcpy(c.email, email);

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
 
void submenu_cliente() {
    

    char opcoes[5][20] = {
        "Cadastrar Cliente",
        "Listar Clientes",
        "Remover Cliente",
        "Consultar Cliente",
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
                        submenu_cliente(janela);
                    } else if(seletor == 1) {
                        submenu_produto(janela);
                    } else if(seletor == 2) {
                        submenu_pedido();
                    } 
                    else{
                        werase(janela);
                        box(janela,0,0);
                        wattron(janela, COLOR_PAIR(1));
                        mvwprintw(janela, 3, 3, "Voce entrou em: %s", menu_principal[seletor]);
                        mvwprintw(janela, 5, 3, "Pressione qualquer tecla...");
                        wattroff(janela, COLOR_PAIR(1));
                        wrefresh(janela);
                        getch();
                    }
            break;
        }


    }

    endwin();

    return 0;
}

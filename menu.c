#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

void submenu_cliente() {
    

    char opcoes[4][20] = {
        "Cadastrar Cliente",
        "Listar Clientes",
        "Remover Cliente",
        "Consultar Cliente"
    };

    int xmax, ymax;
    char titulo[1][100] = {"====== Menu Cliente ======"};
    int tecla, seletor = 2;
    getmaxyx(stdscr, ymax, xmax);

    WINDOW *janela = newwin(8, xmax - 80, ymax - 25, 5);
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
        mvwprintw(janela, 1, 3, "%s", titulo[0]);
        wattroff(janela, COLOR_PAIR(1));

        for(int i = 0; i < 4; i++) {

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
                    seletor = 3;
                }
            break;
            case KEY_DOWN:
                seletor++;
                if(seletor > 3) {
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
    char menu_principal[3][20] = {"Cliente", "Produto", "Pedido "};
    char titulo[1][100] = {"====== Menu Principal ======"};
    int tecla, seletor = 2;
    getmaxyx(stdscr, ymax, xmax);

    WINDOW *janela = newwin(8, xmax - 80, ymax - 25, 5);
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
        mvwprintw(janela, 1, 3, "%s", titulo[0]);
        wattroff(janela, COLOR_PAIR(1));

        for(int i = 0; i < 3; i++) {

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
                    seletor = 2;
                }
            break;
            case KEY_DOWN:
                seletor++;
                if(seletor > 2) {
                    seletor = 0;
                }
            break;

            case '\n':
                case KEY_ENTER:
                    if(seletor == 0) {
                        submenu_cliente(janela);
                    } else {
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "pedido.h"

/* Funções utilitárias */
void DataAtual(char *destino, int tamanho){
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    snprintf(destino, tamanho, "%02d/%02d/%04d",
             tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
}

int codClientejaaExiste(const char *arquivoClientes, int code){
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

int codProdutojaExiste(const char *arquivoProdutos, int code){
    FILE *fp = fopen(arquivoProdutos, "r");
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

int obterinfoProduto(const char *arquivoProdutos, int codigoProduto, char *descricaoDestino, double *precoDestino){
    FILE *fp = fopen(arquivoProdutos, "r");
    if(!fp) return 0;

    char linha[1024];
    while(fgets(linha, sizeof(linha), fp)){
        int id;
        char desc[200];
        double preco;
        int est;

        // Note: Presuming product file format is: ID ; Description ; Price ; Stock
        if(sscanf(linha, "%d ; %[^;] ; %lf ; %d", &id, desc, &preco, &est) == 4){
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

int codigoPedidojaExiste(FILE* fpPe, int code){
    rewind(fpPe);
    int codigoPedido;

    // A leitura tenta casar o primeiro campo (código do pedido)
    // e ignora o resto da linha até o final da linha.
    while(fscanf(fpPe, "%d ; %*d ; %*d ; %*d ; %*[^;] ; %*[^;] ; %*lf ; %*[^
]%*c", &codigoPedido) == 1){
        if(codigoPedido == code)
            return 1;
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

    if(codigoPedidojaExiste(fpPe, p.codigoPedido)){
        mvprintw(5,0,"Erro: código já existe!");
        getch();
        return;
    }

    mvprintw(3,0,"Código do Cliente: ");
    scanw("%d", &p.codigoCliente);

    if(!codClientejaaExiste(arquivoClientes, p.codigoCliente)){
        mvprintw(6,0,"Erro: cliente não existe!");
        getch();
        return;
    }

    mvprintw(4,0,"Código do Produto: ");
    scanw("%d", &p.codigoProduto);

    if(!codProdutojaExiste(arquivoProdutos, p.codigoProduto)){
        mvprintw(7,0,"Erro: produto não existe!");
        getch();
        return;
    }

    mvprintw(5,0,"Quantidade: ");
    scanw("%d", &p.quantidade);

    if(!obterinfoProduto(arquivoProdutos, p.codigoProduto, p.descricao, &precoUnit)){
        // Isso não deve ocorrer se codProdutojaExiste for verdadeiro, mas é uma segurança
        mvprintw(8,0,"Erro ao obter dados do produto!");
        getch();
        return;
    }

    p.valorTotal = p.quantidade * precoUnit;
    DataAtual(p.data, maxData); // Uso da função DataAtual
    strcpy(p.status, "Pendente");

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

    // A leitura tenta casar 8 campos, com tratamento para strings com espaços
    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^
]%*c",
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

    if(!codClientejaaExiste(arquivoClientes, codigoCliente)){
        mvprintw(3,0,"Cliente não existe!");
        getch();
        return 0;
    }

    rewind(fpPe);

    Pedido p;
    printw("\nPedidos do cliente:\n\n");

    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^
]%*c",
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

    if(!codClientejaaExiste(arquivoClientes, codigoCliente)){
        mvprintw(3,0,"Cliente não existe!");
        getch();
        return fpPe;
    }

    mvprintw(1,0,"Código do Pedido: ");
    scanw("%d", &codigoPedido);

    FILE *temp = fopen("Pedidos_temp.csv", "w");
    if (temp == NULL) {
        // Tratamento de erro
        mvprintw(5, 0, "Erro ao criar arquivo temporário!");
        getch();
        return fpPe;
    }


    rewind(fpPe);

    Pedido p;
    int encontrado = 0;

    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^
]%*c",
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
                if(obterinfoProduto(arquivoProdutos, p.codigoProduto, p.descricao, &precoU)){
                     p.valorTotal = p.quantidade * precoU;
                } else {
                    // Trata caso o novo produto não exista
                    mvprintw(6,0,"Novo produto não existe!");
                    getch();
                    // Poderia manter o produto antigo ou sair, aqui opto por seguir com o valor total antigo
                    // A lógica original não tratava este erro, apenas a alteração de código.
                }

            }
            else if(opc == 2){
                mvprintw(5,0,"Nova quantidade: "); scanw("%d",&p.quantidade);
                // Assume que o produto atual é válido
                if(obterinfoProduto(arquivoProdutos, p.codigoProduto, p.descricao, &precoU)){
                    p.valorTotal = p.quantidade * precoU;
                }
            }
            else if(opc == 3){
                mvprintw(5,0,"Novo status: "); scanw("%s", p.status);
            }

            DataAtual(p.data, maxData); // Uso da função DataAtual
        }

        fprintf(temp,"%d ; %d ; %d ; %d ; %s ; %s ; %.2f ; %s\n",
                p.codigoPedido,p.codigoCliente,p.codigoProduto,p.quantidade,
                p.data,p.descricao,p.valorTotal,p.status);
    }

    fclose(fpPe);
    fclose(temp);

    remove(ARQUIVO_PEDIDOS);
    rename("Pedidos_temp.csv", ARQUIVO_PEDIDOS);

    fpPe = fopen(ARQUIVO_PEDIDOS, "a+");
    if (fpPe == NULL) {
        // Tratamento de erro se não conseguir reabrir o arquivo principal
        clear();
        mvprintw(1, 0, "Erro crítico: não foi possível reabrir o arquivo de pedidos!");
        getch();
        // Em um ambiente de produção, aqui deveria haver um tratamento mais robusto.
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

    if(!codClientejaaExiste(arquivoClientes, codigoCliente)){
        mvprintw(3,0,"Cliente não existe!");
        getch();
        return fpPe;
    }

    mvprintw(1,0,"Código do Pedido: ");
    scanw("%d",&codigoPedido);

    FILE *temp = fopen("Pedidos_temp.csv","w");
    if (temp == NULL) {
        // Tratamento de erro
        mvprintw(5, 0, "Erro ao criar arquivo temporário!");
        getch();
        return fpPe;
    }

    rewind(fpPe);
    Pedido p;
    int deletado = 0;

    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^
]%*c",
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
    if (fpPe == NULL) {
        // Tratamento de erro
        clear();
        mvprintw(2, 0, "Erro crítico: não foi possível reabrir o arquivo de pedidos!");
        getch();
        // Em um ambiente de produção, aqui deveria haver um tratamento mais robusto.
    }

    clear();
    if(deletado)
        mvprintw(2,0,"Pedido apagado com sucesso!");
    else
        mvprintw(2,0,"Pedido não encontrado!");

    getch();
    return fpPe;
}

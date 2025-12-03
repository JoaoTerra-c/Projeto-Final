// pedido.c  (versão corrigida — mantém scanw/strcpy etc., corrige apenas parsing e lógica)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "pedido.h"

/* Funções utilitárias */
void obterDataAtual(char *destino, int tamanho){
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    snprintf(destino, tamanho, "%02d/%02d/%04d",
             tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
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

/* Verifica se um código de produto existe */
int codigoProdutoJaExiste(const char *arquivoProdutos, int code){
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

/* Obtém descrição e preço de um produto pelo arquivo de produtos.
  
     codigoProduto ; descricao ; preco ; estoque
*/
int obterDadosProduto(const char *arquivoProdutos, int codigoProduto, char *descricaoDestino, double *precoDestino){
    FILE *fp = fopen(arquivoProdutos, "r");
    if(!fp) return 0;

    char linha[1024];
    while(fgets(linha, sizeof(linha), fp)){
        int id;
        char desc[200];
        double preco;
        int est;

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

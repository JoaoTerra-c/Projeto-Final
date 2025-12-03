#ifndef PEDIDO_H
#define PEDIDO_H

#include <stdio.h>

#define maxDescricao 200
#define maxData 20
#define maxStatus 30

#define ARQUIVO_PEDIDOS   "Pedidos.csv"
#define ARQUIVO_CLIENTES  "Clientes.csv"
#define ARQUIVO_PRODUTOS  "Produtos.csv"

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

//Funções

int codClientejaaExiste(const char *arquivoClientes, int code);
void DataAtual(char *destino, int tamanho);
int codProdutojaExiste(const char *arquivoProdutos, int code);
nt codigoPedidojaExiste(FILE* fpPe, int code);
int obterinfoProduto(const char *arquivoProdutos, int codigoProduto, char *descricaoDestino, double *precoDestino);


void cadastrarPedido(FILE *fpPe, const char *arquivoClientes, const char *arquivoProdutos);
void listarPedidos(FILE *fpPe);
int consultarPedido(FILE *fpPe, const char *arquivoClientes);
FILE* deletarPedido(FILE *fpPe, const char *arquivoClientes);
FILE* atualizarPedido(FILE *fpPe, const char *arquivoClientes, const char *arquivoProdutos);

#endif

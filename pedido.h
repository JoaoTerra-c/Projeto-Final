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

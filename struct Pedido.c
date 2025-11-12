#include <stdio.h>
#include <string.h>

typedef struct {
    int pedidoCOD     // Identificador (Numeral) do pedido
    int cliente;      // ID do cliente que fez o pedido
    char data[11];    // Data do pedido (formato "dd/mm/aaaa")
    float total;      // Valor total do pedido
} Pedido;

//Menu dos pedidos

void menuPedidos();

//Funções utilizadas do Pedido

void inserirPedido(Pedido *quantidade);
void listarPedido(Pedido *quantidade);
void detalharPedido(Pedido *quantidade);
void salvarPedidos(Pedido *quantidade);
void carregarPedidos(Pedido *quantidade);


//Struct ItemPedido


typedef struct {
    int pedidoCOD;    // Codigo do pedido ao qual o item pertence
    int produtoCOD;   // Codigo do produto que está sendo comprado
    int quantidade;   // Quantidade desse produto no pedido
    float subtotal;   // Valor total deste item
} ItemPedido;

//Funções detro do item Pedido

void inserirItemPedido(ItemPedido *quantidade);
void listarItensPedido(ItemPedido quantidade);
void salvarItensCSV(ItemPedido quantidade);
void carregarItensCSV(ItemPedido  *quantidade);
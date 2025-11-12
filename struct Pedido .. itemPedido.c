#include <stdio.h>
#include <string.h>

#define pedidosmax 100; 

typedef struct {

    int pedidoCOD     // Identificador (Numeral) do pedido
    int cliente;      // ID do cliente que fez o pedido
    char data[11];      // Data do pedido (formato "dd/mm/aaaa")
    float total;       // Valor total do pedido
} Pedido;


// funnções que entram dentro da struct (InserirPedido/ ListaPedidos/ DetalharPedido/ SalvarPedido/ CarregarPedidos)


//Função: Mostrar menu

void menuPedidos(){
    printf("\n Menu de opções do pedido\n");
    printf("Inserir Pedido-1\n");
    printf("Listar Pedidos-2\n");
    pritf("Detalhar Pedido-3\n");
    printf("Salvar Pedidos-4 \n");
    printf("Carregar Pedidos-5 \n");
}

// Função : InserirPedido

void InserirPedido(Pedido npedidos [pedidosmax],  int *quantidade){
    Pedido n;

    printf("\n Insira eu Pedido\n");
    pritnf("Numero do pedido:");
    scanf("%d", &n.pedidoCOD);

    printf("Identificador do cliente");// analisar quem é o cliente com um cod diferente pra cada
    scanf("%d", &n.cliente);

    printf("Data"); //Data que o cliente fez pedido
    scanf("%s", &n.data);

    printf("Total do pedido") // quanto o pedido ficou em R$
    scanf("%f", &n. total); 

    pedidos[*quantidade] = n;

    printf("/n Pedido Cadastrado!")
}

// Lista os Pedidos 

void ListarPedido(Pedido[], int quantidade) {
    
    printf("\n Listagem dos Pedidos\n");
    for(int i=0; i< pedidosmax; i++){
        printf("\nCodigo:%d || Cliente:%d || Data:%d || Total: R$%.2f\n "), npedidos[i].pedidoCOD, npedidos[i].cliente, npedidos[i].data, npedidos[i].total;
    }


}


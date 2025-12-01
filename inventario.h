#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 100
#define MAX_DESCRICAO 200
#define ARQUIVO_DADOS "inventario.dat"
#define MAX_ITENS 1000

// Estrutura p representar um item do inventário
typedef struct {
    int codigo;
    char nome[MAX_NOME];
    char descricao[MAX_DESCRICAO];
    int quantidade;
    float preco;
    int ativo; // 1 = ativo, 0 = removido logicamente
} Item;

// Estrutura p gerenciar o inventário
typedef struct {
    Item *itens;        // Ponteiro para o array de itens
    int total_itens;    // Total de itens cadastrados
    int capacidade;     // Capacidade atual do array
} Inventario;

// funcs de inicialização e finalização
Inventario* criar_inventario();
void destruir_inventario(Inventario **inv);

// funcoes de arquivo
int salvar_inventario(Inventario *inv);
int carregar_inventario(Inventario *inv);

// func de gerenciamento de itens
int adicionar_item(Inventario *inv, int codigo, char *nome, char *descricao, int quantidade, float preco);
int remover_item(Inventario *inv, int codigo);
int atualizar_item(Inventario *inv, int codigo, char *nome, char *descricao, int quantidade, float preco);
Item* buscar_item(Inventario *inv, int codigo);
Item* buscar_item_por_nome(Inventario *inv, char *nome);

// func de exibição
void listar_todos_itens(Inventario *inv);
void listar_itens_ativos(Inventario *inv);
void exibir_item(Item *item);
void exibir_estatisticas(Inventario *inv);

// Funções auxiliares
int validar_codigo(Inventario *inv, int codigo);
void ordenar_por_codigo(Inventario *inv);
void ordenar_por_nome(Inventario *inv);

#endif // INVENTARIO_H



#include "inventario.h"

// Função para criar e inicializar o inventário
Inventario* criar_inventario() {
    Inventario *inv = (Inventario*)malloc(sizeof(Inventario));
    if (inv == NULL) {
        printf("Erro ao alocar memoria para o inventario!\n");
        return NULL;
    }

    inv->capacidade = 10;
    inv->total_itens = 0;
    inv->itens = (Item*)malloc(inv->capacidade * sizeof(Item));

    if (inv->itens == NULL) {
        printf("Erro ao alocar memoria para os itens!\n");
        free(inv);
        return NULL;
    }

    return inv;
}

// Função para liberar a memória do inventário
void destruir_inventario(Inventario **inv) {
    if (inv != NULL && *inv != NULL) {
        if ((*inv)->itens != NULL) {
            free((*inv)->itens);
        }
        free(*inv);
        *inv = NULL;
    }
}

// Função pra redimensionar o array de itens quando precis
static int redimensionar_inventario(Inventario *inv) {
    int nova_capacidade = inv->capacidade * 2;
    Item *novos_itens = (Item*)realloc(inv->itens, nova_capacidade * sizeof(Item));

    if (novos_itens == NULL) {
        printf("Erro ao redimensionar o inventario!\n");
        return 0;
    }

    inv->itens = novos_itens;
    inv->capacidade = nova_capacidade;
    return 1;
}

// Função p salvar o inventario  em arquivo binário
int salvar_inventario(Inventario *inv) {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita!\n");
        return 0;
    }

    // Escreve o total de itens
    fwrite(&inv->total_itens, sizeof(int), 1, arquivo);

    // Escreve todos os itens (incluindo os removidos )
    fwrite(inv->itens, sizeof(Item), inv->total_itens, arquivo);

    fclose(arquivo);
    return 1;
}

// Função para carregar o inventário de arquivo binário
int carregar_inventario(Inventario *inv) {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "rb");
    if (arquivo == NULL) {
        // Arquivo não existe ainda, não é um erro
        return 0;
    }

    // le o total de itens
    int total;
    if (fread(&total, sizeof(int), 1, arquivo) != 1) {
        fclose(arquivo);
        return 0;
    }

    // Redimensiona se precisar
    while (inv->capacidade < total) {
        if (!redimensionar_inventario(inv)) {
            fclose(arquivo);
            return 0;
        }
    }

    // lee os itens
    inv->total_itens = total;
    if (total > 0) {
        if (fread(inv->itens, sizeof(Item), total, arquivo) != total) {
            fclose(arquivo);
            return 0;
        }
    }

    fclose(arquivo);
    return 1;
}

// Função p validar se um código já existe
int validar_codigo(Inventario *inv, int codigo) {
    int i;
    for (i = 0; i < inv->total_itens; i++) {
        if (inv->itens[i].codigo == codigo && inv->itens[i].ativo == 1) {
            return 0; // Código já existe
        }
    }
    return 1; // Código válido
}

// Função para adicionar um novo item ao invent
int adicionar_item(Inventario *inv, int codigo, char *nome, char *descricao, int quantidade, float preco) {
    if (inv == NULL) {
        return 0;
    }

    // Valida o código
    if (!validar_codigo(inv, codigo)) {
        printf("Erro: Codigo %d já existe no inventario!\n", codigo);
        return 0;
    }

    // Valida os dados
    if (codigo <= 0 || quantidade < 0 || preco < 0) {
        printf("Erro: Dados invalidos!\n");
        return 0;
    }

    // Redimensiona se necessário
    if (inv->total_itens >= inv->capacidade) {
        if (!redimensionar_inventario(inv)) {
            return 0;
        }
    }

    // Adiciona o novo item
    Item *novo_item = &inv->itens[inv->total_itens];
    novo_item->codigo = codigo;
    strncpy(novo_item->nome, nome, MAX_NOME - 1);
    novo_item->nome[MAX_NOME - 1] = '\0';
    strncpy(novo_item->descricao, descricao, MAX_DESCRICAO - 1);
    novo_item->descricao[MAX_DESCRICAO - 1] = '\0';
    novo_item->quantidade = quantidade;
    novo_item->preco = preco;
    novo_item->ativo = 1;

    inv->total_itens++;
    return 1;
}

// func para buscar um item por código
Item* buscar_item(Inventario *inv, int codigo) {
    if (inv == NULL) {
        return NULL;
    }

    int i;
    for (i = 0; i < inv->total_itens; i++) {
        if (inv->itens[i].codigo == codigo && inv->itens[i].ativo == 1) {
            return &inv->itens[i];
        }
    }
    return NULL;
}

// func para buscar um item por nome
Item* buscar_item_por_nome(Inventario *inv, char *nome) {
    if (inv == NULL) {
        return NULL;
    }

    int i;
    for (i = 0; i < inv->total_itens; i++) {
        if (strcmp(inv->itens[i].nome, nome) == 0 && inv->itens[i].ativo == 1) {
            return &inv->itens[i];
        }
    }
    return NULL;
}

// func para remover um item (remoção lógica)
int remover_item(Inventario *inv, int codigo) {
    if (inv == NULL) {
        return 0;
    }

    Item *item = buscar_item(inv, codigo);
    if (item == NULL) {
        printf("Item com codigo %d nao encontrado!\n", codigo);
        return 0;
    }

    item->ativo = 0;
    return 1;
}

// func para atualizar um item
int atualizar_item(Inventario *inv, int codigo, char *nome, char *descricao, int quantidade, float preco) {
    if (inv == NULL) {
        return 0;
    }

    Item *item = buscar_item(inv, codigo);
    if (item == NULL) {
        printf("Item com codigo %d não encontrado!\n", codigo);
        return 0;
    }

    // Valida os dados
    if (quantidade < 0 || preco < 0) {
        printf("Erro: Dados invalidos!\n");
        return 0;
    }

    // Atualiza os campos
    if (nome != NULL && strlen(nome) > 0) {
        strncpy(item->nome, nome, MAX_NOME - 1);
        item->nome[MAX_NOME - 1] = '\0';
    }
    if (descricao != NULL && strlen(descricao) > 0) {
        strncpy(item->descricao, descricao, MAX_DESCRICAO - 1);
        item->descricao[MAX_DESCRICAO - 1] = '\0';
    }
    if (quantidade >= 0) {
        item->quantidade = quantidade;
    }
    if (preco >= 0) {
        item->preco = preco;
    }

    return 1;
}

// func para exibir um item
void exibir_item(Item *item) {
    if (item == NULL || item->ativo == 0) {
        return;
    }

    printf("\n========================================\n");
    printf("Codigo: %d\n", item->codigo);
    printf("Nome: %s\n", item->nome);
    printf("Descricao: %s\n", item->descricao);
    printf("Quantidade: %d\n", item->quantidade);
    printf("Preco: R$ %.2f\n", item->preco);
    printf("Valor Total: R$ %.2f\n", item->quantidade * item->preco);
    printf("========================================\n");
}

// func para listar todos os itens (incluindo removidos)
void listar_todos_itens(Inventario *inv) {
    if (inv == NULL || inv->total_itens == 0) {
        printf("Inventario vazio!\n");
        return;
    }

    int i, encontrados = 0;
    printf("\n=== LISTA COMPLETA DE ITENS ===\n");
    for (i = 0; i < inv->total_itens; i++) {
        if (inv->itens[i].ativo == 1) {
            exibir_item(&inv->itens[i]);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum item ativo encontrado!\n");
    } else {
        printf("\nTotal de itens ativos: %d\n", encontrados);
    }
}

// func para listar apenas itens ativos
void listar_itens_ativos(Inventario *inv) {
    listar_todos_itens(inv);
}

// func para exibir estatísticas do inventário
void exibir_estatisticas(Inventario *inv) {
    if (inv == NULL) {
        return;
    }

    int i, itens_ativos = 0;
    float valor_total = 0.0;

    for (i = 0; i < inv->total_itens; i++) {
        if (inv->itens[i].ativo == 1) {
            itens_ativos++;
            valor_total += inv->itens[i].quantidade * inv->itens[i].preco;
        }
    }

    printf("\n=== ESTATISTICAS DO INVENTÁRIO ===\n");
    printf("Total de itens cadastrados: %d\n", inv->total_itens);
    printf("Itens ativos: %d\n", itens_ativos);
    printf("Valor total do inventario: R$ %.2f\n", valor_total);
    printf("===================================\n\n");
}

// funcao para ordenar itens por código (c ponteiros)
void ordenar_por_codigo(Inventario *inv) {
    if (inv == NULL || inv->total_itens < 2) {
        return;
    }

    int i, j;
    Item temp;

    for (i = 0; i < inv->total_itens - 1; i++) {
        for (j = 0; j < inv->total_itens - i - 1; j++) {
            if (inv->itens[j].codigo > inv->itens[j + 1].codigo) {
                temp = inv->itens[j];
                inv->itens[j] = inv->itens[j + 1];
                inv->itens[j + 1] = temp;
            }
        }
    }
}

// Função para ordenar itens por nome (usando ponteiros)
void ordenar_por_nome(Inventario *inv) {
    if (inv == NULL || inv->total_itens < 2) {
        return;
    }

    int i, j;
    Item temp;

    for (i = 0; i < inv->total_itens - 1; i++) {
        for (j = 0; j < inv->total_itens - i - 1; j++) {
            if (strcmp(inv->itens[j].nome, inv->itens[j + 1].nome) > 0) {
                temp = inv->itens[j];
                inv->itens[j] = inv->itens[j + 1];
                inv->itens[j + 1] = temp;
            }
        }
    }
}



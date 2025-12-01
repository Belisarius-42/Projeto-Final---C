#include "inventario.h"


void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// menu
void exibir_menu() {
    printf("\n");
    printf("========================================\n");
    printf("   SISTEMA DE GESTAO DE INVENTARIO\n");
    printf("========================================\n");
    printf("1. Adicionar item\n");
    printf("2. Listar todos os itens\n");
    printf("3. Buscar item por codigo\n");
    printf("4. Buscar item por nome\n");
    printf("5. Atualizar item\n");
    printf("6. Remover item\n");
    printf("7. Exibir estatisticas\n");
    printf("8. Ordenar por codigo\n");
    printf("9. Ordenar por nome\n");
    printf("0. Sair\n");
    printf("========================================\n");
    printf("Escolha uma opcao: ");
}

// ler a string
void ler_string(char *buffer, int tamanho) {
    fgets(buffer, tamanho, stdin);
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

// add item
void menu_adicionar_item(Inventario *inv) {
    int codigo, quantidade;
    float preco;
    char nome[MAX_NOME];
    char descricao[MAX_DESCRICAO];

    printf("\n=== ADICIONAR ITEM ===\n");
    printf("Codigo: ");
    scanf("%d", &codigo);
    limpar_buffer();

    printf("Nome: ");
    ler_string(nome, MAX_NOME);

    printf("Descricao: ");
    ler_string(descricao, MAX_DESCRICAO);

    printf("Quantidade: ");
    scanf("%d", &quantidade);

    printf("Preco: ");
    scanf("%f", &preco);
    limpar_buffer();

    if (adicionar_item(inv, codigo, nome, descricao, quantidade, preco)) {
        printf("\nItem adicionado com sucesso!\n");
        salvar_inventario(inv);
    } else {
        printf("\nErro ao adicionar item!\n");
    }
}

//por por cod
void menu_buscar_por_codigo(Inventario *inv) {
    int codigo;
    Item *item;

    printf("\n=== BUSCAR ITEM POR CÓDIGO ===\n");
    printf("Digite o codigo: ");
    scanf("%d", &codigo);
    limpar_buffer();

    item = buscar_item(inv, codigo);
    if (item != NULL) {
        exibir_item(item);
    } else {
        printf("\nItem nao encontrado!\n");
    }
}

// busca por nome
void menu_buscar_por_nome(Inventario *inv) {
    char nome[MAX_NOME];
    Item *item;

    printf("\n=== BUSCAR ITEM POR NOME ===\n");
    printf("Digite o nome: ");
    ler_string(nome, MAX_NOME);

    item = buscar_item_por_nome(inv, nome);
    if (item != NULL) {
        exibir_item(item);
    } else {
        printf("\nItem não encontrado!\n");
    }
}

// Função para att item
void menu_atualizar_item(Inventario *inv) {
    int codigo, quantidade;
    float preco;
    char nome[MAX_NOME];
    char descricao[MAX_DESCRICAO];
    char opcao;

    printf("\n=== ATUALIZAR ITEM ===\n");
    printf("Digite o codigo do item: ");
    scanf("%d", &codigo);
    limpar_buffer();

    Item *item = buscar_item(inv, codigo);
    if (item == NULL) {
        printf("Item nao encontrado!\n");
        return;
    }

    printf("\nItem atual:\n");
    exibir_item(item);

    printf("\nDeseja atualizar o nome? (s/n): ");
    scanf("%c", &opcao);
    limpar_buffer();
    if (opcao == 's' || opcao == 'S') {
        printf("Novo nome: ");
        ler_string(nome, MAX_NOME);
    } else {
        strcpy(nome, "");
    }

    printf("Deseja atualizar a descricao? (s/n): ");
    scanf("%c", &opcao);
    limpar_buffer();
    if (opcao == 's' || opcao == 'S') {
        printf("Nova descricao: ");
        ler_string(descricao, MAX_DESCRICAO);
    } else {
        strcpy(descricao, "");
    }

    printf("Deseja atualizar a quantidade? (s/n): ");
    scanf("%c", &opcao);
    limpar_buffer();
    if (opcao == 's' || opcao == 'S') {
        printf("Nova quantidade: ");
        scanf("%d", &quantidade);
        limpar_buffer();
    } else {
        quantidade = -1;
    }

    printf("Deseja atualizar o preco? (s/n): ");
    scanf("%c", &opcao);
    limpar_buffer();
    if (opcao == 's' || opcao == 'S') {
        printf("Novo preco: ");
        scanf("%f", &preco);
        limpar_buffer();
    } else {
        preco = -1;
    }

    if (atualizar_item(inv, codigo,
                       strlen(nome) > 0 ? nome : NULL,
                       strlen(descricao) > 0 ? descricao : NULL,
                       quantidade, preco)) {
        printf("\nItem atualizado com sucesso!\n");
        salvar_inventario(inv);
    } else {
        printf("\nErro ao atualizar item!\n");
    }
}

// Função para remover item
void menu_remover_item(Inventario *inv) {
    int codigo;
    char confirmacao;

    printf("\n=== REMOVER ITEM ===\n");
    printf("Digite o codigo do item: ");
    scanf("%d", &codigo);
    limpar_buffer();

    Item *item = buscar_item(inv, codigo);
    if (item == NULL) {
        printf("Item nao encontrado!\n");
        return;
    }

    printf("\nItem a ser removido:\n");
    exibir_item(item);

    printf("\nTem certeza que deseja remover? (s/n): ");
    scanf("%c", &confirmacao);
    limpar_buffer();

    if (confirmacao == 's' || confirmacao == 'S') {
        if (remover_item(inv, codigo)) {
            printf("\nItem removido com sucesso!\n");
            salvar_inventario(inv);
        } else {
            printf("\nErro ao remover item!\n");
        }
    } else {
        printf("\nOperacao cancelada.\n");
    }
}

// main
int main() {
    Inventario *inventario;
    int opcao;

    printf("Inicializando sistema de inventario...\n");


    inventario = criar_inventario();
    if (inventario == NULL) {
        printf("Erro ao criar o inventario!\n");
        return 1;
    }


    if (carregar_inventario(inventario)) {
        printf("Dados carregados com sucesso!\n");
    } else {
        printf("Nenhum arquivo de dados encontrado. Criando novo inventario...\n");
    }

    // Loop principal do menu
    do {
        exibir_menu();
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                menu_adicionar_item(inventario);
                break;
            case 2:
                listar_itens_ativos(inventario);
                break;
            case 3:
                menu_buscar_por_codigo(inventario);
                break;
            case 4:
                menu_buscar_por_nome(inventario);
                break;
            case 5:
                menu_atualizar_item(inventario);
                break;
            case 6:
                menu_remover_item(inventario);
                break;
            case 7:
                exibir_estatisticas(inventario);
                break;
            case 8:
                ordenar_por_codigo(inventario);
                printf("\nItens ordenados por codigo!\n");
                salvar_inventario(inventario);
                break;
            case 9:
                ordenar_por_nome(inventario);
                printf("\nItens ordenados por nome!\n");
                salvar_inventario(inventario);
                break;
            case 0:
                printf("\nSalvando dados...\n");
                salvar_inventario(inventario);
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }

    } while (opcao != 0);

    // Libera a mem
    destruir_inventario(&inventario);

    return 0;
}



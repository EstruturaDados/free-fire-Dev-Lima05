/*
 * =====================================================
 *   MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA
 * =====================================================
 * Sistema de inventario para gerenciar itens coletados
 * pelo jogador durante a fase inicial de sobrevivencia.
 *
 * Estruturas utilizadas:
 *   - struct Item: armazena nome, tipo e quantidade
 *   - Vetor sequencial de até MAX_ITENS structs
 *
 * Funcoes principais:
 *   inserirItem(), removerItem(), listarItens(), buscarItem()
 * =====================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Capacidade maxima da mochila */
#define MAX_ITENS 10

/*
 * Struct Item: representa um objeto coletado pelo jogador.
 * Campos:
 *   nome     - identificador do item (ex: "KitMedico")
 *   tipo     - categoria do item (ex: "cura", "arma", "municao")
 *   quantidade - quantas unidades o jogador possui
 */
typedef struct {
    char nome[30];
    char tipo[20];
    int  quantidade;
} Item;

/* Vetor sequencial que representa a mochila do jogador */
Item mochila[MAX_ITENS];

/* Contador de itens atualmente na mochila */
int totalItens = 0;

/* =====================================================
 * Funcao: listarItens
 * Descricao: Exibe todos os itens armazenados na mochila
 *            em formato de tabela. Chamada apos cada operacao.
 * ===================================================== */
void listarItens() {
    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n", totalItens, MAX_ITENS);
    printf("--------------------------------------------------\n");
    printf("%-20s| %-15s| QUANTIDADE\n", "NOME", "TIPO");
    printf("--------------------------------------------------\n");

    if (totalItens == 0) {
        printf("  Mochila vazia.\n");
    } else {
        /* Percorre o vetor e exibe cada item */
        for (int i = 0; i < totalItens; i++) {
            printf("%-20s| %-15s| %d\n",
                   mochila[i].nome,
                   mochila[i].tipo,
                   mochila[i].quantidade);
        }
    }
    printf("--------------------------------------------------\n");
}

/* =====================================================
 * Funcao: inserirItem
 * Descricao: Cadastra um novo item na mochila.
 *            Verifica se ha espaco disponivel antes de inserir.
 *            Lê nome, tipo e quantidade fornecidos pelo jogador.
 * ===================================================== */
void inserirItem() {
    printf("\n--- Adicionar Novo Item ---\n");

    /* Verifica se a mochila atingiu a capacidade maxima */
    if (totalItens >= MAX_ITENS) {
        printf("Mochila cheia! Remova um item antes de adicionar outro.\n");
        return;
    }

    Item novoItem;

    /* Leitura do nome do item */
    printf("Nome do item: ");
    scanf("%29s", novoItem.nome);

    /* Leitura do tipo do item */
    printf("Tipo do item (arma, municao, cura, etc.): ");
    scanf("%19s", novoItem.tipo);

    /* Leitura da quantidade */
    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);

    /* Insere o item no proximo indice disponivel do vetor */
    mochila[totalItens] = novoItem;
    totalItens++;

    printf("\nItem '%s' adicionado com sucesso!\n", novoItem.nome);

    /* Lista os itens apos a insercao */
    listarItens();
}

/* =====================================================
 * Funcao: removerItem
 * Descricao: Remove um item da mochila pelo nome.
 *            Realiza busca sequencial e reorganiza o vetor
 *            apos a remocao para manter a lista compacta.
 * ===================================================== */
void removerItem() {
    printf("\n--- Remover Item ---\n");

    if (totalItens == 0) {
        printf("A mochila esta vazia. Nenhum item para remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item a remover: ");
    scanf("%29s", nomeBusca);

    int indiceEncontrado = -1;

    /* Busca sequencial pelo nome do item */
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Item '%s' nao encontrado na mochila.\n", nomeBusca);
        return;
    }

    /* Reorganiza o vetor deslocando os elementos seguintes */
    for (int i = indiceEncontrado; i < totalItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    totalItens--;

    printf("Item '%s' removido com sucesso!\n", nomeBusca);

    /* Lista os itens apos a remocao */
    listarItens();
}

/* =====================================================
 * Funcao: buscarItem
 * Descricao: Realiza busca sequencial por nome e exibe
 *            os dados do item, se encontrado.
 * ===================================================== */
void buscarItem() {
    printf("\n--- Buscar Item ---\n");

    if (totalItens == 0) {
        printf("A mochila esta vazia. Nenhum item para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item a buscar: ");
    scanf("%29s", nomeBusca);

    /* Busca sequencial: percorre o vetor do inicio ao fim */
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\nItem encontrado:\n");
            printf("--------------------------------------------------\n");
            printf("%-20s| %-15s| QUANTIDADE\n", "NOME", "TIPO");
            printf("--------------------------------------------------\n");
            printf("%-20s| %-15s| %d\n",
                   mochila[i].nome,
                   mochila[i].tipo,
                   mochila[i].quantidade);
            printf("--------------------------------------------------\n");
            return;
        }
    }

    printf("Item '%s' nao encontrado na mochila.\n", nomeBusca);
}

/* =====================================================
 * Funcao: exibirMenu
 * Descricao: Exibe o cabecalho e o menu principal do
 *            sistema, informando o estado atual da mochila.
 * ===================================================== */
void exibirMenu() {
    printf("\n========================================\n");
    printf("   MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
    printf("========================================\n");
    printf("Itens na Mochila: %d/%d\n\n", totalItens, MAX_ITENS);
    printf("1. Adicionar Item (Loot)\n");
    printf("2. Remover Item\n");
    printf("3. Listar Itens na Mochila\n");
    printf("4. Buscar Item\n");
    printf("0. Sair\n");
    printf("----------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/* =====================================================
 * Funcao: main
 * Descricao: Ponto de entrada do programa.
 *            Controla o fluxo principal via menu interativo.
 * ===================================================== */
int main() {
    int opcao;

    /* Loop principal: continua ate o jogador escolher sair (0) */
    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarItem();
                break;
            case 0:
                printf("\nSaindo do sistema de inventario. Boa sorte, sobrevivente!\n");
                break;
            default:
                printf("\nOpcao invalida! Escolha entre 0 e 4.\n");
        }

        /* Pausa para o jogador ler o resultado antes de voltar ao menu */
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            /* Consome o '\n' pendente no buffer e aguarda nova entrada */
            getchar();
            getchar();
        }

    } while (opcao != 0);

    return 0;
}
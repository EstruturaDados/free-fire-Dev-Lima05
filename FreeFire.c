/*
 * MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA (Nivel 2)
 * Compara duas estruturas de dados: Vetor e Lista Encadeada.
 * Ambas implementam: inserir, remover, listar e busca sequencial.
 * Exclusivo do Vetor: ordenacao (Bubble Sort) e busca binaria.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10  /* capacidade maxima do vetor */

/* Representa um item da mochila (nome, tipo e quantidade) */
typedef struct {
    char nome[30];
    char tipo[20];
    int  quantidade;
} Item;

/* No da lista encadeada: guarda um Item e aponta para o proximo no */
typedef struct No {
    Item        dados;
    struct No  *proximo;
} No;

/* --- variaveis globais do VETOR --- */
Item mochilaVetor[MAX_ITENS]; /* array que armazena os itens         */
int  totalVetor    = 0;       /* quantos itens estao no vetor        */
int  vetorOrdenado = 0;       /* 1 = ordenado, 0 = fora de ordem     */

/* --- variaveis globais da LISTA --- */
No  *cabecaLista = NULL;  /* primeiro no da lista (NULL = vazia) */
int  totalLista  = 0;     /* quantos nos existem na lista        */


/* ======================================================
 * FUNCOES AUXILIARES
 * ====================================================== */

/* Imprime os itens em formato de tabela (NOME | TIPO | QUANTIDADE) */
void exibirTabela(Item itens[], int total) {
    printf("--------------------------------------------------\n");
    printf("%-20s| %-15s| QUANTIDADE\n", "NOME", "TIPO");
    printf("--------------------------------------------------\n");
    if (total == 0) {
        printf("  (mochila vazia)\n");
    } else {
        for (int i = 0; i < total; i++)
            printf("%-20s| %-15s| %d\n",
                   itens[i].nome, itens[i].tipo, itens[i].quantidade);
    }
    printf("--------------------------------------------------\n");
}

/* Le nome, tipo e quantidade do teclado e retorna um Item preenchido */
Item lerItem() {
    Item it;
    printf("  Nome do item: ");
    scanf("%29s", it.nome);
    printf("  Tipo do item (arma, municao, cura, etc.): ");
    scanf("%19s", it.tipo);
    printf("  Quantidade: ");
    scanf("%d", &it.quantidade);
    return it;
}

/* Le apenas o nome do item (usado em busca e remocao) */
void lerNome(char *buf, int tam) {
    printf("  Nome do item: ");
    scanf("%29s", buf);
    (void)tam;
}


/* ======================================================
 * SECAO A — VETOR
 * ====================================================== */

/* Adiciona um item no proximo espaco livre do vetor */
void inserirItemVetor(Item novoItem) {
    if (totalVetor >= MAX_ITENS) {
        printf("  [VETOR] Mochila cheia! Remova um item antes de adicionar.\n");
        return;
    }
    mochilaVetor[totalVetor] = novoItem;
    totalVetor++;
    vetorOrdenado = 0; /* nova insercao bagunca a ordem */
    printf("  [VETOR] Item '%s' adicionado com sucesso!\n", novoItem.nome);
}

/* Remove um item pelo nome e fecha o espaco vazio deslocando os outros */
void removerItemVetor(char *nome) {
    int idx = -1;
    for (int i = 0; i < totalVetor; i++) {
        if (strcmp(mochilaVetor[i].nome, nome) == 0) { idx = i; break; }
    }
    if (idx == -1) {
        printf("  [VETOR] Item '%s' nao encontrado.\n", nome);
        return;
    }
    for (int i = idx; i < totalVetor - 1; i++) /* desloca os itens seguintes */
        mochilaVetor[i] = mochilaVetor[i + 1];
    totalVetor--;
    vetorOrdenado = 0;
    printf("  [VETOR] Item '%s' removido com sucesso!\n", nome);
}

/* Exibe todos os itens do vetor em tabela */
void listarItensVetor() {
    printf("\n--- ITENS NA MOCHILA - VETOR (%d/%d) ---\n", totalVetor, MAX_ITENS);
    exibirTabela(mochilaVetor, totalVetor);
}

/* Percorre o vetor do inicio ao fim procurando pelo nome; conta comparacoes */
void buscarSequencialVetor(char *nome) {
    int comparacoes = 0;
    printf("\n  [VETOR] Busca Sequencial por '%s'...\n", nome);
    for (int i = 0; i < totalVetor; i++) {
        comparacoes++;
        if (strcmp(mochilaVetor[i].nome, nome) == 0) {
            printf("  Encontrado na posicao %d | Comparacoes: %d\n", i, comparacoes);
            Item aux[1] = { mochilaVetor[i] };
            exibirTabela(aux, 1);
            return;
        }
    }
    printf("  Item nao encontrado. Comparacoes realizadas: %d\n", comparacoes);
}

/* Ordena o vetor alfabeticamente por nome usando Bubble Sort */
void ordenarVetor() {
    int trocou;
    Item temp;
    for (int i = 0; i < totalVetor - 1; i++) {
        trocou = 0;
        for (int j = 0; j < totalVetor - 1 - i; j++) {
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[j + 1].nome) > 0) {
                temp = mochilaVetor[j];
                mochilaVetor[j]   = mochilaVetor[j + 1];
                mochilaVetor[j+1] = temp;
                trocou = 1;
            }
        }
        if (!trocou) break; /* para cedo se ja estiver ordenado */
    }
    vetorOrdenado = 1;
    printf("  [VETOR] Mochila ordenada por nome (Bubble Sort).\n");
    listarItensVetor();
}

/* Busca pelo nome dividindo o vetor ao meio a cada passo (exige ordenacao) */
void buscarBinariaVetor(char *nome) {
    if (!vetorOrdenado) {
        printf("  [VETOR] Ordene a mochila antes de usar busca binaria!\n");
        return;
    }
    int inicio = 0, fim = totalVetor - 1, meio, comparacoes = 0;
    printf("\n  [VETOR] Busca Binaria por '%s'...\n", nome);
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        comparacoes++;
        int cmp = strcmp(mochilaVetor[meio].nome, nome);
        if (cmp == 0) {
            printf("  Encontrado na posicao %d | Comparacoes: %d\n", meio, comparacoes);
            Item aux[1] = { mochilaVetor[meio] };
            exibirTabela(aux, 1);
            return;
        } else if (cmp < 0) inicio = meio + 1; /* busca na metade direita  */
        else                 fim   = meio - 1; /* busca na metade esquerda */
    }
    printf("  Item nao encontrado. Comparacoes realizadas: %d\n", comparacoes);
}


/* ======================================================
 * SECAO B — LISTA ENCADEADA
 * ====================================================== */

/* Aloca um novo no e o insere no inicio da lista */
void inserirItemLista(Item novoItem) {
    No *novoNo = (No *) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("  [LISTA] Erro: memoria insuficiente.\n");
        return;
    }
    novoNo->dados   = novoItem;
    novoNo->proximo = cabecaLista; /* aponta para o antigo primeiro no */
    cabecaLista     = novoNo;      /* o novo no vira o primeiro        */
    totalLista++;
    printf("  [LISTA] Item '%s' adicionado com sucesso!\n", novoItem.nome);
}

/* Busca o no pelo nome, ajusta os ponteiros para remove-lo e libera memoria */
void removerItemLista(char *nome) {
    No *atual = cabecaLista, *anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome) == 0) {
            if (anterior == NULL) cabecaLista       = atual->proximo;
            else                  anterior->proximo = atual->proximo;
            free(atual); /* libera o espaco alocado com malloc */
            totalLista--;
            printf("  [LISTA] Item '%s' removido com sucesso!\n", nome);
            return;
        }
        anterior = atual;
        atual    = atual->proximo;
    }
    printf("  [LISTA] Item '%s' nao encontrado.\n", nome);
}

/* Percorre todos os nos da lista e exibe em tabela */
void listarItensLista() {
    printf("\n--- ITENS NA MOCHILA - LISTA ENCADEADA (%d nos) ---\n", totalLista);
    printf("--------------------------------------------------\n");
    printf("%-20s| %-15s| QUANTIDADE\n", "NOME", "TIPO");
    printf("--------------------------------------------------\n");
    if (cabecaLista == NULL) {
        printf("  (lista vazia)\n");
    } else {
        No *atual = cabecaLista;
        while (atual != NULL) {
            printf("%-20s| %-15s| %d\n",
                   atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
            atual = atual->proximo;
        }
    }
    printf("--------------------------------------------------\n");
}

/* Percorre os nos um a um procurando pelo nome; conta comparacoes */
void buscarSequencialLista(char *nome) {
    int comparacoes = 0;
    printf("\n  [LISTA] Busca Sequencial por '%s'...\n", nome);
    No *atual = cabecaLista;
    while (atual != NULL) {
        comparacoes++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            printf("  Encontrado! Comparacoes: %d\n", comparacoes);
            Item aux[1] = { atual->dados };
            exibirTabela(aux, 1);
            return;
        }
        atual = atual->proximo;
    }
    printf("  Item nao encontrado. Comparacoes realizadas: %d\n", comparacoes);
}

/* Libera toda a memoria alocada pela lista (evita vazamento ao sair) */
void liberarLista() {
    No *atual = cabecaLista;
    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    cabecaLista = NULL;
    totalLista  = 0;
}


/* ======================================================
 * SECAO C — MENUS
 * ====================================================== */

/* Submenu do vetor: exibe opcoes e chama a funcao correspondente */
void menuVetor() {
    int op;
    char nome[30];
    do {
        printf("\n  === MOCHILA - VETOR (%d/%d) ===\n", totalVetor, MAX_ITENS);
        printf("  1. Adicionar item\n");
        printf("  2. Remover item\n");
        printf("  3. Listar itens\n");
        printf("  4. Busca Sequencial\n");
        printf("  5. Ordenar (Bubble Sort)\n");
        printf("  6. Busca Binaria (requer ordenacao)\n");
        printf("  0. Voltar\n");
        printf("  Opcao: ");
        scanf("%d", &op);

        clock_t inicio = clock(); /* marca o inicio para medir o tempo */

        switch (op) {
            case 1: inserirItemVetor(lerItem()); listarItensVetor(); break;
            case 2: lerNome(nome,30); removerItemVetor(nome); listarItensVetor(); break;
            case 3: listarItensVetor(); break;
            case 4: lerNome(nome,30); buscarSequencialVetor(nome); break;
            case 5: ordenarVetor(); break;
            case 6: lerNome(nome,30); buscarBinariaVetor(nome); break;
            case 0: break;
            default: printf("  Opcao invalida.\n");
        }

        if (op != 0) {
            double ms = (double)(clock() - inicio) / CLOCKS_PER_SEC * 1000.0;
            printf("  Tempo da operacao: %.3f ms\n", ms);
            printf("\n  Pressione Enter para continuar...");
            getchar(); getchar();
        }
    } while (op != 0);
}

/* Submenu da lista encadeada: exibe opcoes e chama a funcao correspondente */
void menuLista() {
    int op;
    char nome[30];
    do {
        printf("\n  === MOCHILA - LISTA ENCADEADA (%d nos) ===\n", totalLista);
        printf("  1. Adicionar item\n");
        printf("  2. Remover item\n");
        printf("  3. Listar itens\n");
        printf("  4. Busca Sequencial\n");
        printf("  0. Voltar\n");
        printf("  Opcao: ");
        scanf("%d", &op);

        clock_t inicio = clock();

        switch (op) {
            case 1: inserirItemLista(lerItem()); listarItensLista(); break;
            case 2: lerNome(nome,30); removerItemLista(nome); listarItensLista(); break;
            case 3: listarItensLista(); break;
            case 4: lerNome(nome,30); buscarSequencialLista(nome); break;
            case 0: break;
            default: printf("  Opcao invalida.\n");
        }

        if (op != 0) {
            double ms = (double)(clock() - inicio) / CLOCKS_PER_SEC * 1000.0;
            printf("  Tempo da operacao: %.3f ms\n", ms);
            printf("\n  Pressione Enter para continuar...");
            getchar(); getchar();
        }
    } while (op != 0);
}

/* Mostra uma tabela comparando a complexidade das operacoes em cada estrutura */
void exibirComparativo() {
    printf("\n========================================================\n");
    printf("  COMPARATIVO: VETOR x LISTA ENCADEADA\n");
    printf("========================================================\n");
    printf("  %-22s | %-15s | %-12s\n", "Operacao",          "Vetor",           "Lista");
    printf("  %-22s | %-15s | %-12s\n", "Acesso por indice", "O(1)",            "O(n)");
    printf("  %-22s | %-15s | %-12s\n", "Insercao",          "O(1)*",           "O(1) inicio");
    printf("  %-22s | %-15s | %-12s\n", "Remocao por nome",  "O(n)",            "O(n)");
    printf("  %-22s | %-15s | %-12s\n", "Busca sequencial",  "O(n)",            "O(n)");
    printf("  %-22s | %-15s | %-12s\n", "Busca binaria",     "O(log n)**",      "Nao suporta");
    printf("  %-22s | %-15s | %-12s\n", "Memoria",           "Fixo",            "Dinamico");
    printf("  %-22s | %-15s | %-12s\n", "Ordenacao",         "Sim (BubbleSort)","Custosa");
    printf("  * O(1) amortizado  ** requer ordenacao previa\n");
    printf("========================================================\n");
    printf("  Itens atuais — Vetor: %d | Lista: %d\n", totalVetor, totalLista);
}

/* Ponto de entrada: exibe o menu principal e direciona para vetor ou lista */
int main() {
    int opcao;
    do {
        printf("\n========================================\n");
        printf("   MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
        printf("========================================\n");
        printf("  Vetor: %d/%d itens  |  Lista: %d nos\n\n", totalVetor, MAX_ITENS, totalLista);
        printf("  1. Gerenciar Mochila com VETOR\n");
        printf("  2. Gerenciar Mochila com LISTA ENCADEADA\n");
        printf("  3. Ver Comparativo de Desempenho\n");
        printf("  0. Sair\n");
        printf("----------------------------------------\n");
        printf("  Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: menuVetor();         break;
            case 2: menuLista();         break;
            case 3: exibirComparativo(); break;
            case 0:
                liberarLista(); /* libera a memoria da lista antes de encerrar */
                printf("\nBoa sorte, sobrevivente!\n");
                break;
            default:
                printf("  Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

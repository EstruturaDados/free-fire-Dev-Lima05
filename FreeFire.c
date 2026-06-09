/*
 * =====================================================================
 *  MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA
 *  Nivel 1 : struct Item + vetor sequencial (CRUD basico)
 *  Nivel 2 : vetor vs lista encadeada + bubble sort + busca binaria
 *  Nivel 3 : Torre de Fuga — bubble/insertion/selection sort
 *            comparacao de desempenho, busca binaria por componente-chave
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ===================================================================
 * CONSTANTES
 * =================================================================== */
#define MAX_ITENS       10   /* capacidade da mochila (niveis 1 e 2)  */
#define MAX_COMPONENTES 20   /* capacidade da torre   (nivel 3)       */

/* ===================================================================
 * STRUCTS
 * =================================================================== */

/* Item: usado nos niveis 1 e 2 (mochila do jogador) */
typedef struct {
    char nome[30];
    char tipo[20];
    int  quantidade;
} Item;

/* No: elemento da lista encadeada (nivel 2) */
typedef struct No {
    Item        dados;
    struct No  *proximo;
} No;

/* Componente: peca necessaria para montar a torre de fuga (nivel 3) */
typedef struct {
    char nome[30];       /* ex: "chip-central"   */
    char tipo[20];       /* ex: "controle"       */
    int  prioridade;     /* 1 (baixa) a 10 (alta)*/
} Componente;


/* ===================================================================
 * VARIAVEIS GLOBAIS — NIVEIS 1 e 2
 * =================================================================== */
Item mochilaVetor[MAX_ITENS];
int  totalVetor    = 0;
int  vetorOrdenado = 0;

No  *cabecaLista = NULL;
int  totalLista  = 0;


/* ===================================================================
 * VARIAVEIS GLOBAIS — NIVEL 3
 * =================================================================== */
Componente torre[MAX_COMPONENTES]; /* vetor de componentes da torre      */
int  totalTorre        = 0;        /* quantos componentes foram inseridos */
int  torreOrdenadaNome = 0;        /* 1 = ordenada por nome (bubble sort) */


/* ===================================================================
 * SECAO AUXILIAR — compartilhada entre niveis
 * =================================================================== */

/* Imprime itens da mochila em tabela formatada */
void exibirTabela(Item itens[], int total) {
    printf("--------------------------------------------------\n");
    printf("%-20s| %-15s| QUANTIDADE\n", "NOME", "TIPO");
    printf("--------------------------------------------------\n");
    if (total == 0) printf("  (mochila vazia)\n");
    else for (int i = 0; i < total; i++)
        printf("%-20s| %-15s| %d\n", itens[i].nome, itens[i].tipo, itens[i].quantidade);
    printf("--------------------------------------------------\n");
}

/* Le nome, tipo e quantidade do teclado e devolve um Item */
Item lerItem() {
    Item it;
    printf("  Nome do item: ");    scanf("%29s", it.nome);
    printf("  Tipo (arma, municao, cura, etc.): "); scanf("%19s", it.tipo);
    printf("  Quantidade: ");      scanf("%d", &it.quantidade);
    return it;
}

/* Le apenas o nome (usado em busca e remocao) */
void lerNome(char *buf, int tam) {
    printf("  Nome do item: ");
    scanf("%29s", buf);
    (void)tam;
}


/* ===================================================================
 * SECAO A — VETOR (Nivel 2)
 * =================================================================== */

/* Insere um item no proximo indice livre do vetor */
void inserirItemVetor(Item novoItem) {
    if (totalVetor >= MAX_ITENS) {
        printf("  [VETOR] Mochila cheia!\n"); return;
    }
    mochilaVetor[totalVetor++] = novoItem;
    vetorOrdenado = 0;
    printf("  [VETOR] '%s' adicionado!\n", novoItem.nome);
}

/* Remove item pelo nome e desloca os seguintes para fechar o espaco */
void removerItemVetor(char *nome) {
    int idx = -1;
    for (int i = 0; i < totalVetor; i++)
        if (strcmp(mochilaVetor[i].nome, nome) == 0) { idx = i; break; }
    if (idx == -1) { printf("  [VETOR] '%s' nao encontrado.\n", nome); return; }
    for (int i = idx; i < totalVetor - 1; i++)
        mochilaVetor[i] = mochilaVetor[i + 1];
    totalVetor--;
    vetorOrdenado = 0;
    printf("  [VETOR] '%s' removido!\n", nome);
}

/* Exibe todos os itens do vetor */
void listarItensVetor() {
    printf("\n--- MOCHILA - VETOR (%d/%d) ---\n", totalVetor, MAX_ITENS);
    exibirTabela(mochilaVetor, totalVetor);
}

/* Busca sequencial no vetor: percorre do inicio ao fim e conta comparacoes */
void buscarSequencialVetor(char *nome) {
    int cmp = 0;
    printf("\n  [VETOR] Busca Sequencial: '%s'\n", nome);
    for (int i = 0; i < totalVetor; i++) {
        cmp++;
        if (strcmp(mochilaVetor[i].nome, nome) == 0) {
            printf("  Encontrado (pos %d) | Comparacoes: %d\n", i, cmp);
            Item aux[1] = { mochilaVetor[i] }; exibirTabela(aux, 1); return;
        }
    }
    printf("  Nao encontrado. Comparacoes: %d\n", cmp);
}

/* Ordena o vetor por nome (Bubble Sort) e ativa a flag de ordenado */
void ordenarVetor() {
    int trocou; Item temp;
    for (int i = 0; i < totalVetor - 1; i++) {
        trocou = 0;
        for (int j = 0; j < totalVetor - 1 - i; j++) {
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[j+1].nome) > 0) {
                temp = mochilaVetor[j];
                mochilaVetor[j] = mochilaVetor[j+1];
                mochilaVetor[j+1] = temp;
                trocou = 1;
            }
        }
        if (!trocou) break; /* ja ordenado: encerra cedo */
    }
    vetorOrdenado = 1;
    printf("  [VETOR] Ordenado por nome (Bubble Sort).\n");
    listarItensVetor();
}

/* Busca binaria no vetor (so funciona apos ordenarVetor): divide o espaco ao meio */
void buscarBinariaVetor(char *nome) {
    if (!vetorOrdenado) { printf("  Ordene antes de usar busca binaria!\n"); return; }
    int ini = 0, fim = totalVetor - 1, meio, cmp = 0;
    printf("\n  [VETOR] Busca Binaria: '%s'\n", nome);
    while (ini <= fim) {
        meio = (ini + fim) / 2; cmp++;
        int r = strcmp(mochilaVetor[meio].nome, nome);
        if (r == 0) {
            printf("  Encontrado (pos %d) | Comparacoes: %d\n", meio, cmp);
            Item aux[1] = { mochilaVetor[meio] }; exibirTabela(aux, 1); return;
        }
        if (r < 0) ini = meio + 1; else fim = meio - 1;
    }
    printf("  Nao encontrado. Comparacoes: %d\n", cmp);
}


/* ===================================================================
 * SECAO B — LISTA ENCADEADA (Nivel 2)
 * =================================================================== */

/* Aloca um no e o insere no inicio da lista (O(1)) */
void inserirItemLista(Item novoItem) {
    No *n = (No *) malloc(sizeof(No));
    if (!n) { printf("  [LISTA] Sem memoria.\n"); return; }
    n->dados = novoItem; n->proximo = cabecaLista; cabecaLista = n;
    totalLista++;
    printf("  [LISTA] '%s' adicionado!\n", novoItem.nome);
}

/* Percorre a lista, desconecta o no pelo nome e libera a memoria */
void removerItemLista(char *nome) {
    No *at = cabecaLista, *ant = NULL;
    while (at) {
        if (strcmp(at->dados.nome, nome) == 0) {
            if (ant) ant->proximo = at->proximo; else cabecaLista = at->proximo;
            free(at); totalLista--;
            printf("  [LISTA] '%s' removido!\n", nome); return;
        }
        ant = at; at = at->proximo;
    }
    printf("  [LISTA] '%s' nao encontrado.\n", nome);
}

/* Percorre e imprime todos os nos da lista */
void listarItensLista() {
    printf("\n--- MOCHILA - LISTA ENCADEADA (%d nos) ---\n", totalLista);
    printf("--------------------------------------------------\n");
    printf("%-20s| %-15s| QUANTIDADE\n", "NOME", "TIPO");
    printf("--------------------------------------------------\n");
    No *at = cabecaLista;
    if (!at) printf("  (lista vazia)\n");
    while (at) {
        printf("%-20s| %-15s| %d\n", at->dados.nome, at->dados.tipo, at->dados.quantidade);
        at = at->proximo;
    }
    printf("--------------------------------------------------\n");
}

/* Busca sequencial na lista: no a no, contando cada comparacao */
void buscarSequencialLista(char *nome) {
    int cmp = 0;
    printf("\n  [LISTA] Busca Sequencial: '%s'\n", nome);
    No *at = cabecaLista;
    while (at) {
        cmp++;
        if (strcmp(at->dados.nome, nome) == 0) {
            printf("  Encontrado! Comparacoes: %d\n", cmp);
            Item aux[1] = { at->dados }; exibirTabela(aux, 1); return;
        }
        at = at->proximo;
    }
    printf("  Nao encontrado. Comparacoes: %d\n", cmp);
}

/* Percorre e libera todos os nos (chame antes de encerrar o programa) */
void liberarLista() {
    No *at = cabecaLista;
    while (at) { No *prox = at->proximo; free(at); at = prox; }
    cabecaLista = NULL; totalLista = 0;
}


/* ===================================================================
 * SECAO D — TORRE DE FUGA  (Nivel Mestre)
 * ===================================================================
 * Tres algoritmos de ordenacao, cada um com criterio diferente:
 *   bubbleSortNome      — ordena por nome        (string)
 *   insertionSortTipo   — ordena por tipo        (string)
 *   selectionSortPrioridade — ordena por prioridade (int, decrescente)
 *
 * medirTempo: executa qualquer algoritmo acima e exibe tempo + comparacoes.
 * buscaBinariaPorNome: busca binaria no vetor ordenado por nome.
 * mostrarComponentes: exibe o vetor formatado com todos os campos.
 * =================================================================== */

/* Contador global de comparacoes — zerado antes de cada algoritmo */
int gComparacoes = 0;

/* Imprime os componentes da torre em tabela (NOME | TIPO | PRIORIDADE) */
void mostrarComponentes(Componente v[], int n) {
    printf("\n  %-20s| %-15s| PRIORIDADE\n", "NOME", "TIPO");
    printf("  --------------------------------------------------\n");
    if (n == 0) { printf("  (nenhum componente cadastrado)\n"); return; }
    for (int i = 0; i < n; i++)
        printf("  %-20s| %-15s| %d\n", v[i].nome, v[i].tipo, v[i].prioridade);
    printf("  --------------------------------------------------\n");
}

/* ---- Bubble Sort por nome (string) ----
 * Compara pares adjacentes e troca se estiver fora de ordem alfabetica.
 * Para cedo se nenhuma troca ocorrer na passagem (otimizacao). */
void bubbleSortNome(Componente v[], int n) {
    gComparacoes = 0;
    Componente tmp;
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            gComparacoes++;
            if (strcmp(v[j].nome, v[j+1].nome) > 0) {
                tmp = v[j]; v[j] = v[j+1]; v[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
    torreOrdenadaNome = 1; /* habilita busca binaria por nome */
}

/* ---- Insertion Sort por tipo (string) ----
 * Pega cada elemento e o insere na posicao correta dentro da parte ja ordenada.
 * Eficiente para listas quase ordenadas. */
void insertionSortTipo(Componente v[], int n) {
    gComparacoes = 0;
    torreOrdenadaNome = 0; /* ordenar por tipo desabilita busca binaria por nome */
    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;
        while (j >= 0 && (gComparacoes++, strcmp(v[j].tipo, chave.tipo) > 0)) {
            v[j+1] = v[j];
            j--;
        }
        v[j+1] = chave;
    }
}

/* ---- Selection Sort por prioridade (int, decrescente) ----
 * A cada passagem, encontra o elemento de maior prioridade e o move para a frente.
 * Numero de trocas e minimo (exatamente n-1). */
void selectionSortPrioridade(Componente v[], int n) {
    gComparacoes = 0;
    torreOrdenadaNome = 0;
    for (int i = 0; i < n - 1; i++) {
        int idxMax = i;
        for (int j = i + 1; j < n; j++) {
            gComparacoes++;
            if (v[j].prioridade > v[idxMax].prioridade) idxMax = j;
        }
        if (idxMax != i) {
            Componente tmp = v[i]; v[i] = v[idxMax]; v[idxMax] = tmp;
        }
    }
}

/* ---- medirTempo ----
 * Recebe um ponteiro para qualquer dos tres algoritmos acima,
 * executa sobre o vetor e exibe tempo de execucao e comparacoes. */
void medirTempo(void (*algoritmo)(Componente[], int), Componente v[], int n) {
    clock_t ini = clock();
    algoritmo(v, n);
    double ms = (double)(clock() - ini) / CLOCKS_PER_SEC * 1000.0;
    printf("  Comparacoes: %d | Tempo: %.4f ms\n", gComparacoes, ms);
}

/* ---- buscaBinariaPorNome ----
 * Divide o vetor ordenado por nome ao meio a cada passo.
 * Muito mais rapido que busca sequencial para vetores grandes.
 * Exige que bubbleSortNome tenha sido chamado antes. */
void buscaBinariaPorNome(Componente v[], int n, char nome[]) {
    if (!torreOrdenadaNome) {
        printf("  [!] Ordene por NOME (Bubble Sort) antes de usar busca binaria.\n");
        return;
    }
    int ini = 0, fim = n - 1, meio, cmp = 0;
    printf("\n  Busca Binaria por componente-chave: '%s'\n", nome);
    while (ini <= fim) {
        meio = (ini + fim) / 2; cmp++;
        int r = strcmp(v[meio].nome, nome);
        if (r == 0) {
            printf("  >>> COMPONENTE-CHAVE ENCONTRADO! Posicao %d | Comparacoes: %d\n",
                   meio, cmp);
            printf("  Nome: %-20s Tipo: %-15s Prioridade: %d\n",
                   v[meio].nome, v[meio].tipo, v[meio].prioridade);
            printf("  [TORRE ATIVADA] Missao de fuga iniciada!\n");
            return;
        }
        if (r < 0) ini = meio + 1; else fim = meio - 1;
    }
    printf("  Componente '%s' nao encontrado. Comparacoes: %d\n", nome, cmp);
    printf("  [ATENCAO] Torre nao pode ser ativada sem o componente-chave!\n");
}

/* ---- cadastrarComponente ----
 * Le nome, tipo e prioridade do teclado e insere no vetor da torre. */
void cadastrarComponente() {
    if (totalTorre >= MAX_COMPONENTES) {
        printf("  Torre ja tem %d componentes (limite atingido).\n", MAX_COMPONENTES);
        return;
    }
    Componente c;
    printf("  Nome do componente: ");
    scanf("%29s", c.nome);
    printf("  Tipo (controle, suporte, propulsao, etc.): ");
    scanf("%19s", c.tipo);
    do {
        printf("  Prioridade (1 a 10): ");
        scanf("%d", &c.prioridade);
        if (c.prioridade < 1 || c.prioridade > 10)
            printf("  Valor invalido. Digite entre 1 e 10.\n");
    } while (c.prioridade < 1 || c.prioridade > 10);

    torre[totalTorre++] = c;
    torreOrdenadaNome = 0; /* nova insercao invalida a ordenacao */
    printf("  Componente '%s' adicionado a torre!\n", c.nome);
}

/* ---- exibirRelatorioFinal ----
 * Exibe o estado atual da torre e confirma se o componente-chave esta presente. */
void exibirRelatorioFinal(char *chave) {
    printf("\n  ============================================\n");
    printf("  RELATORIO FINAL — TORRE DE FUGA\n");
    printf("  ============================================\n");
    printf("  Total de componentes: %d/%d\n", totalTorre, MAX_COMPONENTES);
    mostrarComponentes(torre, totalTorre);

    /* Verifica presenca do componente-chave por busca sequencial */
    int encontrado = 0;
    for (int i = 0; i < totalTorre; i++)
        if (strcmp(torre[i].nome, chave) == 0) { encontrado = 1; break; }

    printf("\n  Componente-chave '%s': %s\n", chave,
           encontrado ? "[PRESENTE] Torre pode ser ativada!"
                      : "[AUSENTE]  Adicione o componente-chave!");
    printf("  ============================================\n");
}

/* ---- menuTorre ----
 * Submenu completo do nivel mestre: cadastro, ordenacao, busca e relatorio. */
void menuTorre() {
    int op;
    char chave[30] = "chip-central"; /* componente padrao para ativar a torre */

    do {
        printf("\n  ==========================================\n");
        printf("  TORRE DE FUGA — NIVEL MESTRE (%d/%d pecas)\n",
               totalTorre, MAX_COMPONENTES);
        printf("  ==========================================\n");
        printf("  1. Cadastrar componente\n");
        printf("  2. Listar componentes\n");
        printf("  3. Ordenar por NOME        (Bubble Sort)\n");
        printf("  4. Ordenar por TIPO        (Insertion Sort)\n");
        printf("  5. Ordenar por PRIORIDADE  (Selection Sort)\n");
        printf("  6. Busca Binaria por nome  (requer opcao 3)\n");
        printf("  7. Definir componente-chave  [atual: %s]\n", chave);
        printf("  8. Relatorio final da torre\n");
        printf("  0. Voltar\n");
        printf("  Opcao: ");
        scanf("%d", &op);

        switch (op) {

            case 1:
                cadastrarComponente();
                break;

            case 2:
                printf("\n--- COMPONENTES DA TORRE (%d/%d) ---\n",
                       totalTorre, MAX_COMPONENTES);
                mostrarComponentes(torre, totalTorre);
                break;

            case 3:
                if (totalTorre == 0) { printf("  Nenhum componente cadastrado.\n"); break; }
                printf("  Executando Bubble Sort por nome...\n");
                medirTempo(bubbleSortNome, torre, totalTorre);
                printf("  Resultado apos ordenacao:\n");
                mostrarComponentes(torre, totalTorre);
                break;

            case 4:
                if (totalTorre == 0) { printf("  Nenhum componente cadastrado.\n"); break; }
                printf("  Executando Insertion Sort por tipo...\n");
                medirTempo(insertionSortTipo, torre, totalTorre);
                printf("  Resultado apos ordenacao:\n");
                mostrarComponentes(torre, totalTorre);
                break;

            case 5:
                if (totalTorre == 0) { printf("  Nenhum componente cadastrado.\n"); break; }
                printf("  Executando Selection Sort por prioridade (maior primeiro)...\n");
                medirTempo(selectionSortPrioridade, torre, totalTorre);
                printf("  Resultado apos ordenacao:\n");
                mostrarComponentes(torre, totalTorre);
                break;

            case 6:
                if (totalTorre == 0) { printf("  Nenhum componente cadastrado.\n"); break; }
                buscaBinariaPorNome(torre, totalTorre, chave);
                break;

            case 7:
                printf("  Nome do novo componente-chave: ");
                scanf("%29s", chave);
                printf("  Componente-chave definido como '%s'.\n", chave);
                break;

            case 8:
                exibirRelatorioFinal(chave);
                break;

            case 0:
                break;

            default:
                printf("  Opcao invalida.\n");
        }

        if (op != 0 && op != 7) {
            printf("\n  Pressione Enter para continuar...");
            getchar(); getchar();
        }

    } while (op != 0);
}


/* ===================================================================
 * SECAO E — MENUS DOS NIVEIS ANTERIORES (mantidos integralmente)
 * =================================================================== */

/* Submenu do vetor (Nivel 2) */
void menuVetor() {
    int op; char nome[30];
    do {
        printf("\n  === MOCHILA - VETOR (%d/%d) ===\n", totalVetor, MAX_ITENS);
        printf("  1. Adicionar  2. Remover  3. Listar\n");
        printf("  4. Busca Sequencial  5. Ordenar  6. Busca Binaria  0. Voltar\n");
        printf("  Opcao: "); scanf("%d", &op);
        clock_t ini = clock();
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
            printf("  Tempo: %.3f ms\n", (double)(clock()-ini)/CLOCKS_PER_SEC*1000.0);
            printf("\n  Pressione Enter..."); getchar(); getchar();
        }
    } while (op != 0);
}

/* Submenu da lista encadeada (Nivel 2) */
void menuLista() {
    int op; char nome[30];
    do {
        printf("\n  === MOCHILA - LISTA ENCADEADA (%d nos) ===\n", totalLista);
        printf("  1. Adicionar  2. Remover  3. Listar  4. Busca Sequencial  0. Voltar\n");
        printf("  Opcao: "); scanf("%d", &op);
        clock_t ini = clock();
        switch (op) {
            case 1: inserirItemLista(lerItem()); listarItensLista(); break;
            case 2: lerNome(nome,30); removerItemLista(nome); listarItensLista(); break;
            case 3: listarItensLista(); break;
            case 4: lerNome(nome,30); buscarSequencialLista(nome); break;
            case 0: break;
            default: printf("  Opcao invalida.\n");
        }
        if (op != 0) {
            printf("  Tempo: %.3f ms\n", (double)(clock()-ini)/CLOCKS_PER_SEC*1000.0);
            printf("\n  Pressione Enter..."); getchar(); getchar();
        }
    } while (op != 0);
}

/* Tabela comparativa de complexidade entre as estruturas */
void exibirComparativo() {
    printf("\n  ========================================================\n");
    printf("  COMPARATIVO: VETOR x LISTA ENCADEADA\n");
    printf("  ========================================================\n");
    printf("  %-22s | %-15s | %-12s\n", "Operacao",          "Vetor",           "Lista");
    printf("  %-22s | %-15s | %-12s\n", "Acesso por indice", "O(1)",            "O(n)");
    printf("  %-22s | %-15s | %-12s\n", "Insercao",          "O(1)*",           "O(1) inicio");
    printf("  %-22s | %-15s | %-12s\n", "Remocao por nome",  "O(n)",            "O(n)");
    printf("  %-22s | %-15s | %-12s\n", "Busca sequencial",  "O(n)",            "O(n)");
    printf("  %-22s | %-15s | %-12s\n", "Busca binaria",     "O(log n)**",      "Nao suporta");
    printf("  %-22s | %-15s | %-12s\n", "Memoria",           "Fixo",            "Dinamico");
    printf("  * O(1) amortizado  ** requer ordenacao previa\n");
    printf("  ========================================================\n");
    printf("  Itens: Vetor=%d | Lista=%d | Torre=%d\n",
           totalVetor, totalLista, totalTorre);
}

/* Tabela explicando os tres algoritmos de ordenacao do nivel mestre */
void exibirComparativoSorts() {
    printf("\n  ========================================================\n");
    printf("  COMPARATIVO: ALGORITMOS DE ORDENACAO (Nivel Mestre)\n");
    printf("  ========================================================\n");
    printf("  %-18s | %-10s | %-10s | %-16s\n",
           "Algoritmo", "Melhor", "Pior", "Criterio");
    printf("  %-18s | %-10s | %-10s | %-16s\n",
           "Bubble Sort",     "O(n)",   "O(n^2)", "nome (string)");
    printf("  %-18s | %-10s | %-10s | %-16s\n",
           "Insertion Sort",  "O(n)",   "O(n^2)", "tipo (string)");
    printf("  %-18s | %-10s | %-10s | %-16s\n",
           "Selection Sort",  "O(n^2)", "O(n^2)", "prioridade (int)");
    printf("  --------------------------------------------------------\n");
    printf("  Bubble   : troca adjacentes; para cedo se ja ordenado.\n");
    printf("  Insertion: insere cada elem na posicao certa da parte ja ord.\n");
    printf("  Selection: seleciona o maior e o move para a frente.\n");
    printf("  Busca binaria: O(log n) — so apos Bubble Sort por nome.\n");
    printf("  ========================================================\n");
}


/* ===================================================================
 * MAIN — Menu raiz com todos os niveis
 * =================================================================== */
int main() {
    int opcao;
    do {
        printf("\n  ========================================\n");
        printf("  MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
        printf("  ========================================\n");
        printf("  Vetor: %d/%d  | Lista: %d nos | Torre: %d/%d\n\n",
               totalVetor, MAX_ITENS, totalLista, totalTorre, MAX_COMPONENTES);
        printf("  1. Mochila com VETOR           (Nivel 2)\n");
        printf("  2. Mochila com LISTA ENCADEADA (Nivel 2)\n");
        printf("  3. Torre de Fuga               (Nivel Mestre)\n");
        printf("  4. Comparativo Vetor x Lista\n");
        printf("  5. Comparativo Algoritmos de Ordenacao\n");
        printf("  0. Sair\n");
        printf("  ----------------------------------------\n");
        printf("  Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: menuVetor();              break;
            case 2: menuLista();              break;
            case 3: menuTorre();              break;
            case 4: exibirComparativo();      break;
            case 5: exibirComparativoSorts(); break;
            case 0:
                liberarLista();
                printf("\n  Boa sorte, sobrevivente!\n");
                break;
            default:
                printf("  Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}

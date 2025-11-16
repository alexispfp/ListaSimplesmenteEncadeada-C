#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Estrutura básica do nó de uma lista simplesmente encadeada.
 * Cada nó contém:
 * - Um ponteiro para um dado armazenado dinamicamente (string)
 * - Um ponteiro para o nó seguinte
 */
typedef struct No {
    char *dado;         // Armazena o dado do nó
    struct No *prox;    // Ponteiro para o próximo nó da lista
} No;

// Ponteiro global que representa o início da lista encadeada
No *inicio = NULL;

/*
 * Função: lerString
 * ------------------
 * Lê uma string de tamanho variável do teclado, utilizando um buffer temporário
 * e posteriormente alocando memória exata para armazenar o texto digitado.
 *
 * Retorna:
 *  Um ponteiro para uma string alocada dinamicamente.
 */
char* lerString() {
    char buffer[1024];

    fgets(buffer, 1024, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';  // remove o '\n'

    char *str = (char*) malloc(strlen(buffer) + 1);

    if (str == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    strcpy(str, buffer);
    return str;
}

/*
 * Função: criarNo
 * ----------------
 * Cria um novo nó para a lista encadeada.
 *
 * Parâmetro:
 *  valor - string já alocada dinamicamente.
 *
 * Retorna:
 *  Ponteiro para o novo nó criado.
 */
No* criarNo(char *valor) {
    No *novo = (No*) malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro ao alocar memória para o nó.\n");
        exit(1);
    }

    novo->dado = valor;
    novo->prox = NULL;

    return novo;
}

/*
 * Função: inserir
 * ----------------
 * Insere um novo nó no final da lista encadeada.
 *
 * Procedimento:
 *  - Caso a lista esteja vazia, o novo nó se torna o início.
 *  - Caso contrário, percorre até o último nó e o conecta ao novo.
 */
void inserir(char *valor) {
    No *novo = criarNo(valor);

    if (inicio == NULL) {    // lista vazia
        inicio = novo;
        return;
    }

    No *aux = inicio;
    while (aux->prox != NULL)
        aux = aux->prox;

    aux->prox = novo;
}

/*
 * Função: listar
 * ---------------
 * Percorre a lista encadeada do início ao fim,
 * exibindo os dados de cada nó.
 */
void listar() {
    if (inicio == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    No *aux = inicio;
    printf("Elementos da lista:\n");
    while (aux != NULL) {
        printf("- %s\n", aux->dado);
        aux = aux->prox;
    }
}

/*
 * Função: consultar
 * ------------------
 * Consulta um elemento da lista baseado em sua posição.
 *
 * Parâmetro:
 *  pos - índice do elemento na lista (0 = primeiro).
 *
 * Retorna:
 *  O dado encontrado ou NULL se a posição for inválida.
 */
char* consultar(int pos) {
    if (pos < 0)     // posição negativa não é válida
        return NULL;

    No *aux = inicio;
    int i = 0;

    while (aux != NULL) {
        if (i == pos)
            return aux->dado;

        aux = aux->prox;
        i++;
    }

    return NULL;     // posição além do tamanho da lista
}

/*
 * Função: alterar
 * ----------------
 * Altera o conteúdo de um nó buscando pelo valor antigo.
 *
 * Parâmetros:
 *  antigo - valor que será buscado na lista.
 *  novoValor - novo texto que substituirá o anterior.
 *
 * Retorna:
 *  1 se a alteração foi realizada; 0 caso contrário.
 */
int alterar(char *antigo, char *novoValor) {
    No *aux = inicio;

    while (aux != NULL) {
        if (strcmp(aux->dado, antigo) == 0) {
            free(aux->dado);      // libera o texto antigo
            aux->dado = novoValor;  // substitui pelo novo
            return 1;
        }
        aux = aux->prox;
    }

    return 0;  // dado não encontrado
}

/*
 * Função: removerElemento
 * ------------------------
 * Remove um nó da lista encadeada baseado no seu conteúdo.
 *
 * Parâmetro:
 *  valor - string que será procurada na lista.
 *
 * Procedimento:
 *  - Ajusta os ponteiros para desconectar o nó removido.
 *  - Libera a memória alocada para o dado e para o nó.
 *
 * Retorna:
 *  1 se removido; 0 se não encontrado.
 */
int removerElemento(char *valor) {
    if (inicio == NULL)   // lista vazia
        return 0;

    No *aux = inicio;
    No *anterior = NULL;

    while (aux != NULL && strcmp(aux->dado, valor) != 0) {
        anterior = aux;
        aux = aux->prox;
    }

    if (aux == NULL)      // não encontrou
        return 0;

    if (anterior == NULL)     // remoção do primeiro nó
        inicio = aux->prox;
    else
        anterior->prox = aux->prox;

    free(aux->dado);
    free(aux);

    return 1;
}

/*
 * Função: menu
 * -------------
 * Exibe as opções do usuário.
 */
void menu() {
    printf("\n===== LISTA ENCADEADA =====\n");
    printf("1 - Inserir\n");
    printf("2 - Listar\n");
    printf("3 - Consultar por posição\n");
    printf("4 - Alterar\n");
    printf("5 - Remover\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

/*
 * Função principal
 * -----------------
 * Gerencia a interação com o usuário e chama as funções da lista.
 */
int main() {
    int op;

    do {
        menu();
        scanf("%d", &op);
        getchar();  // consome o '\n' deixado pelo scanf

        char *valor, *novo;

        switch (op) {

            case 1:
                printf("Insira os dados: ");
                valor = lerString();
                inserir(valor);
                break;

            case 2:
                listar();
                break;

            case 3: {
                printf("Posição da lista a consultar: ");
                int pos;
                scanf("%d", &pos);
                getchar();

                char *resultado = consultar(pos);

                if (resultado)
                    printf("Na posição %d está: %s\n", pos, resultado);
                else
                    printf("Posição inválida.\n");
                break;
            }

            case 4:
                printf("Texto atual: ");
                valor = lerString();
                printf("Novo texto: ");
                novo = lerString();

                if (alterar(valor, novo))
                    printf("Alterado com sucesso!\n");
                else {
                    printf("Não encontrado.\n");
                    free(novo);
                }

                free(valor);
                break;

            case 5:
                printf("Texto a remover: ");
                valor = lerString();

                if (removerElemento(valor))
                    printf("Removido.\n");
                else
                    printf("Não encontrado.\n");

                free(valor);
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (op != 0);

    return 0;
}
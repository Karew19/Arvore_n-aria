#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

typedef int bool;

typedef struct no {
    char nome[100];
    char descricao[100];
    bool concluida;
    char dataVencimento[20];
    struct no* primeiroFilho;
    struct no* proximoIrmao;
} NO;

typedef NO* PONT;

PONT raiz = NULL;

PONT buscaChave(char* nome, PONT raiz) {
    if (raiz == NULL)
        return NULL;
    if (strcmp(raiz->nome, nome) == 0)
        return raiz;
    PONT p = raiz->primeiroFilho;
    while (p) {
        PONT resp = buscaChave(nome, p);
        if (resp)
            return resp;
        p = p->proximoIrmao;
    }
    return NULL;
}

PONT criaNovoNo(char* nome, char* descricao, char* dataVencimento) {
    PONT novo = (PONT)malloc(sizeof(NO));
    strcpy(novo->nome, nome);
    strcpy(novo->descricao, descricao);
    novo->concluida = false;
    strcpy(novo->dataVencimento, dataVencimento);
    novo->primeiroFilho = NULL;
    novo->proximoIrmao = NULL;
    return novo;
}

bool insere(PONT raiz, char* nome, char* descricao, char* nomePai, char* dataVencimento) {
    if (strcmp(nomePai, "raiz") == 0) {
        PONT filho = criaNovoNo(nome, descricao, dataVencimento);
        if (raiz->primeiroFilho == NULL) {
            raiz->primeiroFilho = filho;
        } else {
            PONT p = raiz->primeiroFilho;
            while (p->proximoIrmao != NULL)
                p = p->proximoIrmao;
            p->proximoIrmao = filho;
        }
        return true;
    } else {
        PONT pai = buscaChave(nomePai, raiz);
        if (!pai)
            return false;
        PONT filho = criaNovoNo(nome, descricao, dataVencimento);
        if (!pai->primeiroFilho) {
            pai->primeiroFilho = filho;
        } else {
            PONT p = pai->primeiroFilho;
            while (p->proximoIrmao)
                p = p->proximoIrmao;
            p->proximoIrmao = filho;
        }
        return true;
    }
}

void exibirArvore(PONT raiz, int nivel) {
    if (raiz == NULL)
        return;

    for (int i = 0; i < nivel; i++)
        printf("  ");

    if (raiz->concluida)
        printf("[Concluída] ");
    printf("- %s: %s (Data de Vencimento: %s)\n", raiz->nome, raiz->descricao, raiz->dataVencimento);

    PONT p = raiz->primeiroFilho;
    while (p) {
        exibirArvore(p, nivel + 1);
        p = p->proximoIrmao;
    }
}

void gravarArvoreNoArquivo(PONT raiz, FILE* arquivo) {
    if (raiz == NULL)
        return;

    fprintf(arquivo, "- Nome: %s\n", raiz->nome);
    fprintf(arquivo, "  Descrição: %s\n", raiz->descricao);
    fprintf(arquivo, "  Concluída: %s\n", raiz->concluida ? "Sim" : "Não");
    fprintf(arquivo, "  Data de Vencimento: %s\n", raiz->dataVencimento);

    PONT p = raiz->primeiroFilho;
    while (p) {
        gravarArvoreNoArquivo(p, arquivo);
        p = p->proximoIrmao;
    }
}

void adicionarTarefa() {
    char nome[100];
    char descricao[100];
    char nomePai[100];
    char dataVencimento[20];

    printf("Digite o nome da tarefa: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';  // Remover o caractere de nova linha

    printf("Digite a descrição da tarefa: ");
    fgets(descricao, sizeof(descricao), stdin);
    descricao[strcspn(descricao, "\n")] = '\0';  // Remover o caractere de nova linha

    printf("Digite a data de vencimento da tarefa: ");
    fgets(dataVencimento, sizeof(dataVencimento), stdin);
    dataVencimento[strcspn(dataVencimento, "\n")] = '\0';  // Remover o caractere de nova linha

    printf("Digite o nome da tarefa pai (ou 'raiz' para tarefa de primeiro nível): ");
    fgets(nomePai, sizeof(nomePai), stdin);
    nomePai[strcspn(nomePai, "\n")] = '\0';  // Remover o caractere de nova linha

    if (insere(raiz, nome, descricao, nomePai, dataVencimento)) {
        printf("Tarefa adicionada com sucesso!\n");
    } else {
        printf("Erro ao adicionar a tarefa. Verifique o nome da tarefa pai.\n");
    }
}

void marcarTarefaConcluida() {
    char nome[100];
    printf("Digite o nome da tarefa que deseja marcar como concluída: ");
    scanf("%s", nome);

    PONT tarefa = buscaChave(nome, raiz);
    if (tarefa != NULL) {
        tarefa->concluida = true;
        printf("Tarefa marcada como concluída com sucesso!\n");
    } else {
        printf("Tarefa não encontrada. Verifique o nome da tarefa.\n");
    }
}

void editarTarefa() {
    char nome[100];
    printf("Digite o nome da tarefa que deseja editar: ");
    scanf("%s", nome);

    PONT tarefa = buscaChave(nome, raiz);
    if (tarefa != NULL) {
        char descricao[100];
        char dataVencimento[20];

        printf("Digite a nova descrição da tarefa: ");
        scanf("%s", descricao);
        printf("Digite a nova data de vencimento da tarefa: ");
        scanf("%s", dataVencimento);

        strcpy(tarefa->descricao, descricao);
        strcpy(tarefa->dataVencimento, dataVencimento);

        printf("Tarefa editada com sucesso!\n");
    } else {
        printf("Tarefa não encontrada. Verifique o nome da tarefa.\n");
    }
}

void exibirMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Adicionar Tarefa\n");
    printf("2. Marcar Tarefa como Concluída\n");
    printf("3. Editar Tarefa\n");
    printf("4. Exibir Lista de Afazeres\n");
    printf("5. Sair\n");
    printf("Digite o número da opção desejada: ");
}

void salvarListaDeAfazeres() {
    FILE* arquivo = fopen("lista_afazeres.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de lista de afazeres.\n");
        return;
    }

    gravarArvoreNoArquivo(raiz, arquivo);

    fclose(arquivo);

    printf("Lista de afazeres salva com sucesso no arquivo 'lista_afazeres.txt'.\n");
}

int main() {
    raiz = criaNovoNo("Raiz", "", "");

    int opcao;
    do {
        exibirMenu();
        scanf("%d", &opcao);
        getchar();  // Limpar o buffer do teclado

        switch (opcao) {
            case 1:
                adicionarTarefa();
                break;
            case 2:
                marcarTarefaConcluida();
                break;
            case 3:
                editarTarefa();
                break;
            case 4:
                exibirArvore(raiz, 0);
                break;
            case 5:
                salvarListaDeAfazeres();
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 5);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score.h"

ListaScores* scoreInit() {
    ListaScores *lista = (ListaScores*) malloc(sizeof(ListaScores));
    lista->inicio  = NULL;
    lista->tamanho = 0;
    return lista;
}

void scoreInserir(ListaScores *lista, char *nome, int tempo) {
    No *novo = (No*) malloc(sizeof(No));
    strncpy(novo->nome, nome, MAX_NOME - 1);
    novo->nome[MAX_NOME - 1] = '\0';
    novo->tempo = tempo;
    novo->prox  = NULL;

    if (lista->inicio == NULL || tempo < lista->inicio->tempo) {
        novo->prox    = lista->inicio;
        lista->inicio = novo;
    } else {
        No *atual = lista->inicio;
        while (atual->prox != NULL && atual->prox->tempo <= tempo)
            atual = atual->prox;
        novo->prox    = atual->prox;
        atual->prox   = novo;
    }
    lista->tamanho++;
}

void scoreLer(ListaScores *lista) {
    FILE *f = fopen(ARQUIVO_SCORES, "r");
    if (!f) return;

    char nome[MAX_NOME];
    int tempo;
    while (fscanf(f, "%s %d", nome, &tempo) == 2)
        scoreInserir(lista, nome, tempo);
    fclose(f);
}

void scoreSalvar(ListaScores *lista) {
    FILE *f = fopen(ARQUIVO_SCORES, "w");
    if (!f) return;

    No *atual = lista->inicio;
    int count = 0;
    while (atual != NULL && count < 10) {
        fprintf(f, "%s %d\n", atual->nome, atual->tempo);
        atual = atual->prox;
        count++;
    }
    fclose(f);
}

void scoreDestroy(ListaScores *lista) {
    No *atual = lista->inicio;
    while (atual != NULL) {
        No *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    free(lista);
}
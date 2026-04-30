#ifndef SCORE_H
#define SCORE_H

#define ARQUIVO_SCORES "scores.txt"
#define MAX_NOME 20

typedef struct No {
    char nome[MAX_NOME];
    int tempo;
    struct No *prox;
} No;

typedef struct {
    No *inicio;
    int tamanho;
} ListaScores;

ListaScores* scoreInit();
void scoreInserir(ListaScores *lista, char *nome, int tempo);
void scoreLer(ListaScores *lista);
void scoreSalvar(ListaScores *lista);
void scoreDestroy(ListaScores *lista);

#endif
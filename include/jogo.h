#ifndef JOGO_H
#define JOGO_H

#include "raylib.h"
#include "score.h"

typedef enum {
    TELA_INICIO,
    TELA_JOGO,
    TELA_VITORIA,
    TELA_GAMEOVER,
    TELA_SCORES,
    TELA_SAIR
} TelaAtual;

void telaInicioDesenha();
void telaVitoriaDesenha(int tempo, ListaScores *scores);
void telaGameOverDesenha();
void telaScoresDesenha(ListaScores *scores);

#endif
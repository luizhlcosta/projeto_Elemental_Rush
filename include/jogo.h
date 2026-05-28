#ifndef JOGO_H
#define JOGO_H

#include "raylib.h"
#include "score.h"

#define TOTAL_MAPAS 2

typedef enum {
    TELA_INICIO,
    TELA_SELECAO_MAPA,
    TELA_JOGO,
    TELA_VITORIA,
    TELA_GAMEOVER,
    TELA_SCORES,
    TELA_SAIR
} TelaAtual;

void telaInicioDesenha();
void telaSelecaoMapaDesenha(int mapaAtual);
void telaVitoriaDesenha(int tempo, ListaScores *scores);
void telaGameOverDesenha();
void telaScoresDesenha(ListaScores *scores);

#endif
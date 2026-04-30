#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"

typedef struct {
    Vector2 posicao;     // posição atual (x, y)
    Vector2 velocidade;  // velocidade (x, y)
    int largura;
    int altura;
    Color cor;
    char simbolo;        // 'S' Starboy, 'P' PlasmaGirl
    int noChao;          // 1 = está no chão
    int vivo;            // 1 = vivo
} Jogador;

void jogadorInit(Jogador *j, float x, float y, Color cor, char simbolo);
void jogadorUpdate(Jogador *j);
void jogadorDesenha(Jogador *j);
void jogadorPulaStarboy(Jogador *j);
void jogadorPulaPlasmaGirl(Jogador *j);

#endif
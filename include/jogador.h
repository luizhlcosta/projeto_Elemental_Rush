#ifndef JOGADOR_H
#define JOGADOR_H

#include "tipos.h"

void jogadorInit(Jogador *j, float x, float y, Color cor, char simbolo);
void jogadorUpdate(Jogador *j, Mapa *m);
void jogadorDesenha(Jogador *j, Mapa *m);
void jogadorDestroi(Jogador *j);
void jogadorPulaStarboy(Jogador *j);
void jogadorPulaICE(Jogador *j);

#endif

#ifndef MAPA_H
#define MAPA_H

#include "tipos.h"

Mapa* mapaInit();
void mapaDestroy(Mapa *m);
void mapaDesenha(Mapa *m);
int mapaEhParede(Mapa *m, Rectangle rect);
int mapaEhFogo(Mapa *m, Rectangle rect);
int mapaEhAgua(Mapa *m, Rectangle rect);
int mapaStarboyVenceu(Mapa *m, Jogador *j);
int mapaPlasmaGirlVenceu(Mapa *m, Jogador *j);

#endif
//mapa.h

#ifndef MAPA_H
#define MAPA_H

#include "tipos.h"

Mapa* mapaInit();
void mapaDestroy(Mapa *m);
void mapaDesenha(Mapa *m);
int mapaColisaoTopo(Mapa *m, Rectangle rect, float peFrameAnterior, float velY);
int mapaEhMorte(Mapa *m, Rectangle rect);
int mapaEhParede(Mapa *m, Rectangle rect);
int mapaEhFogo(Mapa *m, Rectangle rect);
int mapaEhAgua(Mapa *m, Rectangle rect);
int mapaStarboyVenceu(Mapa *m, Jogador *j);
int mapaPlasmaGirlVenceu(Mapa *m, Jogador *j);
ListaEstrelas* estrelasInit(Vector2 *posicoes, int quantidade);
void estrelasVerificarColeta(ListaEstrelas *lista, Rectangle jog1, Rectangle jog2, float raio);
bool estrelasPodeProsseguir(ListaEstrelas *lista);
void estrelasDesenhar(ListaEstrelas *lista);
void estrelasDestroy(ListaEstrelas **lista);

#endif
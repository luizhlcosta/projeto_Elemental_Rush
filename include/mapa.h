#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"
#include "jogador.h"

#define MAPA_LINHAS  20
#define MAPA_COLUNAS 30
#define TILE_SIZE    32

#define VAZIO   0
#define PAREDE  1
#define FOGO    2
#define AGUA    3
#define PORTA_S 4
#define PORTA_P 5

typedef struct {
    int **grade;        // matriz alocada dinamicamente
    int linhas;
    int colunas;
    int tileSize;
} Mapa;

Mapa* mapaInit();
void mapaDestroy(Mapa *m);
void mapaDesenha(Mapa *m);
int mapaEhParede(Mapa *m, Rectangle rect);
int mapaEhFogo(Mapa *m, Rectangle rect);
int mapaEhAgua(Mapa *m, Rectangle rect);
int mapaStarboyVenceu(Mapa *m, Jogador *j);
int mapaPlasmaGirlVenceu(Mapa *m, Jogador *j);

#endif
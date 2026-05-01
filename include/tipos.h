#ifndef TIPOS_H
#define TIPOS_H

#include "raylib.h"

#define TILE_SIZE    32
#define MAPA_LINHAS  20
#define MAPA_COLUNAS 30

#define VAZIO   0
#define PAREDE  1
#define FOGO    2
#define AGUA    3
#define PORTA_S 4
#define PORTA_P 5

typedef struct {
    Vector2 posicao;
    Vector2 velocidade;
    int largura;
    int altura;
    Color cor;
    char simbolo;
    int noChao;
    int vivo;
} Jogador;

typedef struct Mapa {
    int **grade;
    int linhas;
    int colunas;
    int tileSize;
} Mapa;

#endif
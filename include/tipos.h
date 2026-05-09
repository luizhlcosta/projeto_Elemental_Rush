//tipos.h

#ifndef TIPOS_H
#define TIPOS_H

#include "raylib.h"

#define TILE_SIZE    40
#define MAPA_LINHAS  21
#define MAPA_COLUNAS 29

#define VAZIO     0
#define PAREDE    1
#define FOGO      2
#define AGUA_1    3
#define AGUA_2    4
#define PORTA_S   5
#define PORTA_P   7
#define TILE_CHAO 6

typedef struct {
    Vector2 posicao;
    Vector2 velocidade;
    int largura;
    int altura;
    Color cor;
    char simbolo;
    int noChao;
    int vivo;
    Texture2D sprite;
} Jogador;

typedef struct Mapa {
    int **grade;
    int linhas;
    int colunas;
    int tileSize;

    Texture2D texParede;
    Texture2D texFogo;
    Texture2D texAgua1;
    Texture2D texAgua2;
    Texture2D texPortaS;
    Texture2D texPortaP;
    Texture2D texChao;
} Mapa;

#endif
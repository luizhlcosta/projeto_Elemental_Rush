//tipos.h

#ifndef TIPOS_H
#define TIPOS_H 

#include "raylib.h"
#include "animacao.h"
#include <stdbool.h>

#define TILE_SIZE    40
#define MAPA_LINHAS  18
#define MAPA_COLUNAS 32

#define VAZIO     0
#define PAREDE    1
#define FOGO_1    2
#define FOGO_MEIO 3
#define FOGO_2    4
#define AGUA_1    5 //esquerda
#define AGUA_MEIO 6
#define AGUA_2    7 //direita 
#define TILE_CHAO 8
#define PORTA_P   9
#define PORTA_S   10
#define MORTE     11

typedef enum {
    ANIM_IDLE = 0,
    ANIM_CORRENDO,
    ANIM_PULANDO,
    ANIM_CAINDO,
    ANIM_TOTAL
} EstadoAnimacao;

typedef struct {
    Vector2 posicao;
    Vector2 velocidade;
    int largura;
    int altura;
    Color cor;
    char simbolo;
    int noChao;
    int vivo;
    AnimacaoSprite animacoes[ANIM_TOTAL];
    EstadoAnimacao estadoAnim;
    int olhandoEsquerda;
} Jogador;

typedef struct Mapa {
    int **grade;
    int linhas;
    int colunas;
    int tileSize;

    Texture2D texParede;
    Texture2D texFogo1;
    Texture2D texFogo2;
    Texture2D texFogoMeio;
    Texture2D texAgua1;
    Texture2D texAgua2;
    Texture2D texAguaMeio;
    Texture2D texPortaS;
    Texture2D texPortaP;
    Texture2D texChao;
    
} Mapa;

// aqui é o nosso tipo estrela que tem uma posição
//status de coletada ou nao
// e a proxima

typedef struct StarNode {
    Vector2 posicao;
    bool coletada;
    AnimacaoSprite animacao;
    struct StarNode *proximo;
} StarNode;

//uma lista do tipo estrela, onde tem o total e as coletadas

typedef struct {
    StarNode *inicio;
    int total;
    int coletadas;
} ListaEstrelas;

#endif
#include "jogador.h"
#include "mapa.h"

#define GRAVIDADE     800.0f
#define FORCA_PULO   -400.0f

void jogadorInit(Jogador *j, float x, float y, Color cor, char simbolo) {
    j->posicao   = (Vector2){x, y};
    j->velocidade = (Vector2){0, 0};
    j->largura   = TILE_SIZE;
    j->altura    = TILE_SIZE;
    j->cor       = cor;
    j->simbolo   = simbolo;
    j->noChao    = 0;
    j->vivo      = 1;
}

void jogadorUpdate(Jogador *j) {
    float dt = GetFrameTime();

    // Gravidade
    j->velocidade.y += GRAVIDADE * dt;

    // Move
    j->posicao.x += j->velocidade.x * dt;
    j->posicao.y += j->velocidade.y * dt;

    // Chão temporário (será substituído pela colisão com mapa)
    float chao = GetScreenHeight() - j->altura - 40;
    if (j->posicao.y >= chao) {
        j->posicao.y = chao;
        j->velocidade.y = 0;
        j->noChao = 1;
    } else {
        j->noChao = 0;
    }
}

void jogadorDesenha(Jogador *j) {
    DrawRectangle(
        (int)j->posicao.x,
        (int)j->posicao.y,
        j->largura,
        j->altura,
        j->cor
    );

    // Letra do personagem
    DrawText(
        (char[]){j->simbolo, '\0'},
        (int)j->posicao.x + 10,
        (int)j->posicao.y + 8,
        20,
        WHITE
    );
}

void jogadorPulaStarboy(Jogador *j) {
    if (j->noChao) {
        j->velocidade.y = FORCA_PULO;
        j->noChao = 0;
    }
}

void jogadorPulaPlasmaGirl(Jogador *j) {
    if (j->noChao) {
        j->velocidade.y = FORCA_PULO;
        j->noChao = 0;
    }
}
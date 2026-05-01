#include "jogador.h"
#include "mapa.h"
#include "tipos.h"

#define GRAVIDADE    800.0f
#define FORCA_PULO  -470.0f
#define VELOCIDADE   200.0f

void jogadorInit(Jogador *j, float x, float y, Color cor, char simbolo) {
    j->posicao    = (Vector2){x, y};
    j->velocidade = (Vector2){0, 0};
    j->largura    = TILE_SIZE;
    j->altura     = TILE_SIZE;
    j->cor        = cor;
    j->simbolo    = simbolo;
    j->noChao     = 0;
    j->vivo       = 1;
}

void jogadorUpdate(Jogador *j, Mapa *m) {
    float dt = GetFrameTime();

    // Gravidade
    j->velocidade.y += GRAVIDADE * dt;

    // Move horizontal
    j->posicao.x += j->velocidade.x * dt;
    Rectangle rH = {j->posicao.x, j->posicao.y, j->largura, j->altura};
    if (mapaEhParede(m, rH)) {
        j->posicao.x -= j->velocidade.x * dt;
        j->velocidade.x = 0;
    }

    // Move vertical
    j->posicao.y += j->velocidade.y * dt;
    Rectangle rV = {j->posicao.x, j->posicao.y, j->largura, j->altura};
    if (mapaEhParede(m, rV)) {
        if (j->velocidade.y > 0) {
            // Caindo - snap no topo do bloco
            int tileY = (int)((j->posicao.y + j->altura) / TILE_SIZE);
            j->posicao.y = (float)(tileY * TILE_SIZE) - j->altura;
            j->noChao = 1;
        } else {
            // Subindo - snap embaixo do bloco
            int tileY = (int)(j->posicao.y / TILE_SIZE);
            j->posicao.y = (float)((tileY + 1) * TILE_SIZE);
        }
        j->velocidade.y = 0;
    } else {
        j->noChao = 0;
    }

    // Limites da tela
    if (j->posicao.x < 0) j->posicao.x = 0;
    if (j->posicao.x + j->largura > GetScreenWidth())
        j->posicao.x = GetScreenWidth() - j->largura;
    if (j->posicao.y < 0) j->posicao.y = 0;
    if (j->posicao.y + j->altura > GetScreenHeight())  {
        j->posicao.y = GetScreenHeight() - j->altura;
        j->velocidade.y = 0;
        j->noChao = 1;
    }
}

void jogadorDesenha(Jogador *j) {
    DrawRectangle((int)j->posicao.x, (int)j->posicao.y, j->largura, j->altura, j->cor);
    DrawText((char[]){j->simbolo, '\0'},
             (int)j->posicao.x + 10,
             (int)j->posicao.y + 8,
             20, WHITE);
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
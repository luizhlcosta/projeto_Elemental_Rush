#include <stdio.h>

#include "jogador.h"
#include "mapa.h"
#include "tipos.h"
#include "musica.h"

#define GRAVIDADE   800.0f
#define FORCA_PULO -440.0f
#define VELOCIDADE  200.0f

#define LAVA_SPRITE  "assets/fireidle.png"
#define LAVA_FRAMES  8
#define LAVA_FPS     10

#define ICE_SPRITE "assets/iceidle.png"
#define ICE_FRAMES 8
#define ICE_FPS    8

void jogadorInit(Jogador *j, float x, float y, Color cor, char simbolo) {
    j->posicao         = (Vector2){x, y};
    j->velocidade      = (Vector2){0, 0};
    j->largura         = TILE_SIZE;
    j->altura          = TILE_SIZE;
    j->cor             = cor;
    j->simbolo         = simbolo;
    j->noChao          = 0;
    j->vivo            = 1;
    j->olhandoEsquerda = 0;
    j->estadoAnim      = ANIM_IDLE;

    const char *caminho = (simbolo == 'S') ? LAVA_SPRITE    : ICE_SPRITE;
    int         nFrames = (simbolo == 'S') ? LAVA_FRAMES    : ICE_FRAMES;
    int         fps     = (simbolo == 'S') ? LAVA_FPS       : ICE_FPS;

    Texture2D tex = LoadTexture(caminho);
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);

    Rectangle frames[32];
    float fw = (float)tex.width / nFrames;
    float fh = (float)tex.height;
    for (int i = 0; i < nFrames; i++)
        frames[i] = (Rectangle){ i * fw, 0, fw, fh };

    for (int s = 0; s < ANIM_TOTAL; s++)
        j->animacoes[s] = Criar_Animacao_Sprite(tex, fps, frames, nFrames);
}

void jogadorUpdate(Jogador *j, Mapa *m) {

    j->noChao = 0;

    float dt = GetFrameTime();

    if (j->velocidade.x < 0) j->olhandoEsquerda = 1;
    else if (j->velocidade.x > 0) j->olhandoEsquerda = 0;

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
            int tileY = (int)((j->posicao.y + j->altura) / TILE_SIZE);
            j->posicao.y = (float)(tileY * TILE_SIZE) - j->altura;
            j->noChao = 1;
        } else if (j->velocidade.y < 0) {
            int tileY = (int)(j->posicao.y / TILE_SIZE);
            j->posicao.y = (float)((tileY + 1) * TILE_SIZE);
        }
        j->velocidade.y = 0;
    }

    else if (j->velocidade.y > 0) {
        int fundoFluid = mapaFundoFluid(m, rV);
        if (fundoFluid > 0 && (j->posicao.y + j->altura) >= (float)(fundoFluid - 4)) {
            j->posicao.y = (float)(fundoFluid - 4) - j->altura;
            j->velocidade.y = 0;
            j->noChao = 1;
        }
    }

    else if (j->velocidade.y < 0) {

        int yAtual   = (int)(j->posicao.y / TILE_SIZE);
        int yAnterior = (int)((j->posicao.y - j->velocidade.y * dt) / TILE_SIZE);

        int x1 = (int)( j->posicao.x               / TILE_SIZE);
        int x2 = (int)((j->posicao.x + j->largura - 1) / TILE_SIZE);

        for (int yTopo = yAtual; yTopo <= yAnterior; yTopo++) {
            for (int x = x1; x <= x2; x++) {
                if (yTopo >= 0 && yTopo < m->linhas && x >= 0 && x < m->colunas) {
                    int tile = m->grade[yTopo][x];

                    if (tile == AGUA_1 || tile == AGUA_2 || tile == AGUA_MEIO ||
                        tile == FOGO_1 || tile == FOGO_2 || tile == FOGO_MEIO) {
                        j->posicao.y = (float)((yTopo + 1) * TILE_SIZE);
                        j->velocidade.y = 0;
                        goto fim_colisao_fluid; 
                    }
                }
            }
        }
        fim_colisao_fluid:;
    }

    float mapaLargura = (float)(m->colunas * m->tileSize);
    float mapaAltura  = (float)(m->linhas  * m->tileSize);

    if (j->posicao.x < 0) j->posicao.x = 0;
    if (j->posicao.x + j->largura > mapaLargura)
        j->posicao.x = mapaLargura - j->largura;
    if (j->posicao.y < 0) j->posicao.y = 0;
    if (j->posicao.y + j->altura > mapaAltura) {
        j->posicao.y = mapaAltura - j->altura;
        j->velocidade.y = 0;
        j->noChao = 1;
    }

    if (!j->noChao)
        j->estadoAnim = (j->velocidade.y < 0) ? ANIM_PULANDO : ANIM_CAINDO;
    else if (j->velocidade.x != 0)
        j->estadoAnim = ANIM_CORRENDO;
    else
        j->estadoAnim = ANIM_IDLE;
}

void jogadorDesenha(Jogador *j, Mapa *m) {
    
    Rectangle rect = {j->posicao.x, j->posicao.y, j->largura, j->altura};
    Color cor = WHITE;

    if (mapaEhAgua(m, rect) || mapaEhFogo(m, rect)) {
        cor = (Color){255, 255, 255, 40};
    }

    Rectangle dest = {
        j->posicao.x,
        j->posicao.y,
        (float)j->largura,
        (float)j->altura
    };

    DesenhaAnimacaoSpritePro(
        &j->animacoes[j->estadoAnim],
        dest,
        (Vector2){0, 0},
        0.0f,
        cor,
        0,
        j->olhandoEsquerda,
        0
    );
}

void jogadorDestroi(Jogador *j) {
    for (int s = 0; s < ANIM_TOTAL; s++)
        PararAnimacaoSprite(j->animacoes[s]);
    UnloadTexture(j->animacoes[ANIM_IDLE].atlas);
}

void jogadorPulaLava(Jogador *j) {
    if (j->noChao) {
        j->velocidade.y = FORCA_PULO;
        j->noChao = 0;
        musicaTocaPulo();
    }
}

void jogadorPulaIce(Jogador *j) {
    if (j->noChao) {
        j->velocidade.y = FORCA_PULO;
        j->noChao = 0;
        musicaTocaPulo();
    }
}
#include <stdio.h>

#include "jogador.h"
#include "mapa.h"
#include "tipos.h"

#define GRAVIDADE   800.0f
#define FORCA_PULO -440.0f
#define VELOCIDADE  200.0f

// ── Spritesheets ──────────────────────────────────────────────────────────────
// Ambos os personagens usam tiras horizontais de 160x32 px (5 frames de 32x32).
// Para adicionar novas animações, crie o arquivo e replique o bloco abaixo.

#define STARBOY_SPRITE  "assets/fireidle.png"
#define STARBOY_FRAMES  8
#define STARBOY_FPS     10

#define PLASMAGIRL_SPRITE "assets/plasmaidle.png"
#define PLASMAGIRL_FRAMES 8
#define PLASMAGIRL_FPS    8

// ─────────────────────────────────────────────────────────────────────────────

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

    const char *caminho = (simbolo == 'S') ? STARBOY_SPRITE    : PLASMAGIRL_SPRITE;
    int         nFrames = (simbolo == 'S') ? STARBOY_FRAMES    : PLASMAGIRL_FRAMES;
    int         fps     = (simbolo == 'S') ? STARBOY_FPS       : PLASMAGIRL_FPS;

    Texture2D tex = LoadTexture(caminho);
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);

    // Define os frames: cada frame começa em (i * fw, 0) e tem tamanho fw x fh
    Rectangle frames[32];
    float fw = (float)tex.width / nFrames;
    float fh = (float)tex.height;
    for (int i = 0; i < nFrames; i++)
        frames[i] = (Rectangle){ i * fw, 0, fw, fh };

    // Todos os estados compartilham o mesmo atlas por enquanto.
    // Quando uma nova animação tiver spritesheet próprio, troque só o slot dela.
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

    // Limites da tela
    if (j->posicao.x < 0) j->posicao.x = 0;
    if (j->posicao.x + j->largura > GetScreenWidth())
        j->posicao.x = GetScreenWidth() - j->largura;
    if (j->posicao.y < 0) j->posicao.y = 0;
    if (j->posicao.y + j->altura > GetScreenHeight()) {
        j->posicao.y = GetScreenHeight() - j->altura;
        j->velocidade.y = 0;
        j->noChao = 1;
    }

    // Estado da animação
    if (!j->noChao)
        j->estadoAnim = (j->velocidade.y < 0) ? ANIM_PULANDO : ANIM_CAINDO;
    else if (j->velocidade.x != 0)
        j->estadoAnim = ANIM_CORRENDO;
    else
        j->estadoAnim = ANIM_IDLE;
}

void jogadorDesenha(Jogador *j) {
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
        WHITE,
        0,                  // pausado
        j->olhandoEsquerda, // flipH
        0                   // flipV
    );
}

void jogadorDestroi(Jogador *j) {
    // Libera os retângulos de cada slot
    for (int s = 0; s < ANIM_TOTAL; s++)
        PararAnimacaoSprite(j->animacoes[s]);
    // A textura foi carregada uma vez — descarrega uma vez
    UnloadTexture(j->animacoes[ANIM_IDLE].atlas);
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
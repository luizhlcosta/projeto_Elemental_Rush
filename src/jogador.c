#include <stdio.h> 

#include "jogador.h"
#include "mapa.h"
#include "tipos.h"

#define GRAVIDADE    800.0f
#define FORCA_PULO  -450.0f
#define VELOCIDADE   200.0f

// ── Configuração dos spritesheets ─────────────────────────────────────────────
// Starboy: assets/starboy_idle.png — tira horizontal com 5 frames de 36×32 px
// PlasmaGirl: assets/plasmagirl.png — sprite único 32×32 px
//
// Como só existe animação de idle, todos os estados de animação usam
// o mesmo spritesheet. Quando novas animações forem adicionadas, basta
// criar os arquivos e ajustar os valores abaixo.

#define STARBOY_FRAMES    5
#define STARBOY_FPS       8
#define STARBOY_SPRITE    "assets/pixil-frame-0_16.png"

#define PLASMAGIRL_FRAMES 1
#define PLASMAGIRL_FPS    1
#define PLASMAGIRL_SPRITE "assets/plasmagirl.png"

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

    const char *caminho  = (simbolo == 'S') ? STARBOY_SPRITE    : PLASMAGIRL_SPRITE;
    int         nFrames  = (simbolo == 'S') ? STARBOY_FRAMES    : PLASMAGIRL_FRAMES;
    int         fps      = (simbolo == 'S') ? STARBOY_FPS       : PLASMAGIRL_FPS;

    // Calcula os retângulos de cada frame (tira horizontal)
    Texture2D ref = LoadTexture(caminho);
    SetTextureFilter(ref, TEXTURE_FILTER_POINT);

    float fw = (float)ref.width  / nFrames;
    float fh = (float)ref.height;

    Rectangle frames[32];
    for (int i = 0; i < nFrames; i++)
        frames[i] = (Rectangle){ i * fw, 0, fw, fh };

    // ANIM_IDLE usa o atlas já carregado
    j->animacoes[ANIM_IDLE] = Criar_Animacao_Sprite(ref, fps, frames, nFrames);

    // Demais estados carregam o mesmo arquivo como textura independente,
    // permitindo que cada um seja descarregado separadamente no destroy.
    for (int s = 1; s < ANIM_TOTAL; s++) {
        Texture2D t = LoadTexture(caminho);
        SetTextureFilter(t, TEXTURE_FILTER_POINT);
        j->animacoes[s] = Criar_Animacao_Sprite(t, fps, frames, nFrames);
    }
}

void jogadorUpdate(Jogador *j, Mapa *m) {

    j->noChao = 0;

    float dt = GetFrameTime();

    // Atualiza direção para o flip horizontal
    if (j->velocidade.x < 0) j->olhandoEsquerda = 1;
    else if (j->velocidade.x > 0) j->olhandoEsquerda = 0;

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
    float prevBottom = j->posicao.y + j->altura;
    (void)prevBottom;

    j->posicao.y += j->velocidade.y * dt;

    Rectangle rV = {
        j->posicao.x,
        j->posicao.y,
        j->largura,
        j->altura
    };

    // Colisão sólida completa
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

    // Determina o estado da animação com base no movimento
    if (!j->noChao) {
        j->estadoAnim = (j->velocidade.y < 0) ? ANIM_PULANDO : ANIM_CAINDO;
    } else if (j->velocidade.x != 0) {
        j->estadoAnim = ANIM_CORRENDO;
    } else {
        j->estadoAnim = ANIM_IDLE;
    }
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
        0,                    // pausado
        j->olhandoEsquerda,   // flipH
        0                     // flipV
    );
}

void jogadorDestroi(Jogador *j) {
    for (int s = 0; s < ANIM_TOTAL; s++) {
        PararAnimacaoSprite(j->animacoes[s]);
        UnloadTexture(j->animacoes[s].atlas);
    }
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
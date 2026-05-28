//main.c
#include "raylib.h"
#include "jogador.h"
#include "mapa.h"
#include "animacao.h"
#include "jogo.h"
#include "score.h"
#include <time.h>
#include <math.h>

#define LARGURA  1280
#define ALTURA   720

// Posições iniciais dos jogadores por mapa
// [mapa][jogador] -> {x, y}
static float posInicio[TOTAL_MAPAS][2][2] = {
    // Mapa 0 (Vulcão): Starboy à esquerda, PlasmaGirl à direita
    { {80.0f, 480.0f}, {980.0f, 480.0f} },
    // Mapa 1 (Caverna de Gelo): Starboy à direita, PlasmaGirl à esquerda
    { {980.0f, 480.0f}, {80.0f, 480.0f} },
};

// Estrelas por mapa
static Vector2 posEstrelasMapa[TOTAL_MAPAS][3] = {
    // Mapa 0
    { {14*40, 5*40}, {25*40, 8*40}, {14*40, 9*40} },
    // Mapa 1
    { {16*40, 6*40}, {8*40,  8*40}, {24*40, 8*40} },
};

typedef struct {
    Jogador starboy;
    Jogador plasmagirl;
    Mapa   *mapa;
    ListaEstrelas *estrelas;
    time_t  inicio;
    int     tempoFinal;
    int     mapaSelecionado;
} EstadoJogo;

static void estadoJogoInit(EstadoJogo *e, int mapaSelecionado) {
    e->mapaSelecionado = mapaSelecionado;

    jogadorInit(&e->starboy,
        posInicio[mapaSelecionado][0][0],
        posInicio[mapaSelecionado][0][1],
        YELLOW, 'S');
    jogadorInit(&e->plasmagirl,
        posInicio[mapaSelecionado][1][0],
        posInicio[mapaSelecionado][1][1],
        WHITE, 'P');

    e->mapa    = mapaInitNivel(mapaSelecionado);
    e->estrelas = estrelasInit(posEstrelasMapa[mapaSelecionado], 3);
    e->inicio   = 0;
    e->tempoFinal = 0;
}

static void estadoJogoDestroy(EstadoJogo *e) {
    estrelasDestroy(&e->estrelas);
    mapaDestroy(e->mapa);
    jogadorDestroi(&e->starboy);
    jogadorDestroi(&e->plasmagirl);
}

int main() {

    InitWindow(LARGURA, ALTURA, "Elemental Rush");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    Texture2D background = LoadTexture("assets/background_vulcanico.png");

    RenderTexture2D target = LoadRenderTexture(LARGURA, ALTURA);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    // Scores
    ListaScores *scores = scoreInit();
    scoreLer(scores);

    // Estado do jogo
    TelaAtual tela = TELA_INICIO;
    int mapaSelecionado = 0;   // cursor da tela de seleção

    EstadoJogo jogo = {0};

    while (!WindowShouldClose()) {

        // ── UPDATE ──────────────────────────────────────────
        switch (tela) {

            case TELA_INICIO:
                if (IsKeyPressed(KEY_ENTER)) {
                    tela = TELA_SELECAO_MAPA;
                    mapaSelecionado = 0;
                }
                break;

            case TELA_SELECAO_MAPA:
                if (IsKeyPressed(KEY_UP))
                    mapaSelecionado = (mapaSelecionado - 1 + TOTAL_MAPAS) % TOTAL_MAPAS;

                if (IsKeyPressed(KEY_DOWN))
                    mapaSelecionado = (mapaSelecionado + 1) % TOTAL_MAPAS;

                if (IsKeyPressed(KEY_ENTER)) {
                    estadoJogoInit(&jogo, mapaSelecionado);
                    jogo.inicio = time(NULL);
                    tela = TELA_JOGO;
                }

                if (IsKeyPressed(KEY_ESCAPE))
                    tela = TELA_INICIO;

                break;

            case TELA_JOGO: {

                // Starboy - WASD
                jogo.starboy.velocidade.x = 0;
                if (IsKeyDown(KEY_A)) jogo.starboy.velocidade.x = -200;
                if (IsKeyDown(KEY_D)) jogo.starboy.velocidade.x =  200;

                // PlasmaGirl - Setas
                jogo.plasmagirl.velocidade.x = 0;
                if (IsKeyDown(KEY_LEFT))  jogo.plasmagirl.velocidade.x = -200;
                if (IsKeyDown(KEY_RIGHT)) jogo.plasmagirl.velocidade.x =  200;

                jogadorUpdate(&jogo.starboy,    jogo.mapa);
                jogadorUpdate(&jogo.plasmagirl, jogo.mapa);

                if (IsKeyDown(KEY_W))   jogadorPulaStarboy(&jogo.starboy);
                if (IsKeyDown(KEY_UP))  jogadorPulaPlasmaGirl(&jogo.plasmagirl);

                Rectangle rS = {
                    jogo.starboy.posicao.x, jogo.starboy.posicao.y,
                    jogo.starboy.largura,   jogo.starboy.altura
                };
                Rectangle rP = {
                    jogo.plasmagirl.posicao.x, jogo.plasmagirl.posicao.y,
                    jogo.plasmagirl.largura,   jogo.plasmagirl.altura
                };

                estrelasVerificarColeta(jogo.estrelas, rS, rP, 20.0f);

                if (mapaEhAgua(jogo.mapa, rS) || mapaEhFogo(jogo.mapa, rP) ||
                    mapaEhMorte(jogo.mapa, rS) || mapaEhMorte(jogo.mapa, rP)) {
                    jogo.tempoFinal = (int)(time(NULL) - jogo.inicio);
                    tela = TELA_GAMEOVER;
                }

                if (mapaStarboyVenceu(jogo.mapa, &jogo.starboy) &&
                    mapaPlasmaGirlVenceu(jogo.mapa, &jogo.plasmagirl) &&
                    estrelasPodeProsseguir(jogo.estrelas)) {
                    jogo.tempoFinal = (int)(time(NULL) - jogo.inicio);
                    tela = TELA_VITORIA;
                }

                // ESC volta ao menu
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoJogoDestroy(&jogo);
                    tela = TELA_INICIO;
                }

                break;
            }

            case TELA_VITORIA:
            case TELA_GAMEOVER:
                if (IsKeyPressed(KEY_ENTER)) {
                    estadoJogoDestroy(&jogo);
                    tela = TELA_SAIR;
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoJogoDestroy(&jogo);
                    tela = TELA_INICIO;
                }
                break;

            default:
                break;
        }

        if (tela == TELA_SAIR) break;

        // ── DRAW ────────────────────────────────────────────
        BeginTextureMode(target);
        ClearBackground(BLACK);

        switch (tela) {

            case TELA_INICIO:
                telaInicioDesenha();
                break;

            case TELA_SELECAO_MAPA:
                telaSelecaoMapaDesenha(mapaSelecionado);
                break;

            case TELA_JOGO: {
                ClearBackground(BLACK);

                float escalaBg = fmax((float)LARGURA / background.width,
                                      (float)ALTURA  / background.height);
                float larguraBg = background.width  * escalaBg;
                float alturaBg  = background.height * escalaBg;

                DrawTexturePro(
                    background,
                    (Rectangle){0, 0, background.width, background.height},
                    (Rectangle){(LARGURA - larguraBg) / 2, (ALTURA - alturaBg) / 2, larguraBg, alturaBg},
                    (Vector2){0, 0}, 0.0f, WHITE
                );

                mapaDesenha(jogo.mapa);
                estrelasDesenhar(jogo.estrelas);
                jogadorDesenha(&jogo.starboy);
                jogadorDesenha(&jogo.plasmagirl);
                DrawText(TextFormat("Tempo: %ds", (int)(time(NULL) - jogo.inicio)), 10, 10, 20, WHITE);
                DrawText(TextFormat("Estrelas: %d/3", jogo.estrelas->coletadas), LARGURA - 180, 10, 20, GOLD);
                DrawText("ESC = Menu", LARGURA - 130, ALTURA - 30, 18, (Color){200,200,200,180});
                break;
            }

            case TELA_VITORIA:
                telaVitoriaDesenha(jogo.tempoFinal, scores);
                break;

            case TELA_GAMEOVER:
                telaGameOverDesenha();
                break;

            default:
                break;
        }

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        float scale = fmin(
            (float)GetScreenWidth()  / LARGURA,
            (float)GetScreenHeight() / ALTURA
        );

        DrawTexturePro(
            target.texture,
            (Rectangle){0, 0, LARGURA, -ALTURA},
            (Rectangle){
                (GetScreenWidth()  - (LARGURA * scale)) / 2,
                (GetScreenHeight() - (ALTURA  * scale)) / 2,
                LARGURA * scale,
                ALTURA  * scale
            },
            (Vector2){0, 0}, 0.0f, WHITE
        );

        EndDrawing();
    }

    // Libera recursos globais
    scoreDestroy(scores);
    UnloadTexture(background);
    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}

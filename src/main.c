//main.c
#include "raylib.h"
#include "jogador.h"
#include "mapa.h"
#include "jogo.h"
#include "score.h"
#include <time.h>

#define LARGURA  1160
#define ALTURA   840

int main() {
    InitWindow(LARGURA, ALTURA, "Elemental Rush");
    SetTargetFPS(60);
    Texture2D background = LoadTexture("assets/background_vulcanico.png");


    // Scores
    ListaScores *scores = scoreInit();
    scoreLer(scores);

    // Estado do jogo
    TelaAtual tela = TELA_INICIO;

    // Jogadores
    Jogador starboy, plasmagirl;
    jogadorInit(&starboy,    80,  480, YELLOW,  'S');
    jogadorInit(&plasmagirl, 980, 480, WHITE, 'P');

    // Mapa
    Mapa *mapa = mapaInit();

    time_t inicio = 0;
    int tempoFinal = 0;

    while (!WindowShouldClose()) {

        // UPDATE
        switch (tela) {
            case TELA_INICIO:
                if (IsKeyPressed(KEY_ENTER)) {
                    tela  = TELA_JOGO;
                    inicio = time(NULL);
                }
                break;

            case TELA_JOGO:
                // Starboy - WASD
                starboy.velocidade.x = 0;
                if (IsKeyDown(KEY_A)) starboy.velocidade.x = -200;
                if (IsKeyDown(KEY_D)) starboy.velocidade.x =  200;

                // PlasmaGirl - Setas
                plasmagirl.velocidade.x = 0;
                if (IsKeyDown(KEY_LEFT))  plasmagirl.velocidade.x = -200;
                if (IsKeyDown(KEY_RIGHT)) plasmagirl.velocidade.x =  200;

                // Update primeiro (atualiza noChao corretamente)
                jogadorUpdate(&starboy, mapa);
                jogadorUpdate(&plasmagirl, mapa);

                // Pulo depois do update (noChao já está correto)
                if (IsKeyDown(KEY_W)) jogadorPulaStarboy(&starboy);
                if (IsKeyDown(KEY_UP)) jogadorPulaPlasmaGirl(&plasmagirl);

                // Verifica morte
                Rectangle rS = {starboy.posicao.x, starboy.posicao.y, starboy.largura, starboy.altura};
                Rectangle rP = {plasmagirl.posicao.x, plasmagirl.posicao.y, plasmagirl.largura, plasmagirl.altura};
                if (mapaEhAgua(mapa, rS) || mapaEhFogo(mapa, rP) || mapaEhMorte(mapa, rS) || mapaEhMorte(mapa, rP)) {
                    tela = TELA_GAMEOVER;
                }

                if (mapaStarboyVenceu(mapa, &starboy) && mapaPlasmaGirlVenceu(mapa, &plasmagirl)) {
                    tempoFinal = (int)(time(NULL) - inicio);
                    tela = TELA_VITORIA;
                }
                break;

            case TELA_VITORIA:
            case TELA_GAMEOVER:
                if (IsKeyPressed(KEY_ENTER)) tela = TELA_SAIR;
                break;

            default: break;
        }

        if (tela == TELA_SAIR) break;

        // DRAW
        BeginDrawing();
        switch (tela) {
            case TELA_INICIO:
                telaInicioDesenha();
                break;

            case TELA_JOGO:
                ClearBackground(BLACK);
                Rectangle source = {
                    0,
                    0,
                    background.width,
                    background.height
                };

                Rectangle dest = {
                    0,
                    0,
                    GetScreenWidth(),
                    GetScreenHeight()
                };

                DrawTexturePro(
                    background,
                    source,
                    dest,
                    (Vector2) {0,0},
                    0.0f,
                    WHITE
                );

                mapaDesenha(mapa);
                jogadorDesenha(&starboy);
                jogadorDesenha(&plasmagirl);
                DrawText(TextFormat("Tempo: %ds", (int)(time(NULL) - inicio)), 10, 10, 20, WHITE);
                break;

            case TELA_VITORIA:
                telaVitoriaDesenha(tempoFinal, scores);
                break;

            case TELA_GAMEOVER:
                telaGameOverDesenha();
                break;

            default: break;
        }
        EndDrawing();
    }

    mapaDestroy(mapa);
    scoreDestroy(scores);
    UnloadTexture(background);
    UnloadTexture(starboy.sprite);
    UnloadTexture(plasmagirl.sprite);
    CloseWindow();
    return 0;
}
//main.c
#include "raylib.h"
#include "jogador.h"
#include "mapa.h"
#include "jogo.h"
#include "score.h"
#include <time.h>
#include <math.h>

#define LARGURA  1280
#define ALTURA   720

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

    // Jogadores
    Jogador starboy, plasmagirl;

    jogadorInit(&starboy, 80, 480, YELLOW, 'S');
    jogadorInit(&plasmagirl, 980, 480, WHITE, 'P');

    // Mapa
    Mapa *mapa = mapaInit();

    // esttrelas do level 1
    Vector2 posEstrelas[3] = {
    {13 * TILE_SIZE, 4 * TILE_SIZE},
    { 3 * TILE_SIZE, 8 * TILE_SIZE},
    {14 * TILE_SIZE, 12 * TILE_SIZE}
    };
    ListaEstrelas *estrelas = estrelasInit(posEstrelas, 3);

    time_t inicio = 0;
    int tempoFinal = 0;

    while (!WindowShouldClose()) {

        // UPDATE
        switch (tela) {

            case TELA_INICIO:

                if (IsKeyPressed(KEY_ENTER)) {
                    tela = TELA_JOGO;
                    inicio = time(NULL);
                }

                break;

            case TELA_JOGO:

                // Starboy - WASD
                starboy.velocidade.x = 0;

                if (IsKeyDown(KEY_A))
                    starboy.velocidade.x = -200;

                if (IsKeyDown(KEY_D))
                    starboy.velocidade.x = 200;

                // PlasmaGirl - Setas
                plasmagirl.velocidade.x = 0;

                if (IsKeyDown(KEY_LEFT))
                    plasmagirl.velocidade.x = -200;

                if (IsKeyDown(KEY_RIGHT))
                    plasmagirl.velocidade.x = 200;

                // Update primeiro
                jogadorUpdate(&starboy, mapa);
                jogadorUpdate(&plasmagirl, mapa);
                estrelasVerificarColeta(estrelas, starboy.posicao, plasmagirl.posicao, 20.0f);

                // Pulo
                if (IsKeyDown(KEY_W))
                    jogadorPulaStarboy(&starboy);

                if (IsKeyDown(KEY_UP))
                    jogadorPulaPlasmaGirl(&plasmagirl);

                // Verifica morte
                Rectangle rS = {
                    starboy.posicao.x,
                    starboy.posicao.y,
                    starboy.largura,
                    starboy.altura
                };

                Rectangle rP = {
                    plasmagirl.posicao.x,
                    plasmagirl.posicao.y,
                    plasmagirl.largura,
                    plasmagirl.altura
                };

                if (
                    mapaEhAgua(mapa, rS) ||
                    mapaEhFogo(mapa, rP) ||
                    mapaEhMorte(mapa, rS) ||
                    mapaEhMorte(mapa, rP)
                ) {
                    tela = TELA_GAMEOVER;
                }

                if (mapaStarboyVenceu(mapa, &starboy) && mapaPlasmaGirlVenceu(mapa, &plasmagirl) && estrelasPodeProsseguir(estrelas)) {
                    tempoFinal = (int)(time(NULL) - inicio);
                    tela = TELA_VITORIA;
                }

                break;

            case TELA_VITORIA:
            case TELA_GAMEOVER:

                if (IsKeyPressed(KEY_ENTER))
                    tela = TELA_SAIR;

                break;

            default:
                break;
        }

        if (tela == TELA_SAIR)
            break;

        // DRAW
        BeginTextureMode(target);

        ClearBackground(BLACK);

        switch (tela) {

            case TELA_INICIO:

                telaInicioDesenha();

                break;

            case TELA_JOGO:

                ClearBackground(BLACK);

                float escalaBg = fmax(
                    (float)LARGURA / background.width,
                    (float)ALTURA / background.height
                );

                float larguraBg = background.width * escalaBg;
                float alturaBg  = background.height * escalaBg;

                Rectangle source = {
                    0,
                    0,
                    background.width,
                    background.height
                };

                Rectangle dest = {
                    (LARGURA - larguraBg) / 2,
                    (ALTURA - alturaBg) / 2,
                    larguraBg,
                    alturaBg
                };

                DrawTexturePro(
                    background,
                    source,
                    dest,
                    (Vector2){0, 0},
                    0.0f,
                    WHITE
                );

                mapaDesenha(mapa);
                estrelasDesenhar(estrelas); 
                jogadorDesenha(&starboy);
                jogadorDesenha(&plasmagirl);
                DrawText(TextFormat("Tempo: %ds", (int)(time(NULL) - inicio)), 10, 10, 20, WHITE);
                DrawText(TextFormat("Estrelas: %d/3", estrelas->coletadas), LARGURA - 180, 10, 20, GOLD);
                break;

            case TELA_VITORIA:

                telaVitoriaDesenha(tempoFinal, scores);

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
            (float)GetScreenWidth() / LARGURA,
            (float)GetScreenHeight() / ALTURA
        );

        DrawTexturePro(
            target.texture,
            (Rectangle){0, 0, LARGURA, -ALTURA},
            (Rectangle){
                (GetScreenWidth() - (LARGURA * scale)) / 2,
                (GetScreenHeight() - (ALTURA * scale)) / 2,
                LARGURA * scale,
                ALTURA * scale
            },
            (Vector2){0, 0},
            0.0f,
            WHITE
        );

        EndDrawing();
    }
    
    estrelasDestroy(&estrelas);

    mapaDestroy(mapa);

    scoreDestroy(scores);

    UnloadTexture(background);

    UnloadTexture(starboy.sprite);
    UnloadTexture(plasmagirl.sprite);

    UnloadRenderTexture(target);

    CloseWindow();

    return 0;
}
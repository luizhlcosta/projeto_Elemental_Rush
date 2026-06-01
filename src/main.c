#include <stdio.h>
#include "raylib.h"
#include "jogador.h"
#include "mapa.h"
#include "animacao.h"
#include "jogo.h"
#include "score.h"
#include "musica.h"
#include <time.h>
#include <math.h>

#define LARGURA  1280
#define ALTURA   720

#define PROGRESSO_PATH "saves/progresso.txt" 

static int progressoLer(void) {                
    FILE *f = fopen(PROGRESSO_PATH, "r");
    if (!f) return 0;
    int mapa = 0;
    fscanf(f, "%d", &mapa);
    fclose(f);
    return mapa;
}

static void progressoSalvar(int mapaDesbloqueado) { 
    system("mkdir -p saves");   
    FILE *f = fopen(PROGRESSO_PATH, "w");
    if (!f) return;
    fprintf(f, "%d\n", mapaDesbloqueado);
    fclose(f);
}


static float posInicio[TOTAL_MAPAS][2][2] = {
    
    { {80.0f, 660.0f}, {980.0f, 540.0f} },
    
    { {980.0f, 480.0f}, {80.0f, 480.0f} },

    { {1100.0f, 530.0f}, {0.0f, 530.0f} },

    { {0.0f,   600.0f}, {1150.0f,600.0f} },

    { {0.0f,    480.0f}, {1150.0f, 480.0f} },
};

static Vector2 posEstrelasMapa[TOTAL_MAPAS][3] = {

    { {40, 420}, {300, 180}, {1200, 220} },
    
    { {600, 200}, {120,  300}, {1000, 280} },

    { {50, 200}, {600, 650}, {630, 350} },

    { {860, 100}, {50, 360}, {1150, 370} },

    { {40, 220}, {620, 200}, {980, 300} },
};

typedef struct {
    Jogador lava;
    Jogador ice;
    Mapa   *mapa;
    ListaEstrelas *estrelas;
    time_t  inicio;
    int     tempoFinal;
    int     mapaSelecionado;
    int     tempoAcumulado;
} EstadoJogo;

static void estadoJogoInit(EstadoJogo *e, int mapaSelecionado) {
    e->mapaSelecionado = mapaSelecionado;

    jogadorInit(&e->lava,
        posInicio[mapaSelecionado][0][0],
        posInicio[mapaSelecionado][0][1],
        YELLOW, 'S');
    jogadorInit(&e->ice,
        posInicio[mapaSelecionado][1][0],
        posInicio[mapaSelecionado][1][1],
        WHITE, 'P');

    e->mapa    = mapaInitNivel(mapaSelecionado);
    e->estrelas = estrelasInit(posEstrelasMapa[mapaSelecionado], 3);
    e->inicio   = 0;
    e->tempoFinal = 0;
}

static void estadoJogoLiberarFase(EstadoJogo *e) {
    estrelasDestroy(&e->estrelas);
    mapaDestroy(e->mapa);
    jogadorDestroi(&e->lava);
    jogadorDestroi(&e->ice);
}

static void estadoJogoDestroy(EstadoJogo *e) {
    estadoJogoLiberarFase(e);
    e->tempoAcumulado = 0;
}

int main() {

    InitWindow(LARGURA, ALTURA, "Elemental Rush");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL);  
    SetTargetFPS(120);

    musicaInit();

    Texture2D background = LoadTexture("assets/background_vulcanico.png");

    RenderTexture2D target = LoadRenderTexture(LARGURA, ALTURA);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    ListaScores *scores = scoreInit();
    scoreLer(scores);

    TelaAtual tela = TELA_INICIO;
    int mapaSelecionado = 0;   
    int mapaDesbloqueado = progressoLer(); 

    EstadoJogo jogo = {0};

    while (!WindowShouldClose()) {

        musicaUpdate();

        switch (tela) {

            case TELA_INICIO:
                if (IsKeyPressed(KEY_ENTER)) {
                    tela = TELA_SELECAO_MAPA;
                    mapaSelecionado = 0;
                }
                if (IsKeyPressed(KEY_ESCAPE))
                    tela = TELA_SAIR;
                break;

            case TELA_SELECAO_MAPA:
                if (IsKeyPressed(KEY_UP))
                    mapaSelecionado = (mapaSelecionado - 1 + TOTAL_MAPAS) % TOTAL_MAPAS;

                if (IsKeyPressed(KEY_DOWN))
                    mapaSelecionado = (mapaSelecionado + 1) % TOTAL_MAPAS;

                if (IsKeyPressed(KEY_ENTER)) {
                    if (mapaSelecionado <= mapaDesbloqueado) {
                        jogo.tempoAcumulado = 0;
                        estadoJogoInit(&jogo, mapaSelecionado);
                        jogo.inicio = time(NULL);
                        tela = TELA_JOGO;
                    }
                }

                if (IsKeyPressed(KEY_ESCAPE))
                    tela = TELA_INICIO;

                break;

            case TELA_JOGO: {

                if (IsKeyPressed(KEY_M)) musicaToggle();

                // Lava - WASD
                jogo.lava.velocidade.x = 0;
                if (IsKeyDown(KEY_A)) jogo.lava.velocidade.x = -200;
                if (IsKeyDown(KEY_D)) jogo.lava.velocidade.x =  200;

                // Ice - Setas
                jogo.ice.velocidade.x = 0;
                if (IsKeyDown(KEY_LEFT))  jogo.ice.velocidade.x = -200;
                if (IsKeyDown(KEY_RIGHT)) jogo.ice.velocidade.x =  200;

                jogadorUpdate(&jogo.lava,    jogo.mapa);
                jogadorUpdate(&jogo.ice, jogo.mapa);

                if (IsKeyDown(KEY_W))   jogadorPulaLava(&jogo.lava);
                if (IsKeyDown(KEY_UP))  jogadorPulaIce(&jogo.ice);

                Rectangle rL = {
                    jogo.lava.posicao.x, jogo.lava.posicao.y,
                    jogo.lava.largura,   jogo.lava.altura
                };
                Rectangle rI = {
                    jogo.ice.posicao.x, jogo.ice.posicao.y,
                    jogo.ice.largura,   jogo.ice.altura
                };

                estrelasVerificarColeta(jogo.estrelas, rL, rI, 20.0f);

                if (mapaEhAgua(jogo.mapa, rL) || mapaEhFogo(jogo.mapa, rI) ||
                    mapaEhMorte(jogo.mapa, rL) || mapaEhMorte(jogo.mapa, rI)) {
                    jogo.tempoFinal = jogo.tempoAcumulado + (int)(time(NULL) - jogo.inicio);
                    musicaToggle();
                    musicaTocaMorte();
                    tela = TELA_GAMEOVER;
                    break;
                }

                bool LavaNaPorta = mapaLavaVenceu(jogo.mapa, &jogo.lava);
                bool iceNaPorta     = mapaIceVenceu(jogo.mapa, &jogo.ice);
                bool todasEstrelas     = estrelasPodeProsseguir(jogo.estrelas);

                if (LavaNaPorta && iceNaPorta && todasEstrelas) {
                    int tempoEstaFase = (int)(time(NULL) - jogo.inicio);
                    jogo.tempoAcumulado += tempoEstaFase;
                    jogo.tempoFinal = jogo.tempoAcumulado;

                    int proximoMapa = jogo.mapaSelecionado + 1;

                    if (proximoMapa < TOTAL_MAPAS) {
                        if (proximoMapa > mapaDesbloqueado) {
                            mapaDesbloqueado = proximoMapa;
                            progressoSalvar(mapaDesbloqueado); 
                        }
                        estadoJogoLiberarFase(&jogo);
                        estadoJogoInit(&jogo, proximoMapa);
                        jogo.inicio = time(NULL);
                    } else {
                        scoreInserir(scores, "Jogador", jogo.tempoFinal);
                        scoreSalvar(scores);
                        tela = TELA_VITORIA;
                    }
                }

                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoJogoDestroy(&jogo);
                    tela = TELA_SELECAO_MAPA;
                }

                break;
            }

            case TELA_VITORIA:
                if (IsKeyPressed(KEY_ENTER)) {
                    estadoJogoDestroy(&jogo);
                    tela = TELA_SAIR;
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoJogoDestroy(&jogo);
                    tela = TELA_INICIO;
                }
                break;

            case TELA_GAMEOVER:
                if (IsKeyPressed(KEY_ENTER)) {
                    estadoJogoDestroy(&jogo);
                    tela = TELA_SAIR;
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoJogoDestroy(&jogo);
                    tela = TELA_SELECAO_MAPA;
                }
                if (IsKeyPressed(KEY_R)) {
                    musicaResumir();
                    int mapaAtual = jogo.mapaSelecionado;
                    estadoJogoDestroy(&jogo);
                    estadoJogoInit(&jogo, mapaAtual);
                    jogo.inicio = time(NULL);
                    tela = TELA_JOGO;
                }
                break;

            default:
                break;
        }

        if (tela == TELA_SAIR) break;

        BeginTextureMode(target);
        ClearBackground(BLACK);

        switch (tela) {

            case TELA_INICIO:
                telaInicioDesenha(scores);
                break;

            case TELA_SELECAO_MAPA:
                telaSelecaoMapaDesenha(mapaSelecionado, mapaDesbloqueado);
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
                jogadorDesenha(&jogo.lava, jogo.mapa);
                jogadorDesenha(&jogo.ice, jogo.mapa);

               
                int tempoTotal = jogo.tempoAcumulado + (int)(time(NULL) - jogo.inicio);
                DrawText(TextFormat("Tempo: %ds", tempoTotal), 10, 10, 20, WHITE);
                DrawText(TextFormat("Estrelas: %d/3", jogo.estrelas->coletadas), LARGURA - 180, 10, 20, GOLD);
                DrawText(TextFormat("Fase: %d/%d", jogo.mapaSelecionado + 1, TOTAL_MAPAS),
                         LARGURA / 2 - 40, 10, 20, WHITE);

                if (estrelasPodeProsseguir(jogo.estrelas)) {
                    DrawText("PORTA ABERTA! Va ate a saida!", LARGURA / 2 - 160, 40, 20, GREEN);
                }

                DrawText("ESC = Menu", LARGURA - 130, ALTURA - 30, 18, (Color){200,200,200,180});
                DrawText(musicaEstaAtiva() ? "M = Musica: ON" : "M = Musica: OFF", LARGURA - 160, ALTURA - 55, 18, (Color){200,200,200,180});
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

    scoreDestroy(scores);
    UnloadTexture(background);
    UnloadRenderTexture(target);
    musicaDestroy();
    CloseWindow();

    return 0;
}
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


// Posições iniciais dos jogadores por mapa
// [mapa][jogador] -> {x, y}
static float posInicio[TOTAL_MAPAS][2][2] = {
    // Mapa 0 (Vulcão): Starboy à esquerda, PlasmaGirl à direita
    { {80.0f, 480.0f}, {980.0f, 480.0f} },
    // Mapa 1 (Caverna de Gelo): Starboy à direita, PlasmaGirl à esquerda
    { {980.0f, 480.0f}, {80.0f, 480.0f} },

    { {1280.0f, 480.0f}, {0.0f, 480.0f} },

    { {0.0f,   480.0f}, {1000.0f,480.0f} },

    { {0.0f,    480.0f}, {1150.0f, 480.0f} },
};

// Estrelas por mapa
static Vector2 posEstrelasMapa[TOTAL_MAPAS][3] = {
    // Mapa 0
    { {14*40, 5*40}, {25*40, 8*40}, {14*40, 9*40} },
    // Mapa 1
    { {16*40, 6*40}, {8*40,  8*40}, {24*40, 8*40} },

    { {12*40, 9*40}, {15*40, 5*40}, {18*40, 9*40} },

    { {15*40, 6*40}, {9*40, 10*40}, {22*40, 10*40} },

    { {6*40,  4*40}, {15*40, 11*40}, {25*40,  4*40} },
};

typedef struct {
    Jogador starboy;
    Jogador plasmagirl;
    Mapa   *mapa;
    ListaEstrelas *estrelas;
    time_t  inicio;
    int     tempoFinal;
    int     mapaSelecionado;

    // Tempo acumulado de fases anteriores (para placar final correto)
    int     tempoAcumulado;
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
    // tempoAcumulado NÃO é zerado aqui pois pode vir de uma fase anterior
}

// Libera apenas os recursos de mapa/jogadores/estrelas (sem zerar tempoAcumulado)
static void estadoJogoLiberarFase(EstadoJogo *e) {
    estrelasDestroy(&e->estrelas);
    mapaDestroy(e->mapa);
    jogadorDestroi(&e->starboy);
    jogadorDestroi(&e->plasmagirl);
}

// Libera tudo e zera o acumulado (usado ao voltar ao menu)
static void estadoJogoDestroy(EstadoJogo *e) {
    estadoJogoLiberarFase(e);
    e->tempoAcumulado = 0;
}

int main() {

    InitWindow(LARGURA, ALTURA, "Elemental Rush");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL);  // desativa ESC como tecla de fechar janela
    SetTargetFPS(120);

    musicaInit();

    Texture2D background = LoadTexture("assets/background_vulcanico.png");

    RenderTexture2D target = LoadRenderTexture(LARGURA, ALTURA);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    // Scores
    ListaScores *scores = scoreInit();
    scoreLer(scores);

    // Estado do jogo
    TelaAtual tela = TELA_INICIO;
    int mapaSelecionado = 0;   // cursor da tela de seleção
    int mapaDesbloqueado = progressoLer(); 

    EstadoJogo jogo = {0};

    while (!WindowShouldClose()) {

        // ── AUDIO UPDATE ────────────────────────────────────
        musicaUpdate();

        // ── UPDATE ──────────────────────────────────────────
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
                    // Só entra se o mapa estiver desbloqueado
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

                // Morte por perigo elemental ou zona de morte
                if (mapaEhAgua(jogo.mapa, rS) || mapaEhFogo(jogo.mapa, rP) ||
                    mapaEhMorte(jogo.mapa, rS) || mapaEhMorte(jogo.mapa, rP)) {
                    jogo.tempoFinal = jogo.tempoAcumulado + (int)(time(NULL) - jogo.inicio);
                    musicaToggle();
                    musicaTocaMorte();
                    tela = TELA_GAMEOVER;
                    break;
                }

                // Verifica se ambos os jogadores chegaram à porta
                bool starboyNaPorta    = mapaPlasmaGirlVenceu(jogo.mapa, &jogo.starboy);
                bool plasmaGirlNaPorta = mapaStarboyVenceu(jogo.mapa, &jogo.plasmagirl);
                bool todasEstrelas     = estrelasPodeProsseguir(jogo.estrelas);

                if (starboyNaPorta && plasmaGirlNaPorta && todasEstrelas) {
                    // Soma o tempo desta fase ao acumulado
                    int tempoEstaFase = (int)(time(NULL) - jogo.inicio);
                    jogo.tempoAcumulado += tempoEstaFase;
                    jogo.tempoFinal = jogo.tempoAcumulado;

                    int proximoMapa = jogo.mapaSelecionado + 1;

                    if (proximoMapa < TOTAL_MAPAS) {
                        // ── Avança para a próxima fase ──────────────────
                        // Desbloqueia o próximo mapa na tela de seleção
                        if (proximoMapa > mapaDesbloqueado) {
                            mapaDesbloqueado = proximoMapa;
                            progressoSalvar(mapaDesbloqueado); 
                        }
                        estadoJogoLiberarFase(&jogo);
                        estadoJogoInit(&jogo, proximoMapa);
                        jogo.inicio = time(NULL);
                        // tela permanece TELA_JOGO — transição transparente
                    } else {
                        // ── Último mapa concluído: vitória total ─────────
                        // Salva o tempo automaticamente como "Jogador"
                        scoreInserir(scores, "Jogador", jogo.tempoFinal);
                        scoreSalvar(scores);
                        tela = TELA_VITORIA;
                    }
                }

                // ESC volta ao menu de seleção de mapa
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

        // ── DRAW ────────────────────────────────────────────
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
                jogadorDesenha(&jogo.starboy, jogo.mapa);
                jogadorDesenha(&jogo.plasmagirl, jogo.mapa);

                // HUD
                int tempoTotal = jogo.tempoAcumulado + (int)(time(NULL) - jogo.inicio);
                DrawText(TextFormat("Tempo: %ds", tempoTotal), 10, 10, 20, WHITE);
                DrawText(TextFormat("Estrelas: %d/3", jogo.estrelas->coletadas), LARGURA - 180, 10, 20, GOLD);
                DrawText(TextFormat("Fase: %d/%d", jogo.mapaSelecionado + 1, TOTAL_MAPAS),
                         LARGURA / 2 - 40, 10, 20, WHITE);

                // Indicador de porta: só aparece quando todas as estrelas foram pegas
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

    // Libera recursos globais
    scoreDestroy(scores);
    UnloadTexture(background);
    UnloadRenderTexture(target);
    musicaDestroy();
    CloseWindow();

    return 0;
}
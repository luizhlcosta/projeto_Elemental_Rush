//mapa.c

#include <stdlib.h>
#include "mapa.h"
#include "tipos.h"
#include "raylib.h"

// Mapa do nível 1
static int nivel1[18][32] = {

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},

    {0 ,0 ,10,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,9 ,0 ,0},

    {8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8},

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,0 ,0 ,0 ,0 ,0},

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},

    {8 ,8 ,0 ,0 ,8 ,8 ,0 ,0 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,0 ,0 ,8 ,8},

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,2 ,3 ,4 ,8 ,8 ,5 ,6 ,7 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 , 8, 8,8 ,8 ,8 ,8 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},

    {8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,8 ,8 ,8 ,8 ,8},

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},

    {8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8},

    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},

    {11,11,11,11,11,11,1,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11}
};

// Mapa do nível 2 - Caverna de Gelo
// Starboy (S, porta 10) começa à direita; PlasmaGirl (P, porta 9) começa à esquerda
// Fogo mata PlasmaGirl; Água mata Starboy
static int nivel2[18][32] = {
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {0 ,9 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,10,0},
    {8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8},
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,8 ,8 ,8 ,0 ,0 ,0 ,0 ,8 ,8 ,8 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {0 ,0 ,8 ,8 ,0 ,0 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,0 ,0 ,8 ,8 ,0 ,0},
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,5 ,6 ,7 ,8 ,8 ,2 ,3 ,4 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {8 ,8 ,8 ,8 ,8 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,8 ,8 ,8 ,8 ,8},
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8},
    {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},
    {11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11}
};

static void mapaCarregaTexturas(Mapa *m) {
    m->texParede   = LoadTexture("assets/tiles/parede.png");
    m->texFogo1    = LoadTexture("assets/tiles/fogo1.png");
    m->texFogo2    = LoadTexture("assets/tiles/fogo2.png");
    m->texFogoMeio = LoadTexture("assets/tiles/fogoMeio.png");
    m->texAgua1    = LoadTexture("assets/tiles/agua1.png");
    m->texAgua2    = LoadTexture("assets/tiles/agua2.png");
    m->texAguaMeio = LoadTexture("assets/tiles/aguaMeio.png");
    m->texPortaS   = LoadTexture("assets/tiles/porta_s.png");
    m->texPortaP   = LoadTexture("assets/tiles/porta_p.png");
    m->texChao     = LoadTexture("assets/tiles/chao.png");

    SetTextureFilter(m->texParede,   TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texFogo1,    TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texFogo2,    TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texFogoMeio, TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texAgua1,    TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texAgua2,    TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texAguaMeio, TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texPortaS,   TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texPortaP,   TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texChao,     TEXTURE_FILTER_POINT);
}

Mapa* mapaInitNivel(int nivel) {
    Mapa *m = (Mapa*) malloc(sizeof(Mapa));
    m->linhas   = MAPA_LINHAS;
    m->colunas  = MAPA_COLUNAS;
    m->tileSize = TILE_SIZE;

    int (*grade)[32] = (nivel == 1) ? nivel2 : nivel1;

    m->grade = (int**) malloc(m->linhas * sizeof(int*));
    for (int i = 0; i < m->linhas; i++) {
        m->grade[i] = (int*) malloc(m->colunas * sizeof(int));
        for (int j = 0; j < m->colunas; j++) {
            m->grade[i][j] = grade[i][j];
        }
    }

    mapaCarregaTexturas(m);
    return m;
}

Mapa* mapaInit() {
    return mapaInitNivel(0);
}

void mapaDestroy(Mapa *m) {
    for (int i = 0; i < m->linhas; i++) free(m->grade[i]);

    UnloadTexture(m->texParede);
    UnloadTexture(m->texFogo1);
    UnloadTexture(m->texFogo2);
    UnloadTexture(m->texFogoMeio);
    UnloadTexture(m->texAgua1);
    UnloadTexture(m->texAgua2);
    UnloadTexture(m->texPortaS);
    UnloadTexture(m->texPortaP);
    UnloadTexture(m->texChao);

    free(m->grade);
    free(m);
}

int mapaEhMorte(Mapa *m, Rectangle rect) {
    int x = (int)((rect.x + rect.width/2) / m->tileSize);
    int y = (int)((rect.y + rect.height/2) / m->tileSize);
    if (y >= 0 && y < m->linhas && x >= 0 && x < m->colunas)
        return m->grade[y][x] == MORTE;
    return 0;
}

void mapaDesenha(Mapa *m) {

    for (int y = 0; y < m->linhas; y++) {

        for (int x = 0; x < m->colunas; x++) {

            int tile = m->grade[y][x];

            int posX = x * m->tileSize;
            int posY = y * m->tileSize;

            switch (tile) {

                case PAREDE:
                    DrawTexture(m->texParede, posX, posY, WHITE);
                    break;

                case FOGO_1:
                    DrawTexture(m->texFogo1, posX, posY, WHITE);
                    break;

                case FOGO_2:
                    DrawTexture(m->texFogo2, posX, posY, WHITE);
                    break;

                case FOGO_MEIO:
                    DrawTexture(m->texFogoMeio, posX, posY, WHITE);
                    break;

                case AGUA_1:
                    DrawTexture(m->texAgua1, posX, posY, WHITE);
                    break;
                
                case AGUA_2:
                    DrawTexture(m->texAgua2, posX, posY, WHITE);
                    break;

                case AGUA_MEIO:
                    DrawTexture(m->texAguaMeio, posX, posY, WHITE);
                    break;

                case PORTA_S:
                    DrawTexture(m->texPortaS, posX, posY, WHITE);
                    break;

                case PORTA_P:
                    DrawTexture(m->texPortaP, posX, posY, WHITE);
                    break;

                case TILE_CHAO:
                    DrawTexture(m->texChao, posX, posY, WHITE);
                    break;
                case MORTE:
                    break;

                default:
                    break;
            }
        }
    }
}

int mapaEhParede(Mapa *m, Rectangle rect) {
    int x1 = (int)(rect.x / m->tileSize);
    int y1 = (int)(rect.y / m->tileSize);
    int x2 = (int)((rect.x + rect.width - 1) / m->tileSize);
    int y2 = (int)((rect.y + rect.height - 1) / m->tileSize);

    for (int y = y1; y <= y2; y++)
        for (int x = x1; x <= x2; x++)
            if (y >= 0 && y < m->linhas && x >= 0 && x < m->colunas)
                if (m->grade[y][x] == TILE_CHAO) {
                    return 1;
                }
    return 0;
}


int mapaEhFogo(Mapa *m, Rectangle rect) {
    int x = (int)((rect.x + rect.width/2) / m->tileSize);
    int y = (int)((rect.y + rect.height/2) / m->tileSize);
    if (y >= 0 && y < m->linhas && x >= 0 && x < m->colunas)
        return m->grade[y][x] == FOGO_1 || m->grade[y][x] == FOGO_2 || m->grade[y][x] == FOGO_MEIO;
    return 0;
}

int mapaEhAgua(Mapa *m, Rectangle rect) {
    int x = (int)((rect.x + rect.width/2) / m->tileSize);
    int y = (int)((rect.y + rect.height/2) / m->tileSize);
    if (y >= 0 && y < m->linhas && x >= 0 && x < m->colunas)
        return m->grade[y][x] == AGUA_1 || m->grade[y][x] == AGUA_2 || m->grade[y][x] == AGUA_MEIO;
    return 0;
}

int mapaStarboyVenceu(Mapa *m, Jogador *j) {
    int x = (int)((j->posicao.x + j->largura/2) / m->tileSize);
    int y = (int)((j->posicao.y + j->altura/2) / m->tileSize);
    if (y >= 0 && y < m->linhas && x >= 0 && x < m->colunas)
        return m->grade[y][x] == PORTA_S;
    return 0;
}

int mapaPlasmaGirlVenceu(Mapa *m, Jogador *j) {
    int x = (int)((j->posicao.x + j->largura/2) / m->tileSize);
    int y = (int)((j->posicao.y + j->altura/2) / m->tileSize);
    if (y >= 0 && y < m->linhas && x >= 0 && x < m->colunas)
        return m->grade[y][x] == PORTA_P;
    return 0;
}

ListaEstrelas* estrelasInit(Vector2 *posicoes, int quantidade) {
    ListaEstrelas *lista = (ListaEstrelas*) malloc(sizeof(ListaEstrelas));
    lista->inicio    = NULL;
    lista->total     = quantidade;
    lista->coletadas = 0;

    for (int i = quantidade - 1; i >= 0; i--) {
        StarNode *novo  = (StarNode*) malloc(sizeof(StarNode));
        novo->posicao   = posicoes[i];
        novo->coletada  = false;
        novo->proximo   = lista->inicio;
        lista->inicio   = novo;
    }
    return lista;
}

void estrelasVerificarColeta(ListaEstrelas *lista, Rectangle jog1, Rectangle jog2, float raio) {
    StarNode *atual = lista->inicio;
    while (atual != NULL) {
        if (!atual->coletada) {
            if (CheckCollisionCircleRec(atual->posicao, raio, jog1) ||
                CheckCollisionCircleRec(atual->posicao, raio, jog2)) {
                atual->coletada = true;
                lista->coletadas++;
            }
        }
        atual = atual->proximo;
    }
}

bool estrelasPodeProsseguir(ListaEstrelas *lista) {
    return lista->coletadas == lista->total;
}

void estrelasDesenhar(ListaEstrelas *lista) {
    StarNode *atual = lista->inicio;
    while (atual != NULL) {
        if (!atual->coletada) {
            DrawPoly(atual->posicao, 5, 14.0f, 18.0f, GOLD);
            DrawPolyLinesEx(atual->posicao, 5, 14.0f, 18.0f, 1.5f, GOLD);
        }
        atual = atual->proximo;
    }
}

void estrelasDestroy(ListaEstrelas **lista) {
    StarNode *atual = (*lista)->inicio;
    while (atual != NULL) {
        StarNode *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    free(*lista);
    *lista = NULL;
}
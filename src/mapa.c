//mapa.c

#include <stdlib.h>
#include "mapa.h"
#include "tipos.h"
#include "raylib.h"

// Mapa do nível 1
static int nivel1[21][29] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0},
    {6,6,6,6,6,6,6,6,6,6,6,6,6,0,0,0,0,0,6,6,6,6,6,6,6,6,6,6,6},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,6,3,3,6,3,3,6,6,6,6,0,0,0,0,0,0,0,0,6,6,6,6,2,2,2,2,2,0},
    {6,6,1,1,0,1,1,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,6,6,6,6,6,6},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,6,6,6,6,6,6,0,0,0,6,6,6,6,6,6,6,0,0,0,0,0,0},
    {6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8}
};

static Vector2 estrelasNivel1[3] = {
    {13 * TILE_SIZE, 4 * TILE_SIZE},  // plataforma central do topo
    { 3 * TILE_SIZE, 8 * TILE_SIZE},  // plataforma esquerda do meio
    {14 * TILE_SIZE, 12 * TILE_SIZE}  // plataforma central inferior
};

Mapa* mapaInit() {
    Mapa *m = (Mapa*) malloc(sizeof(Mapa));
    m->linhas   = MAPA_LINHAS;
    m->colunas  = MAPA_COLUNAS;
    m->tileSize = TILE_SIZE;

    m->grade = (int**) malloc(m->linhas * sizeof(int*));
    for (int i = 0; i < m->linhas; i++) {   
        m->grade[i] = (int*) malloc(m->colunas * sizeof(int));
        for (int j = 0; j < m->colunas; j++) {
            m->grade[i][j] = nivel1[i][j];
        }
    }

    m->texParede = LoadTexture("assets/tiles/parede.png");
    m->texFogo   = LoadTexture("assets/tiles/fogo.png");
    m->texAgua1  = LoadTexture("assets/tiles/agua1.png");
    m->texAgua2  = LoadTexture("assets/tiles/agua2.png");
    m->texPortaS = LoadTexture("assets/tiles/porta_s.png");
    m->texPortaP = LoadTexture("assets/tiles/porta_p.png");
    m->texChao   = LoadTexture("assets/tiles/chao.png");

    SetTextureFilter(m->texParede, TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texFogo,   TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texAgua1,   TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texAgua2,   TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texPortaS, TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texPortaP, TEXTURE_FILTER_POINT);
    SetTextureFilter(m->texChao,   TEXTURE_FILTER_POINT);

    return m;
}

void mapaDestroy(Mapa *m) {
    for (int i = 0; i < m->linhas; i++) free(m->grade[i]);

    UnloadTexture(m->texParede);
    UnloadTexture(m->texFogo);
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

                case FOGO:
                    DrawTexture(m->texFogo, posX, posY, WHITE);
                    break;

                case AGUA_1:
                    DrawTexture(m->texAgua1, posX, posY, WHITE);
                    break;
                
                case AGUA_2:
                    DrawTexture(m->texAgua2, posX, posY, WHITE);
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
                if (m->grade[y][x] == PAREDE || m->grade[y][x] == TILE_CHAO) return 1;
    return 0;
}


int mapaEhFogo(Mapa *m, Rectangle rect) {
    int x = (int)((rect.x + rect.width/2) / m->tileSize);
    int y = (int)((rect.y + rect.height/2) / m->tileSize);
    if (y >= 0 && y < m->linhas && x >= 0 && x < m->colunas)
        return m->grade[y][x] == FOGO;
    return 0;
}

int mapaEhAgua(Mapa *m, Rectangle rect) {
    int x = (int)((rect.x + rect.width/2) / m->tileSize);
    int y = (int)((rect.y + rect.height/2) / m->tileSize);
    if (y >= 0 && y < m->linhas && x >= 0 && x < m->colunas)
        return m->grade[y][x] == AGUA_1 || m->grade[y][x] == AGUA_2;
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

void estrelasVerificarColeta(ListaEstrelas *lista, Vector2 jog1, Vector2 jog2, float raio) {
    StarNode *atual = lista->inicio;
    while (atual != NULL) {
        if (!atual->coletada) {
            if (CheckCollisionPointCircle(jog1, atual->posicao, raio) ||
                CheckCollisionPointCircle(jog2, atual->posicao, raio)) {
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
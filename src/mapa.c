#include <stdlib.h>
#include "mapa.h"
#include "tipos.h"
#include "raylib.h"

// Mapa do nível 1
static int nivel1[MAPA_LINHAS][MAPA_COLUNAS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1},
    {1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,1,1,0,0,0,0,2,2,2,2,0,0,0,0,1,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,0,0,0,3,3,3,0,0,0,0,0,3,3,3,0,0,0,0,1,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
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
    return m;
}

void mapaDestroy(Mapa *m) {
    for (int i = 0; i < m->linhas; i++) free(m->grade[i]);
    free(m->grade);
    free(m);
}

void mapaDesenha(Mapa *m) {
    for (int y = 0; y < m->linhas; y++) {
        for (int x = 0; x < m->colunas; x++) {
            int tile = m->grade[y][x];
            Rectangle rect = {x * m->tileSize, y * m->tileSize, m->tileSize, m->tileSize};
            switch (tile) {
                case PAREDE:
                    DrawRectangleRec(rect, DARKGRAY);
                    DrawRectangle(x * m->tileSize,     y * m->tileSize,     m->tileSize, 3, LIGHTGRAY); // topo
                    DrawRectangle(x * m->tileSize,     y * m->tileSize,     3, m->tileSize, LIGHTGRAY); // esquerda
                    DrawRectangle(x * m->tileSize,     y * m->tileSize + m->tileSize - 3, m->tileSize, 3, BLACK); // base
                    DrawRectangle(x * m->tileSize + m->tileSize - 3, y * m->tileSize, 3, m->tileSize, BLACK); // direita
                    break;
                case FOGO:
                    DrawRectangleRec(rect, (Color){200, 80, 0, 255});
                    DrawRectangle(x * m->tileSize, y * m->tileSize, m->tileSize, 3, (Color){255, 160, 0, 255});
                    break;
                case AGUA:
                    DrawRectangleRec(rect, (Color){0, 80, 180, 255});
                    DrawRectangle(x * m->tileSize, y * m->tileSize, m->tileSize, 3, (Color){0, 160, 255, 255});
                    break;
                case PORTA_S:
                    DrawRectangleRec(rect, RED);
                    DrawRectangleLinesEx(rect, 3, MAROON);
                    DrawText("S", x * m->tileSize + 8, y * m->tileSize + 6, 20, WHITE);
                    break;
                case PORTA_P:
                    DrawRectangleRec(rect, SKYBLUE);
                    DrawRectangleLinesEx(rect, 3, BLUE);
                    DrawText("P", x * m->tileSize + 8, y * m->tileSize + 6, 20, WHITE);
                    break;
                default: break;
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
                if (m->grade[y][x] == PAREDE) return 1;
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
        return m->grade[y][x] == AGUA;
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
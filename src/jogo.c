#include "jogo.h"
#include "raylib.h"
#include <stddef.h>

void telaInicioDesenha() {
    ClearBackground(BLACK);

    DrawText("ELEMENTAL RUSH", 220, 150, 60, RED);
    DrawText("RUSH", 320, 220, 60, YELLOW);

    DrawText("S - Starboy    usa: A D e W para pular", 200, 340, 22, YELLOW);
    DrawText("P - PlasmaGirl usa: Setas e Cima para pular", 200, 370, 22, YELLOW);

    DrawText("* Fogo = mata PlasmaGirl", 220, 420, 18, ORANGE);
    DrawText("~ Agua = mata Starboy",    220, 445, 18, BLUE);

    DrawText("Pressione ENTER para jogar", 230, 510, 24, WHITE);
}

void telaVitoriaDesenha(int tempo, ListaScores *scores) {
    ClearBackground(BLACK);

    DrawText("VITORIA!", 270, 100, 70, YELLOW);
    DrawText(TextFormat("Tempo: %d segundos", tempo), 270, 190, 28, WHITE);

    DrawText("TOP SCORES:", 270, 260, 28, YELLOW);

    No *atual = scores->inicio;
    int y = 300;
    int pos = 1;
    while (atual != NULL && pos <= 5) {
        DrawText(TextFormat("%d. %-15s %ds", pos, atual->nome, atual->tempo), 270, y, 24, WHITE);
        atual = atual->prox;
        y += 35;
        pos++;
    }

    DrawText("Pressione ENTER para sair", 240, 530, 24, LIGHTGRAY);
}

void telaGameOverDesenha() {
    ClearBackground(BLACK);

    DrawText("GAME OVER", 220, 180, 70, RED);
    DrawText("Um dos jogadores morreu!", 200, 280, 28, WHITE);
    DrawText("Pressione ENTER para sair", 210, 530, 24, LIGHTGRAY);
}

void telaScoresDesenha(ListaScores *scores) {
    ClearBackground(BLACK);

    DrawText("TOP SCORES", 270, 80, 50, YELLOW);

    No *atual = scores->inicio;
    int y = 180;
    int pos = 1;
    while (atual != NULL && pos <= 10) {
        DrawText(TextFormat("%d. %-15s %ds", pos, atual->nome, atual->tempo), 200, y, 26, WHITE);
        atual = atual->prox;
        y += 40;
        pos++;
    }

    if (scores->inicio == NULL) {
        DrawText("Nenhum score ainda!", 230, 300, 28, LIGHTGRAY);
    }

    DrawText("Pressione ENTER para voltar", 210, 530, 24, LIGHTGRAY);
}
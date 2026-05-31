#include "jogo.h"
#include "raylib.h"
#include <stddef.h>

void telaInicioDesenha(ListaScores *scores) {
    ClearBackground(BLACK);
    DrawText("ELEMENTAL", 220, 150, 60, RED);
    DrawText("RUSH", 320, 220, 60, YELLOW);
    DrawText("S - Starboy    usa: A D e W para pular", 200, 340, 22, YELLOW);
    DrawText("P - PlasmaGirl usa: Setas e Cima para pular", 200, 370, 22, YELLOW);
    DrawText("* Fogo = mata PlasmaGirl", 220, 420, 18, ORANGE);
    DrawText("~ Agua = mata Starboy",    220, 445, 18, BLUE);

    // Exibe o recorde se existir
    if (scores != NULL && scores->inicio != NULL) {
        DrawText("RECORDE:", 390, 478, 20, GOLD);
        DrawText(TextFormat("%ds", scores->inicio->tempo), 510, 478, 20, GOLD);
    }

    DrawText("Pressione ENTER para jogar", 230, 510, 24, WHITE);
    DrawText("ESC - Fechar o jogo", 230, 545, 20, (Color){160, 160, 160, 255});
}

void telaVitoriaDesenha(int tempo, ListaScores *scores) {
    ClearBackground(BLACK);
    DrawText("VITORIA!", 270, 100, 70, YELLOW);
    DrawText(TextFormat("Seu tempo: %ds", tempo), 270, 190, 28, WHITE);

    // Compara com o recorde (o melhor tempo é sempre o primeiro da lista)
    if (scores != NULL && scores->inicio != NULL) {
        int recorde = scores->inicio->tempo;
        if (tempo <= recorde) {
            DrawText("NOVO RECORDE!", 270, 235, 26, GOLD);
        } else {
            DrawText(TextFormat("Recorde: %ds  (faltaram %ds)", recorde, tempo - recorde), 270, 235, 22, LIGHTGRAY);
        }
    }

    DrawText("TOP SCORES:", 270, 280, 28, YELLOW);
    No *atual = scores->inicio;
    int y = 318;
    int pos = 1;
    while (atual != NULL && pos <= 5) {
        Color cor = (pos == 1) ? GOLD : WHITE;
        DrawText(TextFormat("%d. %ds", pos, atual->tempo), 270, y, 24, cor);
        atual = atual->prox;
        y += 34;
        pos++;
    }
    DrawText("ENTER para sair  |  ESC para menu", 220, 530, 24, LIGHTGRAY);
}

void telaSelecaoMapaDesenha(int mapaAtual, int mapaDesbloqueado) {
    ClearBackground(BLACK);
    DrawText("SELECAO DE MAPA", 230, 80, 50, YELLOW);

    const char *nomes[5] = {
        "Mapa 1",
        "Mapa 2",
        "Mapa 3",
        "Mapa 4",
        "Mapa 5"
    };

    for (int i = 0; i < 5; i++) {
        bool bloqueado = (i > mapaDesbloqueado);
        int y = 180 + i * 55;

        if (bloqueado) {
            // Mapa travado: cinza escuro + cadeado
            DrawText(nomes[i], 240, y, 28, DARKGRAY);
            DrawText("[BLOQUEADO]", 240, y + 32, 18, (Color){120, 60, 60, 255});
        } else {
            // Mapa disponível: destaque se selecionado
            Color cor    = (i == mapaAtual) ? YELLOW : LIGHTGRAY;
            int tamanho  = (i == mapaAtual) ? 34 : 28;
            int x        = (i == mapaAtual) ? 230 : 240;
            DrawText(nomes[i], x, y, tamanho, cor);
            if (i == mapaAtual)
                DrawText(">", 195, y, 34, YELLOW);
        }
    }

    DrawText("Setas CIMA/BAIXO para navegar", 220, 480, 22, GRAY);
    DrawText("ENTER para iniciar  |  ESC para voltar", 185, 515, 22, GRAY);
}

void telaGameOverDesenha() {
    ClearBackground(BLACK);
    DrawText("GAME OVER", 220, 180, 70, RED);
    DrawText("Um dos jogadores morreu!", 200, 280, 28, WHITE);
    DrawText("R - Reiniciar fase", 320, 400, 24, YELLOW);
    DrawText("ENTER para sair  |  ESC para menu", 230, 530, 24, LIGHTGRAY);
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
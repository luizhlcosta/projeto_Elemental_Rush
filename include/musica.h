// musica.h
#ifndef MUSICA_H
#define MUSICA_H

#include "raylib.h"

// Inicializa o dispositivo de áudio e carrega a música de fundo.
// Deve ser chamado após InitWindow().
void musicaInit(void);

// Atualiza o stream de música. Deve ser chamado uma vez por frame no loop principal.
void musicaUpdate(void);

// Pausa a música em execução.
void musicaPausar(void);

// Retoma a música pausada.
void musicaResumir(void);

// Alterna entre pausado e tocando.
void musicaToggle(void);

// Libera a música e fecha o dispositivo de áudio.
// Deve ser chamado antes de CloseWindow().
void musicaDestroy(void);

void musicaTocaPulo(void);

bool musicaEstaAtiva(void);

void musicaTocaEstrela(void);

void musicaTocaMorte(void);

#endif // MUSICA_H

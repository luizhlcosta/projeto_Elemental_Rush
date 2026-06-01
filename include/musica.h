#ifndef MUSICA_H
#define MUSICA_H

#include "raylib.h"

void musicaInit(void);

void musicaUpdate(void);

void musicaPausar(void);

void musicaResumir(void);

void musicaToggle(void);

void musicaDestroy(void);

void musicaTocaPulo(void);

bool musicaEstaAtiva(void);

void musicaTocaEstrela(void);

void musicaTocaMorte(void);

#endif

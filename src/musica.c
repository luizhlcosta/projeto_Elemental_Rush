// musica.c
#include "musica.h"
#include <stdio.h>

#define MUSICA_PATH "assets/musica.mp3"
#define MUSICA_VOLUME 0.5f

static Music gMusica;
static bool  gPausada = false;

void musicaInit(void) {
    InitAudioDevice();

    gMusica = LoadMusicStream(MUSICA_PATH);

    if (gMusica.stream.buffer == NULL) {
        printf("[musica] Aviso: nao foi possivel carregar '%s'\n", MUSICA_PATH);
        return;
    }

    gMusica.looping = true;
    SetMusicVolume(gMusica, MUSICA_VOLUME);
    PlayMusicStream(gMusica);
    gPausada = false;
}

void musicaUpdate(void) {
    if (gMusica.stream.buffer == NULL) return;
    if (!gPausada)
        UpdateMusicStream(gMusica);
}

void musicaPausar(void) {
    if (gMusica.stream.buffer == NULL || gPausada) return;
    PauseMusicStream(gMusica);
    gPausada = true;
}

void musicaResumir(void) {
    if (gMusica.stream.buffer == NULL || !gPausada) return;
    ResumeMusicStream(gMusica);
    gPausada = false;
}

void musicaToggle(void) {
    if (gPausada) musicaResumir();
    else          musicaPausar();
}

void musicaDestroy(void) {
    if (gMusica.stream.buffer != NULL)
        UnloadMusicStream(gMusica);
    CloseAudioDevice();
}

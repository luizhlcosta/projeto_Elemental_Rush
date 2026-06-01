#include "musica.h"
#include <stdio.h>

#define MUSICA_PATH  "assets/sounds/musica.mp3"
#define MUSICA_VOLUME 0.20f
#define PULO_PATH    "assets/sounds/jump.wav"
#define ESTRELA_PATH "assets/sounds/estrela.mp3"
#define MORTE_PATH   "assets/sounds/morte.mp3"   

static Music gMusica;
static bool  gPausada     = false;
static bool  gMusicaAtiva = true;
static Sound gSomPulo;
static Sound gSomEstrela;
static Sound gSomMorte;                          

void musicaInit(void) {
    InitAudioDevice();

    gMusica = LoadMusicStream(MUSICA_PATH);
    if (gMusica.stream.buffer == NULL) {
        printf("[musica] Aviso: nao foi possivel carregar '%s'\n", MUSICA_PATH);
    } else {
        gMusica.looping = true;
        SetMusicVolume(gMusica, MUSICA_VOLUME);
        PlayMusicStream(gMusica);
        gPausada     = false;
        gMusicaAtiva = true;
    }

    gSomPulo = LoadSound(PULO_PATH);
    if (gSomPulo.frameCount == 0)
        printf("[musica] Aviso: nao foi possivel carregar '%s'\n", PULO_PATH);
    else
        SetSoundVolume(gSomPulo, 1.5f);

    gSomEstrela = LoadSound(ESTRELA_PATH);
    if (gSomEstrela.frameCount == 0)
        printf("[musica] Aviso: nao foi possivel carregar '%s'\n", ESTRELA_PATH);
    else
        SetSoundVolume(gSomEstrela, 2.0f);

    gSomMorte = LoadSound(MORTE_PATH);            
    if (gSomMorte.frameCount == 0)
        printf("[musica] Aviso: nao foi possivel carregar '%s'\n", MORTE_PATH);
    else
        SetSoundVolume(gSomMorte, 2.0f);
}

void musicaTocaPulo(void) {
    if (gSomPulo.frameCount == 0) return;
    PlaySound(gSomPulo);
}

void musicaTocaEstrela(void) {
    if (gSomEstrela.frameCount == 0) return;
    PlaySound(gSomEstrela);
}

void musicaTocaMorte(void) {                     
    if (gSomMorte.frameCount == 0) return;
    PlaySound(gSomMorte);
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
    if (gMusica.stream.buffer == NULL) return;
    gMusicaAtiva = !gMusicaAtiva;
    if (gMusicaAtiva) {
        ResumeMusicStream(gMusica);
        gPausada = false;
    } else {
        PauseMusicStream(gMusica);
        gPausada = true;
    }
}

bool musicaEstaAtiva(void) {
    return gMusicaAtiva;
}

void musicaDestroy(void) {
    if (gSomPulo.frameCount != 0)
        UnloadSound(gSomPulo);
    if (gSomEstrela.frameCount != 0)
        UnloadSound(gSomEstrela);
    if (gSomMorte.frameCount != 0)   
        UnloadSound(gSomMorte);
    if (gMusica.stream.buffer != NULL)
        UnloadMusicStream(gMusica);
    CloseAudioDevice();
}
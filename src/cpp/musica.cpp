#include "include/musica.h"

Musica::Musica()
{
    InitAudioDevice();
    musica_menu = LoadMusicStream("music/menu_inicial.mp3");
    musica_combate = LoadMusicStream("music/musica_combate.mp3");

    SetMusicVolume(musica_menu, masterVolume);
    SetMusicVolume(musica_combate, masterVolume);
}

Musica::~Musica()
{
    UnloadMusicStream(musica_menu);
    UnloadMusicStream(musica_combate);

    CloseAudioDevice();
}

void Musica::CambiarMenu()
{
    if (actual != nullptr)
        StopMusicStream(*actual);

    actual = &musica_menu;
    PlayMusicStream(*actual);
    SetMusicVolume(*actual, masterVolume);
}

void Musica::CambiarCombate()
{
    if (actual != nullptr)
        StopMusicStream(*actual);

    actual = &musica_combate;
    PlayMusicStream(*actual);
    SetMusicVolume(*actual, masterVolume);
}

void Musica::SetVolumen(float v)
{
    masterVolume = v;
    if (actual != nullptr)
        SetMusicVolume(*actual, masterVolume);
}

void Musica:: SetVelocidad(float factor)
{
    if (actual != nullptr)
        SetMusicPitch(*actual, factor);
}

void Musica::Update()
{
    if (actual != nullptr)
        UpdateMusicStream(*actual);
}
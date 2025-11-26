#pragma once
#include "raylib.h"

class Musica
{
private:
    Music musica_menu;
    Music musica_combate;
    
    Sound hit;

    Music* actual = nullptr;
    float masterVolume = 1.0f;

public:
    Musica();
    ~Musica();

    void CambiarMenu();
    void CambiarCombate();
    void SetVolumen(float v);
    void SetVelocidad(float factor);
    void EfectoDamage();
    void Update();
};

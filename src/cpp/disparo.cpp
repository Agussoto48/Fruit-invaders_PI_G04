#include "include/disparo.h"
#include <iostream>
#define ESCALA 0.2f

extern "C" void asm_actualizar_disparo(Vector2* position, int speed, int screenHeight, int* active);

Disparo::Disparo(Vector2 position, int speed, bool esDelEnemigo) 
{
    if(esDelEnemigo) {
        cuchillo = LoadTexture("sprites/Semilla.png");
    } 
    else {
        cuchillo = LoadTexture("sprites/Cuchillo.png");
    }
    this->position = position;
    this->speed = speed;
    active = true;
}

void Disparo::Draw() {
    if(active) {
        DrawTextureEx(cuchillo, position, 0.0, ESCALA, WHITE);
    }
}

void Disparo::Update() {
    if(active) {
        int activeInt = active ? 1 : 0;
        asm_actualizar_disparo(&position, speed, GetScreenHeight(), &activeInt);
        active = (activeInt != 0);
    }
}

Rectangle Disparo::getRect() {
   Rectangle rectangle;

    float ancho = cuchillo.width * ESCALA;
    float alto = cuchillo.height * ESCALA;
    rectangle.x = position.x + ancho/2.5;
    rectangle.y = position.y;
    rectangle.width = ancho / 5;
    rectangle.height = alto;

    return rectangle;
}
#include "include/disparo.h"
#include <iostream>

Disparo::Disparo(Vector2 position, int speed) 
{
    cuchillo = LoadTexture("sprites/Cuchillo.png");
    this->position = position;
    this->speed = speed;
    active = true;
}

void Disparo::Draw() {
    if(active) {
        DrawTextureEx(cuchillo, position, 0.0, 0.2f, WHITE);
    }
}

void Disparo::Update() {
    position.y += speed;
    if(active) {
        if(position.y > GetScreenHeight() || position.y < 0) {
            active = false;
        }
    }
}
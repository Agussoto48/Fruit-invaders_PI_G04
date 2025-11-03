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

Rectangle Disparo::getRect() {
  Rectangle rectangle;
  rectangle.x = position.x;
  rectangle.y = position.y;
  rectangle.width = 3;
  rectangle.height = 3;
  return rectangle;
}
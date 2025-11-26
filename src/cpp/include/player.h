#pragma once
#include<raylib.h>
#include "disparo.h"
#include <vector>

class Player{
private:
    Texture2D chef;
    Vector2 position;
    double tiempoDisparo; 
public:
    Player();
    ~Player();
    void Draw();
    void MoveLeft();
    void MoveRight();
    void Disparar();
    std::vector<Disparo> disparos;
    Rectangle getRect();
    
    void MoveFromJoystick(int xValue);
    bool ShouldShootFromJoystick(int buttonState);
};
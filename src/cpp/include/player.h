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
    //Construcotres
    Player();
    ~Player();
    //Funcionalidades
    void Draw();
    void MoveLeft();
    void MoveRight();
    void Disparar();
    std::vector<Disparo> disparos;
    Rectangle getRect();

};
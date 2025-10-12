#pragma once
#include<raylib.h>

class Player{
private:
    Texture2D chef;
    Vector2 position;
public:
    //Construcotres
    Player();
    ~Player();
    //Funcionalidades
    void Draw();
    void MoveLeft();
    void MoveRight();
    void Disparar();

};
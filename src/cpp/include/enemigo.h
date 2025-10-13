#pragma once
#include<raylib.h>


class Enemigo{
private:
    
public:
    Texture2D image;
    Vector2 position;
    int type;

    //Construcotres
    Enemigo(int type, Vector2 position);
    ~Enemigo();

    void Update();
    void Draw();
    int GetType();

    //Funcionalidades
    void Movimiento();
    void Disparar();

};
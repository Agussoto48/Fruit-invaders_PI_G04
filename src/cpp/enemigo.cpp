#include "include/enemigo.h"
#define ESCALA 0.2f

Enemigo::Enemigo(int type, Vector2 position)
{
    this->type = type;
    this->position = position;
    switch(type){
        case 1:
            image = LoadTexture("sprites/Manzana.png");
            break;
        case 2:
            image = LoadTexture("sprites/Piña.png");
            break;
        case 3:
            image = LoadTexture("sprites/Sandia.png");
            break;
    }
}

void Enemigo::Draw(){
    DrawTextureV(image,position, WHITE);
}

int Enemigo::GetType(){
    return type;
}

Enemigo::~Enemigo(){
}

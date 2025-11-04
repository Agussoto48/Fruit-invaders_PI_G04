#include "include/enemigo.h"
#include <iostream> 

// Declarar funciones externas de ensamblador
extern "C" void moverEnemigo(int direction, Vector2* pos);
extern "C" void moverEnemigoAbajo(int distance, Vector2* pos);

Texture2D Enemigo::enemigoImages[3] = {};

Enemigo::Enemigo(int type, Vector2 position)
{
    this->type = type;
    this->position = position;
    
    if(enemigoImages[type-1].id==0){

    switch(type){
        case 1:
            enemigoImages[0] = LoadTexture("sprites/Manzana.png");
            break;
        case 2:
            enemigoImages[1] = LoadTexture("sprites/Piña.png");
            break;
        case 3:
            enemigoImages[2] = LoadTexture("sprites/Sandia.png");
            break;
        default:
            enemigoImages[0] = LoadTexture("sprites/Manzana.png");
            break;
    }
    }
}

void Enemigo::Draw(){
    DrawTextureV(enemigoImages[type - 1],position, WHITE);
}

int Enemigo::GetType(){
    return type;
}

void Enemigo::UnloadImages(){
    for(int i = 0; i < 3 ; ++i){
        if(enemigoImages[i].id != 0){  //Se supone que cone sta verificacion ya no da segmentation fault
            UnloadTexture(enemigoImages[i]);
            enemigoImages[i].id = 0;
        }
    }
}

void Enemigo::Update(int direction){
    // Llamar a la función de ensamblador para mover el enemigo
    moverEnemigo(direction, &position);
}

void Enemigo::MoveDown(int distance){
    // Llamar a la función de ensamblador para mover el enemigo hacia abajo
    moverEnemigoAbajo(distance, &position);
}

Rectangle Enemigo::getRect() {
    return {position.x, position.y, float(enemigoImages[type - 1].width), float(enemigoImages[type - 1].height)};
}
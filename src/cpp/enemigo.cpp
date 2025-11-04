#include "include/enemigo.h"
#include <iostream> 
#define ESCALA 0.2f

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

void Enemigo::UnloadImages(){ //REVISAR PROBLEMA DE SEGMENTATION FAULT, no se esta haciendo correctamente el Unload
    for(int i = 0; i < 3 ; ++i){
        UnloadTexture(enemigoImages[i]);
    }
}

void Enemigo::Update(int direction){
    position.x += direction;
}

Rectangle Enemigo::getRect() {
    return {position.x, position.y, float(enemigoImages[type - 1].width), float(enemigoImages[type - 1].height)};
}
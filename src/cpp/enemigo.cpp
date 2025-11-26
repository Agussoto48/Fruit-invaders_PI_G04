#include "include/enemigo.h"
#include <iostream> 
#define ESCALA 0.2f

extern "C" int movimientoEnemigo(int tipo, int parametro, Vector2* pos, float limite);

Texture2D Enemigo::enemigoImages[3] = {};

Enemigo::Enemigo(EnemigoTipo type, Vector2 position)
{
    this->type = type;
    this->position = position;
    
    int typeIndex = static_cast<int>(type);

    if(enemigoImages[typeIndex].id==0){

    switch(type){
            case EnemigoTipo::MANZANA:
                enemigoImages[0] = LoadTexture("sprites/Manzana.png");
                break;
            case EnemigoTipo::PINA:
                enemigoImages[1] = LoadTexture("sprites/Piña.png");
                break;
            case EnemigoTipo::SANDIA:
                enemigoImages[2] = LoadTexture("sprites/Sandia.png");
                break;
        }
    }
}

void Enemigo::Draw(){
    int typeIndex = static_cast<int>(type);
    DrawTextureV(enemigoImages[typeIndex], position, WHITE);
}

EnemigoTipo Enemigo::GetType(){
    return type;
}

void Enemigo::UnloadImages(){
    for(int i = 0; i < 3 ; ++i){
        if(enemigoImages[i].id != 0){
            UnloadTexture(enemigoImages[i]);
            enemigoImages[i].id = 0;
        }
    }
}

void Enemigo::Update(int direction){
    movimientoEnemigo(0, direction, &position, 0.0f);
}

bool Enemigo::MoveDown(int distance){
    float limiteInferior = GetScreenHeight() - 150.0f;
    
    int alcanzoLimite = movimientoEnemigo(1, distance, &position, limiteInferior);
    
    return (alcanzoLimite == 1);
}

Rectangle Enemigo::getRect() {
    int typeIndex = static_cast<int>(type); 
    return {position.x, position.y, float(enemigoImages[typeIndex].width), float(enemigoImages[typeIndex].height)};
}


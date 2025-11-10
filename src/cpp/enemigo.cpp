#include "include/enemigo.h"
#include <iostream> 
#define ESCALA 0.2f

// Declarar la función unificada de ensamblador
extern "C" int movimientoEnemigo(int tipo, int parametro, Vector2* pos, float limite);

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
        if(enemigoImages[i].id != 0){
            UnloadTexture(enemigoImages[i]);
            enemigoImages[i].id = 0;
        }
    }
}

void Enemigo::Update(int direction){
    // Llamar con tipo 0 (horizontal)
    movimientoEnemigo(0, direction, &position, 0.0f);
}

bool Enemigo::MoveDown(int distance){
    float limiteInferior = GetScreenHeight() - 150.0f;
    
    // Llamar con tipo 1 (abajo) y verificar retorno
    int alcanzoLimite = movimientoEnemigo(1, distance, &position, limiteInferior);
    
    // Retornar true si alcanzó el límite (game over)
    return (alcanzoLimite == 1);
}

Rectangle Enemigo::getRect() {
    return {position.x, position.y, float(enemigoImages[type - 1].width), float(enemigoImages[type - 1].height)};
}
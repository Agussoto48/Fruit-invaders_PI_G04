#include "include/enemigo.h"
#include <iostream> 
#define ESCALA 0.2f

// Declarar la función unificada de ensamblador
extern "C" int movimientoEnemigo(int tipo, int parametro, Vector2* pos, float limite, float velocidad);

Texture2D Enemigo::enemigoImages[3] = {};
float Enemigo::velocidadMultiplicador = 1.0f;

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
    // Llamar con tipo 0 (horizontal)
    movimientoEnemigo(0, direction, &position, 0.0f, velocidadMultiplicador);

}

bool Enemigo::MoveDown(int distance){
    float limiteInferior = GetScreenHeight() - 150.0f;
    
    // Llamar con tipo 1 (abajo) y verificar retorno
    int alcanzoLimite = movimientoEnemigo(1, distance, &position, limiteInferior, velocidadMultiplicador);

    
    return (alcanzoLimite == 1);
}

Rectangle Enemigo::getRect() {
    int typeIndex = static_cast<int>(type); 
    //return {position.x, position.y, float(enemigoImages[typeIndex].width), float(enemigoImages[typeIndex].height)};

    Rectangle rectangle;

    rectangle.x = position.x + 18;
    rectangle.y = position.y;
    rectangle.width = float(enemigoImages[typeIndex].width) / 1.5;
    rectangle.height = float(enemigoImages[typeIndex].height);

    return rectangle;
}


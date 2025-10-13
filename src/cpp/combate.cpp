#include "include/combate.h"
#define cellSize 100

Combate::Combate()
{
    enemigos = crearEnemigos();
}

Combate::~Combate(){

}

void Combate::Draw(){
    jugador.Draw();

    for(auto& enemigo: enemigos){
        enemigo.Draw();
    }
}
void Combate::Inputs(){
    if(IsKeyDown(KEY_LEFT)){
        jugador.MoveLeft();
    } else if(IsKeyDown(KEY_RIGHT)){
        jugador.MoveRight();
    }
}

std::vector<Enemigo> Combate::crearEnemigos(){
    std::vector<Enemigo> enemigos;
    for(int fila = 0; fila<5; ++fila){
        for(int columna=0; columna < 11; ++columna){

            int enemigoType;
            if(fila==0){
                enemigoType = 3;
            }
            else{
                if (fila ==1 || fila ==2){
                    enemigoType = 2;
                }
                else{
                    enemigoType = 1;
                }
            }

            float x = 75 + columna * cellSize;
            float y = 110 + fila * cellSize;
            enemigos.push_back(Enemigo(enemigoType, {x, y}));
        }
    }
    return enemigos;
}
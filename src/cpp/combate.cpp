#include "include/combate.h"
#include <iostream>

Combate::Combate()
{

}

Combate::~Combate(){

}

void Combate::Update(){
    for(auto& disparo: jugador.disparos){
        disparo.Update();
    }

    EliminarDisparoInactivo();
}

void Combate::Draw(){
    jugador.Draw();

    for(auto& disparo: jugador.disparos){
        disparo.Draw();
    }
}
void Combate::Inputs(){
    if(IsKeyDown(KEY_LEFT)){
        jugador.MoveLeft();
    } else if(IsKeyDown(KEY_RIGHT)){
        jugador.MoveRight();
    } else if(IsKeyDown(KEY_SPACE)){
        jugador.Disparar();
    }
}

void Combate::EliminarDisparoInactivo(){
    for(auto it = jugador.disparos.begin(); it != jugador.disparos.end();){
        if(!it -> active){
            it = jugador.disparos.erase(it);
        } else {
            ++it;
        }
    }
}
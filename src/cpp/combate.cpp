#include "include/combate.h"

Combate::Combate()
{

}

Combate::~Combate(){

}

void Combate::Draw(){
    jugador.Draw();
}
void Combate::Inputs(){
    if(IsKeyDown(KEY_LEFT)){
        jugador.MoveLeft();
    } else if(IsKeyDown(KEY_RIGHT)){
        jugador.MoveRight();
    }
}
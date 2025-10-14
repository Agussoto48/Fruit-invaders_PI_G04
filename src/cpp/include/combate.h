#pragma once
#include "player.h"

class Combate{
private:
    Player jugador;
    void EliminarDisparoInactivo();

public:
    Combate();
    ~Combate();
    void Draw();
    void Update();
    void Inputs();



};
#pragma once
#include "player.h"

class Combate{
private:
    Player jugador;

public:
    Combate();
    ~Combate();
    void Draw();
    void Update();
    void Inputs();



};
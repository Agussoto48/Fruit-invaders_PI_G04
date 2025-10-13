#pragma once
#include "player.h"
#include "enemigo.h"
#include <vector> 

class Combate{
private:
    Player jugador;
    std::vector<Enemigo> enemigos;
    std::vector<Enemigo> crearEnemigos();
public:
    Combate();
    ~Combate();
    void Draw();
    void Update();
    void Inputs();



};
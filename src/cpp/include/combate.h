#pragma once
#include "player.h"
#include "enemigo.h"
#include <vector> 

class Combate{
private:
    Player jugador;
    void EliminarDisparoInactivo();
    std::vector<Enemigo> enemigos;
    std::vector<Enemigo> crearEnemigos();
    void moverEnemigos();
    void moverAbajoEnemigos(int distance);
    void disparoEnemigo();
    int direccionEnemigos;
    //PARTE QUE FALTA IMPLEMENTAR DE ALIENS, QUE DISPAREN:
    std::vector<Disparo> enemigoDisparos;
    constexpr static float disparoEnemigoIntervalo = 0.35;
    float ultimoDisparoEnemigo;
public:
    Combate();
    ~Combate();
    void Draw();
    void Update();
    void Inputs();
};
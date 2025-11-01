#pragma once
#include "player.h"
#include "enemigo.h"
#include "obstacle.h"
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

    std::vector<Obstacle> CreateObstacle();
    std::vector<Obstacle> obstacles;
public:
    Combate();
    ~Combate();
    void Draw();
    void Update();
    void Inputs();
};
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
    std::vector<Disparo> enemigoDisparos;
    float disparoEnemigoIntervalo;
    float ultimoDisparoEnemigo;

    std::vector<Obstacle> CreateObstacle();
    std::vector<Obstacle> obstacles;
    void checkForCollisions();

    void GameOver();
    void InitGame();
    
    int arduinoFile;
    bool SetupArduino();
    void ReadArduinoInput();
    void NextLevel();
public:
    Combate();
    ~Combate();
    void Reset();
    void Draw();
    void Update();
    void Inputs();
    bool run;
    bool pausado;
    int lives;
    int score;
    int level;
};
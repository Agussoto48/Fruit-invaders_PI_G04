#include "include/combate.h"
#include <iostream>
#define cellSize 100

Combate::Combate()
{
    InitGame();
}

Combate::~Combate()
{
    Enemigo::UnloadImages();
}

void Combate::Update()
{
    if(run){ 
        for (auto &disparo : jugador.disparos)
        {
            disparo.Update();
        }
        moverEnemigos();

        disparoEnemigo();
        for (auto &disparo : enemigoDisparos)
        {
            disparo.Update();
        }

        for (auto &obstacle : obstacles)
        {
            obstacle.Draw();
        }
        EliminarDisparoInactivo();
        checkForCollisions();
    }
    else {
        if(IsKeyDown(KEY_ENTER)){
            //Aquí saltaría a la pantalla de gameover
            Reset();
            InitGame();
        }
    }
}

void Combate::Draw()
{
    jugador.Draw();

    for (auto &disparo : jugador.disparos)
    {
        disparo.Draw();
    }
    for (auto &enemigo : enemigos)
    {
        enemigo.Draw();
    }

    for (auto &disparo : enemigoDisparos)
    {
        disparo.Draw();
    }
}
void Combate::Inputs()
{
    if(run)
    {
        if (IsKeyDown(KEY_LEFT))
        {
            jugador.MoveLeft();
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            jugador.MoveRight();
        }
        else if (IsKeyDown(KEY_SPACE))
        {
            jugador.Disparar();
        }
    }
}

void Combate::EliminarDisparoInactivo()
{
    for (auto it = jugador.disparos.begin(); it != jugador.disparos.end();)
    {
        if (!it->active)
        {
            it = jugador.disparos.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto it = enemigoDisparos.begin(); it != enemigoDisparos.end();)
    {
        if (!it->active)
        {
            it = enemigoDisparos.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
std::vector<Enemigo> Combate::crearEnemigos()
{
    std::vector<Enemigo> enemigos;
    for (int fila = 0; fila < 5; ++fila)
    {
        for (int columna = 0; columna < 11; ++columna)
        {

            int enemigoType;
            if (fila == 0)
            {
                enemigoType = 3;
            }
            else
            {
                if (fila == 1 || fila == 2)
                {
                    enemigoType = 2;
                }
                else
                {
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

void Combate::moverEnemigos()
{
    for (auto &enemigo : enemigos)
    {
        if (enemigo.position.x + enemigo.enemigoImages[enemigo.type - 1].width > GetScreenWidth())
        {
            direccionEnemigos = -1;
            moverAbajoEnemigos(4);
        }
        if (enemigo.position.x < 0)
        {
            direccionEnemigos = 1;
            moverAbajoEnemigos(4);
        }
        enemigo.Update(direccionEnemigos);
    }
}

void Combate::moverAbajoEnemigos(int distance)
{
    for (auto &enemigo : enemigos)
    {
        enemigo.position.y += distance;
    }
}

void Combate::disparoEnemigo()
{

    double tiempoActual = GetTime();
    if (tiempoActual - ultimoDisparoEnemigo >= disparoEnemigoIntervalo && !enemigos.empty())
    {
        int randomIndex = GetRandomValue(0, enemigos.size() - 1);
        Enemigo &enemigo = enemigos[randomIndex];
        enemigoDisparos.push_back(Disparo({enemigo.position.x + enemigo.enemigoImages[enemigo.type - 1].width / 2,
                                           enemigo.position.y + enemigo.enemigoImages[enemigo.type - 1].height},
                                          6));
        ultimoDisparoEnemigo = GetTime();
    }
}

std::vector<Obstacle> Combate::CreateObstacle()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5;

    for (int i = 0; i < 4; i++)
    {
        float offSetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offSetX, float(GetScreenHeight() - 200)}));
    }

    return obstacles;
};

void Combate::checkForCollisions()
{
    for (auto &disparo : jugador.disparos)
    {
        auto it = enemigos.begin();
        while (it != enemigos.end())
        {
            if (CheckCollisionRecs(it->getRect(), disparo.getRect()))
            {
                it = enemigos.erase(it);
                disparo.active = false;
            }
            else
            {
                ++it;
            }
        }

        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), disparo.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                    disparo.active = false;
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    for (auto &disparo : enemigoDisparos)
    {
        if (CheckCollisionRecs(disparo.getRect(), jugador.getRect()))
        {
            disparo.active = false;
            lives--;
            if(lives == 0)
                GameOver();
        }

        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), disparo.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                    disparo.active = false;
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    for (auto &enemigo : enemigos)
    {
        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), enemigo.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }
} 

void Combate::GameOver(){
    run = false;

}
void Combate::InitGame(){
    enemigos = crearEnemigos();
    direccionEnemigos = 1;
    ultimoDisparoEnemigo = 0;
    lives = 3;
    run = true;
    obstacles = CreateObstacle();
}

void Combate::Reset(){
    enemigos.clear();
    enemigoDisparos.clear();
    obstacles.clear();
}
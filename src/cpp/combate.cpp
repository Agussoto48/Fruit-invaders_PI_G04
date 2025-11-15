#include "include/combate.h"
#include <iostream>
#define cellSize 100

extern "C" int asm_calcular_puntaje(int tipo_enemigo);

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

        EliminarDisparoInactivo();
        checkForCollisions();

        if(enemigos.empty()){
            NextLevel();
        }
    }
    else {
        if(IsKeyDown(KEY_ENTER)){
            //Aquí saltaría a la pantalla de gameover
            Reset();
            InitGame();
        }
    }
}

void Combate::NextLevel() {
    level++;
    enemigos = crearEnemigos();
    direccionEnemigos = 1;
    
    // Aumentar velocidad de movimiento (máximo 2.5x)
    if(Enemigo::velocidadMultiplicador < 2.5f) {
        Enemigo::velocidadMultiplicador += 0.15f;
    }

    //Reducir el intervalo hasta un miniomo, esto es la velocidad 
    //y frecuencia con la que el enemigo dispara
    if(disparoEnemigoIntervalo > 0.15) {
        disparoEnemigoIntervalo -= 0.01;
    }
    //ESTO HACE QUE DESPUES DE CADA 3 NIVELES
    //LOS ESCUDOS SE VUELVAN A CREAR 
    if(level % 3 == 1) {
        obstacles = CreateObstacle();
    }

    enemigoDisparos.clear();
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
    for (auto &obstacle : obstacles)
    {
        obstacle.Draw();
    }

    /*
    Para hacer debug
    DrawRectangleLinesEx(jugador.getRect(), 1, BLUE);
    for (auto &enemigo : enemigos)
        DrawRectangleLinesEx(enemigo.getRect(), 1, GREEN);
    for (auto &disparo : enemigoDisparos)
        DrawRectangleLinesEx(disparo.getRect(), 1, RED);
    for (auto &disparo : jugador.disparos)
        DrawRectangleLinesEx(disparo.getRect(), 1, RED);*/

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
std::vector<Enemigo> Combate::crearEnemigos(){
    std::vector<Enemigo> enemigos;
    for (int fila = 0; fila < 5; ++fila){ //SI QUIEREN CREAR MAS O MENOS ENEMIGOS, ES AQUI
        for (int columna = 0; columna < 5; ++columna){
            EnemigoTipo enemigoType;
            if (fila == 0)
            {
                enemigoType = EnemigoTipo::SANDIA;
            }
            else
            {
                if (fila == 1 || fila == 2)
                {
                    enemigoType = EnemigoTipo::PINA;
                }
                else
                {
                    enemigoType = EnemigoTipo::MANZANA;
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
        int typeIndex = static_cast<int>(enemigo.type);

        if (enemigo.position.x + enemigo.enemigoImages[typeIndex].width > GetScreenWidth())
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
        bool alcanzoLimite = enemigo.MoveDown(distance);
        if(alcanzoLimite) {
            std::cout << "*** GAME OVER DETECTADO ***" << std::endl;
            GameOver();
            return;
        }
    }
}

void Combate::disparoEnemigo(){
    double tiempoActual = GetTime();
    if (tiempoActual - ultimoDisparoEnemigo >= disparoEnemigoIntervalo && !enemigos.empty())
    {
        int randomIndex = GetRandomValue(0, enemigos.size() - 1);
        Enemigo &enemigo = enemigos[randomIndex];
        
        int typeIndex = static_cast<int>(enemigo.type);  
        
        // usar inicialización correcta de Vector2
        Vector2 disparoPos = {
            enemigo.position.x + enemigo.enemigoImages[typeIndex].width / 2,
            enemigo.position.y + enemigo.enemigoImages[typeIndex].height
        };
        
        enemigoDisparos.push_back(Disparo(disparoPos, 6, true));
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
                //Calculo del puntaje llamando a la funcion de ensamblador, misma logica de antes
                //Manzana(10) Pina(20) Sandia(30)
                int tipoEnemigo = static_cast<int>(it->type);
                score += asm_calcular_puntaje(tipoEnemigo);
                
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
            {
                GameOver();
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
    score = 0;
    level = 1;
    disparoEnemigoIntervalo = 0.35;
    Enemigo::velocidadMultiplicador = 1.0f;
    obstacles = CreateObstacle();
}

void Combate::Reset(){
    enemigos.clear();
    enemigoDisparos.clear();
    obstacles.clear();
}
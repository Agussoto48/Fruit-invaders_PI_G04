#include "include/combate.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sstream>
#include <sys/ioctl.h>
#define cellSize 100

extern "C" int asm_calcular_puntaje(int tipo_enemigo);

Combate::Combate()
{
    InitGame();
    SetupArduino();
}

Combate::~Combate()
{
    Enemigo::UnloadImages();
    if (arduinoFile != -1) {
        close(arduinoFile);
    }
}

bool Combate::SetupArduino() {
    std::cout << "Conectando a /dev/ttyACM0..." << std::endl;
    arduinoFile = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
    
    if (arduinoFile < 0) {
        std::cout << "No se pudo abrir /dev/ttyACM0, error: " << std::endl;
        std::cout << "Usando teclado." << std::endl;
        return false;
    }
    
    std::cout << "Conectado a /dev/ttyACM0 exitosamente!" << std::endl;
    
    struct termios tty;
    if(tcgetattr(arduinoFile, &tty) != 0) {
        std::cout << "Error obteniendo atributos serie" << std::endl;
        close(arduinoFile);
        arduinoFile = -1;
        return false;
    }
    
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    
    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;
    
    tty.c_cc[VTIME] = 10;
    tty.c_cc[VMIN] = 0;
    
    if(tcsetattr(arduinoFile, TCSANOW, &tty) != 0) {
        std::cout << "Error aplicando configuración serie" << std::endl;
        close(arduinoFile);
        arduinoFile = -1;
        return false;
    }
    
    tcflush(arduinoFile, TCIOFLUSH);
    
    usleep(1000000);
    
    std::cout << "Configuración serie completada!" << std::endl;
    return true;
}

void Combate::ReadArduinoInput() {
    if (arduinoFile == -1) return;
    
    static std::string buffer = "";
    
    char readBuffer[256];
    int n = read(arduinoFile, readBuffer, sizeof(readBuffer) - 1);
    
    if (n > 0) {
        readBuffer[n] = '\0';
        buffer += readBuffer;

        // Esta tecnica del buffer acumulador es parte de la sugerencia de la IA para la recepcion de los datos provenientes de las coordenadas que envia el joystick
        
        std::cout << "Buffer acumulado: '" << buffer << "'" << std::endl;
        
        size_t newlinePos;
        while ((newlinePos = buffer.find('\n')) != std::string::npos) {
            std::string line = buffer.substr(0, newlinePos);
            buffer = buffer.substr(newlinePos + 1);
            
            std::cout << "Procesando línea completa: '" << line << "'" << std::endl;
            
            size_t comma1 = line.find(',');
            size_t comma2 = line.find(',', comma1 + 1);
            
            if (comma1 != std::string::npos && comma2 != std::string::npos && 
                comma2 < line.length() - 1) {
                
                std::string xStr = line.substr(0, comma1);
                std::string botonStr = line.substr(comma2 + 1);
                
                std::cout << "X string: '" << xStr << "', Boton string: '" << botonStr << "'" << std::endl;
                
                try {
                    int x = std::stoi(xStr);
                    int boton = std::stoi(botonStr);
                    
                    std::cout << "Parsed - X: " << x << ", Boton: " << boton << std::endl;
                    
                    if (x >= 0 && x <= 1024) {
                        const int deadZoneLow = 450;
                        const int deadZoneHigh = 550;
                        
                        if (x < deadZoneLow) {
                            std::cout << "Moving LEFT" << std::endl;
                            jugador.MoveLeft();
                        } else if (x > deadZoneHigh) {
                            std::cout << "Moving RIGHT" << std::endl;
                            jugador.MoveRight();
                        } else {
                            std::cout << "In DEAD ZONE - no movement" << std::endl;
                        }
                        
                        if (boton == 0) {
                            std::cout << "DISPARO!" << std::endl;
                            jugador.Disparar();
                        }
                    } else {
                        std::cout << "X value out of range: " << x << std::endl;
                    }
                    
                } catch (const std::exception& e) {
                    std::cout << "Error parsing line '" << line << "': " << e.what() << std::endl;
                }
            } else {
                std::cout << "Invalid line format: '" << line << "'" << std::endl;
            }
        }
        
        if (buffer.length() > 1024) {
            buffer.clear();
        }
    }
}

void Combate::Update()
{
    if(run){ 
        ReadArduinoInput();
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
    else
    {
        Reset();
    }
}

void Combate::NextLevel() {
    level++;
    enemigos = crearEnemigos();
    direccionEnemigos = 1;
    
    // Aumentar velocidad de movimiento (máximo 2.5x)
    if(Enemigo::velocidadMultiplicador < 2.5f) {
        Enemigo::velocidadMultiplicador += 0.3f;
    }

    //Reducir el intervalo hasta un miniomo, esto es la velocidad 
    //y frecuencia con la que el enemigo dispara
    if(disparoEnemigoIntervalo > 0.15) {
        disparoEnemigoIntervalo -= 0.01;
    }
    //ESTO HACE QUE DESPUES DE CADA 4 NIVELES
    //LOS ESCUDOS SE VUELVAN A CREAR 
    if(level % 4 == 1) {
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
    if (run)
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
        else if(IsKeyPressed(KEY_P))
        {
            pausado = true;
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
        for (int columna = 0; columna < 6; ++columna){
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
        if (alcanzoLimite)
        {
            std::cout << "*** GAME OVER DETECTADO ***" << std::endl;
            GameOver();
            return;
        }
    }
}

void Combate::disparoEnemigo()
{
    double tiempoActual = GetTime();
    if (tiempoActual - ultimoDisparoEnemigo >= disparoEnemigoIntervalo && !enemigos.empty())
    {
        int randomIndex = GetRandomValue(0, enemigos.size() - 1);
        Enemigo &enemigo = enemigos[randomIndex];


        int typeIndex = static_cast<int>(enemigo.type);

        // usar inicialización correcta de Vector2
        Vector2 disparoPos = {
            enemigo.position.x + enemigo.enemigoImages[typeIndex].width / 2,
            enemigo.position.y + enemigo.enemigoImages[typeIndex].height};

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
            if (lives == 0)
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

void Combate::GameOver()
{
    run = false;
}

void Combate::InitGame()
{
    enemigos = crearEnemigos();
    direccionEnemigos = 1;
    ultimoDisparoEnemigo = 0;
    lives = 3;
    run = false;
    pausado = false;
    score = 0;
    level = 1;
    disparoEnemigoIntervalo = 0.35;
    Enemigo::velocidadMultiplicador = 1.0f;
    obstacles = CreateObstacle();
    arduinoFile = -1;
}

void Combate::Reset()
{
    enemigos.clear();
    enemigoDisparos.clear();
    obstacles.clear();

    InitGame();
}
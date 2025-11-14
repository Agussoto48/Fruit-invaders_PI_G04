#pragma once
#include<raylib.h>
#include<string>

enum semillas_posiciones 
{
    CERRADO = 0,
    ABIERTO = 1
};

class GameOver
{
private:
    Texture2D cuchillo;
    Texture2D chef;
    Texture2D boton_exit;
    Texture2D game_over_logo;
    Texture2D tabla_cortar;
    Texture2D semilla;

    Vector2 position_cuchillo;
    Vector2 position_chef;
    Vector2 position_exit;
    Vector2 position_game_over_logo;
    Vector2 position_tabla_cortar;
    Vector2 position_semilla_1;
    Vector2 position_semilla_2;

    
    void InitGameOver();
    void cargarTexturas();
    void InitPositionsTexturas();
    void HomeScreen();
    int opcion;
    int posicion_horizontal_cuchillo;
    int final_score;
    int semillas;
    Font font;

public:
    GameOver();
    ~GameOver();
    void Reset();
    void Draw(int score);
    void animacionSemillas();
    void Update();
    void Inputs();
    inline void setFinalScore(int score) {this->final_score = score;}
    bool run;
};
#pragma once
#include <vector>
#include <raylib.h>

enum Opciones
{
    START = 1,
    QUIT = 2
};
enum PosicionesAnimacion
{
    IZQUIERDA = 0,
    DERECHA = 1
};

class MenuInicio
{
private:
    Texture2D cuchillo;
    Texture2D boton_play;
    Texture2D boton_quit;
    Texture2D Logo;

    Vector2 position_cuchillo;
    Vector2 position_play;
    Vector2 position_quit;
    Vector2 position_logo;

    void Reset();
    void InitMenuInicio();
    void GameStart();
    void actualizarCuchillo();
    void QuitGame();
    int opcion;
    int posicion_horizontal_cuchillo;

public:
    MenuInicio();
    ~MenuInicio();
    void Draw();
    void Update();
    void Inputs();
    void animacionCuchillo();
    bool run;
    bool quit;
};
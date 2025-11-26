#pragma once
#include <vector>
#include <raylib.h>

enum opciones_pausa
{
    CONTINUE = 1,
    EXIT = 2
};
enum animacion_cuchillo
{
    LEFT = 0,
    RIGHT = 1
};

class Pausado
{
private:
    Texture2D receta;
    Texture2D boton_continue;
    Texture2D boton_exit;
    Texture2D cuchillo;

    Vector2 position_receta;
    Vector2 position_continue;
    Vector2 position_exit;
    Vector2 position_cuchillo;

    void InitPausado();
    void cargarTexturas();
    void InitPositionsTexturas();
    void GameContinue();
    void actualizarCuchillo();
    void ExitGame();
    int opcion;
    int posicion_horizontal_cuchillo;

public:
    Pausado();
    ~Pausado();
    void Reset();
    void Draw();
    void Update();
    void Inputs();
    void animacionCuchillo();
    bool run;
    bool exit;
};
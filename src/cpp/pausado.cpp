#include "include/pausado.h"

void Pausado::InitPausado()
{
    run = false;
    exit = false;
    opcion = CONTINUE;
    posicion_horizontal_cuchillo = LEFT;
    cargarTexturas();
    InitPositionsTexturas();
}
void Pausado::cargarTexturas()
{
    receta = LoadTexture("sprites/Recipe_pause.png");
    boton_continue = LoadTexture("sprites/Continue_button.png");
    boton_exit = LoadTexture("sprites/Exit_button.png");
    cuchillo = LoadTexture("sprites/Cuchillo.png");
}
void Pausado::InitPositionsTexturas()
{
    position_receta = {225, 0};
    position_continue = {425, 375};
    position_exit = {450, 550};
    position_cuchillo = {700, 595};
}
void Pausado::GameContinue()
{
    run = false;
}
void Pausado::actualizarCuchillo()
{
    switch (opcion)
    {
    case CONTINUE:
        position_cuchillo = {700.0f, 595.0f};
        break;
    case EXIT:
        position_cuchillo = {650.0f,730.0f};
        break;
    default:
        break;
    }
}
void Pausado::ExitGame()
{
    exit = true;
}
Pausado::Pausado()
{
    InitPausado();
}
Pausado::~Pausado()
{
}
void Pausado::Reset()
{
    InitPausado();
}
void Pausado::Draw()
{
    DrawTextureEx(receta, position_receta, 0.0, 1.7, WHITE);
    DrawTextureEx(cuchillo, position_cuchillo, 270.0, 0.5, WHITE);
    DrawTextureEx(boton_continue, position_continue, 0.0, 0.5, WHITE);
    DrawTextureEx(boton_exit, position_exit, 0.0, 0.4, WHITE);
}
void Pausado::Update()
{
    actualizarCuchillo();
}
void Pausado::Inputs()
{
    if (IsKeyPressed(KEY_UP))
    {
        if (opcion != CONTINUE)
        {
            opcion = CONTINUE;
        }
        else
        {
            opcion = EXIT;
        }
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        if (opcion != EXIT)
        {
            opcion = EXIT;
        }
        else
            opcion = CONTINUE;
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        if (opcion == CONTINUE)
        {
            GameContinue();
        }
        else
        {
            ExitGame();
        }
    }
}
void Pausado::animacionCuchillo()
{
    if (posicion_horizontal_cuchillo == LEFT)
    {

        position_cuchillo.x += 150;
        posicion_horizontal_cuchillo = RIGHT;
    }
    else
    {
        
        position_cuchillo.x -= 150.0f;
        posicion_horizontal_cuchillo = LEFT;
    }
}

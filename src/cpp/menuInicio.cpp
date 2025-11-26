#include "include/menuInicio.h"
#define ESCALA 3.0f
void MenuInicio::Reset()
{

}
void MenuInicio::InitMenuInicio()
{
    run = true;
    quit = false;
    opcion = START;
    posicion_horizontal_cuchillo = IZQUIERDA;
    cuchillo = LoadTexture("sprites/Cuchillo.png");
    boton_play = LoadTexture("sprites/play_button.png");
    boton_quit = LoadTexture("sprites/quit_button.png");
    logo = LoadTexture("sprites/Main_logo.png");

    position_cuchillo.x = 750.0f;
    position_cuchillo.y = 650.0f;

    position_play.x = 475.0f;
    position_play.y = 400.0f;
    position_quit.x = 475.0f;
    position_quit.y = 600.0f;

    position_logo.x = 340.0f;
    position_logo.y = -160.0f;
}
MenuInicio::MenuInicio()
{
    InitMenuInicio();
}
MenuInicio::~MenuInicio()
{
}
void MenuInicio::Draw()
{
    DrawTextureEx(cuchillo, position_cuchillo, 270.0, 0.5, WHITE);
    DrawTextureEx(boton_play, position_play, 0.0, 1.0, WHITE);
    DrawTextureEx(boton_quit, position_quit, 0.0, 1.0, WHITE);
    DrawTextureEx(logo, position_logo, 0.0, 1.3f, WHITE);
}
void MenuInicio::Update()
{
    actualizarCuchillo();
}
void MenuInicio::Inputs()
{

    if (run)
    {
        if (IsKeyPressed(KEY_UP))
        {
            if (opcion != START)
            {
                opcion = START;
            }
            else
            {
                opcion = QUIT;
            }
        }
        else if (IsKeyPressed(KEY_DOWN))
        {
            if (opcion != QUIT)
            {
                opcion = QUIT;
            }
            else
                opcion = START;
        }
        else if (IsKeyPressed(KEY_ENTER))
        {
            if (opcion == START)
            {
                GameStart();
            }
            else
            {
                QuitGame();
            }
        }
    }
}
void MenuInicio::GameStart()
{
    run = false;
}
void MenuInicio::actualizarCuchillo()
{
    switch (opcion)
    {
    case START:
        position_cuchillo.y = 600.0f;
        break;
    case QUIT:
        position_cuchillo.y = 800.0f;
        break;
    default:
        break;
    }
}
void MenuInicio::QuitGame()
{
    quit = true;
}
void MenuInicio::animacionCuchillo()
{
    if (posicion_horizontal_cuchillo == IZQUIERDA)
    {
        // Cambia a la derecha
        position_cuchillo.x = 800.0f;
        posicion_horizontal_cuchillo = DERECHA;
    }
    else
    {
        // Cambia a la derecha
        position_cuchillo.x = 750.0f;
        posicion_horizontal_cuchillo = IZQUIERDA;
    }
}
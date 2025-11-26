#include "include/gameOver.h"

void GameOver::Reset()
{
}
void GameOver::InitGameOver()
{
    cargarTexturas();
    InitPositionsTexturas();
    font = LoadFontEx("Font/monogram.ttf", 54, 0, 0);
    semillas = CERRADO;
    run = false;
}
void GameOver::cargarTexturas()
{
    cuchillo = LoadTexture("sprites/Cuchillo.png");
    chef = LoadTexture("sprites/Chef.png");
    boton_exit = LoadTexture("sprites/exit_button.png");
    game_over_logo = LoadTexture("sprites/GameOver.png");
    tabla_cortar = LoadTexture("sprites/Cuttin_Board.png");
    semilla = LoadTexture("sprites/Semilla.png");

    SetTextureFilter(game_over_logo, TEXTURE_FILTER_POINT);
}
void GameOver::InitPositionsTexturas()
{
    position_cuchillo = {500, 600};
    position_chef = {350, 580};

    position_exit = {447, 700};
    position_game_over_logo = {150, -175};

    position_tabla_cortar = {1075, 400};

    position_semilla_1 = {375, 811};
    position_semilla_2 = {758, 740};
}
GameOver::GameOver()
{
    InitGameOver();
}
GameOver::~GameOver()
{
}
void GameOver::Draw(int score)
{
    DrawTextureEx(game_over_logo, position_game_over_logo, 0.0, 1.7, WHITE);

    DrawTextureEx(cuchillo, position_cuchillo, 200.0, 0.5, WHITE);
    Color grisSuave = {185, 185, 185, 255};
    DrawTextureEx(tabla_cortar, position_tabla_cortar, 90.0, 1.5, grisSuave);

    DrawTextureEx(chef, position_chef, 270.0, 0.5, WHITE);
    DrawTextureEx(boton_exit, position_exit, 0.0, 0.9, WHITE);
    DrawTextureEx(semilla, position_semilla_1, 270.0, 0.4, WHITE);
    DrawTextureEx(semilla, position_semilla_2, 90.0, 0.4, WHITE);

    DrawTextEx(font, "SCORE: ", {375, 620}, 60, 2, RED);
    std::string numTexto = std::to_string(score);
    int cerosRest = 5 - numTexto.length();
    numTexto = std::string(cerosRest, '0') + numTexto;

    DrawTextEx(font, numTexto.c_str(), {600, 620}, 60, 2, RED);
}
void GameOver::Update()
{
}
void GameOver::Inputs()
{
    if (run)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            HomeScreen();
        }
    }
}
void GameOver::HomeScreen()
{
    run = false;
}

void GameOver::animacionSemillas()
{
    if (semillas == CERRADO)
    {
        // Cambia a la derecha
        position_semilla_1.x = 350;
        position_semilla_2.x = 783;
        semillas = ABIERTO;
    }
    else
    {
        // Cambia a la derecha
        position_semilla_1.x = 375;
        position_semilla_2.x = 758;
        semillas = CERRADO;
    }
}
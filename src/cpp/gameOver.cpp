#include "include/gameOver.h"

void GameOver::Reset()
{
}
void GameOver::InitGameOver()
{
    cargarTexturas();
    InitPositionsTexturas();
    font = LoadFontEx("Font/monogram.ttf", 54, 0, 0);
    run = false;
}
void GameOver::cargarTexturas()
{
    cuchillo = LoadTexture("sprites/Cuchillo.png");
    chef = LoadTexture("sprites/Chef.png");
    boton_exit = LoadTexture("sprites/Exit_button.png");
    game_over_logo = LoadTexture("sprites/GameOver.png");
    tabla_cortar = LoadTexture("sprites/Cuttin_Board.png");

    SetTextureFilter(game_over_logo, TEXTURE_FILTER_POINT); 
}
void GameOver::InitPositionsTexturas()
{
    position_cuchillo = {500, 500};
    position_chef = {200, 200};

    position_exit = {300, 300};
    position_game_over_logo = {400, 100};


    position_tabla_cortar = {1075, 400};
}
GameOver::GameOver()
{
    InitGameOver();
}
GameOver::~GameOver()
{
}
void GameOver::Draw()
{
    DrawTextureEx(cuchillo, position_cuchillo, 250.0, 0.5, WHITE);
    DrawTextureEx(chef, position_chef, 0.0, 0.5, WHITE);

    DrawTextureEx(boton_exit, position_exit, 0.0, 0.5, WHITE);
    DrawTextureEx(game_over_logo, position_game_over_logo, 0.0, 2.0, WHITE);

    DrawTextureEx(tabla_cortar, position_tabla_cortar, 90.0, 1.5, WHITE);

    DrawTextEx(font, "Game Over", {1000, 20}, 34, 2, YELLOW);
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
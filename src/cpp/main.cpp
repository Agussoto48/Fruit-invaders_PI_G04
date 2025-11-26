#include<raylib.h>
#include "include/combate.h"
#include <string>

#include <iostream>


int main() {


    int windowWidth = 1200;
    int windowHeight = 960;
    InitWindow(windowWidth, windowHeight, "Fruit Invaders"); 
    Texture2D background = LoadTexture("sprites/kitchen.png");
    Font font = LoadFontEx("Font/monogram.ttf", 54, 0, 0);
    Texture2D chefVidas = LoadTexture("sprites/Chef.png");
    Texture2D scoreIcono = LoadTexture("sprites/score.png");
    SetTargetFPS(60);
{
    Combate combate;
 
    while(!WindowShouldClose()){

        combate.Inputs();
        combate.Update();

        BeginDrawing();
        ClearBackground(GRAY);
        DrawTexturePro(
            background,
            { 0, 0, (float)background.width, (float)background.height },
            { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            { 0, 0 }, 0.0f, Fade(GRAY, 0.9f)
        );
        if(combate.run)
        {
            DrawTextEx(font, "Level 01", {1050, 20}, 34, 2, YELLOW);
        }
        else
        {
            DrawTextEx(font, "Game Over", {1000, 20}, 34, 2, YELLOW);
        }
        float x = 25.0;
        for(int i = 0; i < combate.lives; i++){
            DrawTextureEx(chefVidas, {x, 0}, 0.0 , 0.2f , WHITE);
            x += 50;
        }

        float posX = (GetScreenWidth() - scoreIcono.width * 0.2f) / 2 - 50;
        float posY = 0.0;
        DrawTextureEx(scoreIcono, {posX, posY}, 0.0f, 0.2f, WHITE);

        std::string numTexto = std::to_string(combate.score);
        int cerosRest = 5 - numTexto.length();
        numTexto = std::string(cerosRest, '0') + numTexto;

        float textX = posX + scoreIcono.width * 0.25f - 20.0f;
        float textY = posY + 35.0f;
        DrawTextEx(font, numTexto.c_str(), {textX, textY}, 34, 2, YELLOW);

        combate.Draw();

        EndDrawing();
    }
}
    UnloadTexture(background);
    CloseWindow();

    return 0;
}

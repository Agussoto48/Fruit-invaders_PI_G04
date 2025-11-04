#include<raylib.h>
#include "include/combate.h"

#include <iostream>


int main() {


    int windowWidth = 1200;
    int windowHeight = 960;
    InitWindow(windowWidth, windowHeight, "Fruit Invaders"); 
    Texture2D background = LoadTexture("sprites/kitchen.png");
    Font font = LoadFontEx("Font/monogram.ttf", 54, 0, 0);
    Texture2D chefVidas = LoadTexture("sprites/Chef.png");
    SetTargetFPS(60);
{
    Combate combate;
 
    while(!WindowShouldClose()){

        combate.Inputs();
        combate.Update();

        BeginDrawing();
        ClearBackground(GRAY);
        DrawTexturePro(
            background,                                      // textura
            { 0, 0, (float)background.width, (float)background.height }, // área original
            { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, // destino en pantalla
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
            //DrawTextureV(chefVidas, {x, 30}, WHITE);
            DrawTextureEx(chefVidas, {x, 0}, 0.0 , 0.2f , WHITE);
            x += 50;
        }

        combate.Draw();

        EndDrawing();
    }
}
    UnloadTexture(background);
    CloseWindow();

    return 0;
}
